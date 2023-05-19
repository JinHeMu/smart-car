# smart-car

## 2022/11/30

 备战第十八届智能车，参加智能视觉组，以此来记录此次比赛经历和过程，为以后的自己提供相应的参考
 1.首先该做的事情是将硬件和底层做出来，在算法实现的基础上确定好大致框架，例如RT1064最小系统版的学习和RT-Thread操作系统的学习，确定好中央处理器模块的框架，然后再进行PCB板子的打板，焊接和测试，最后进行相应的图像识别，路径规划等相应的算法。
 2.2022年即将结束，2022造神计划实现的不尽人意，但终是还是有一定的收获，这一年给我的启示就一句话：坚持，保持热爱，持之以恒的去做某一件事。接下来的生活中，再一次提醒自己坚持去做，比如背英语，健身。

## 2023/1/16

 RT-Thread操作系统大致学习完毕，大多数驱动和设备都调试完成，接下来准备制作第一项功能-----使用蓝牙串口来控制小车运动

## 2013/1/19

蓝牙串口来控制小车运动基本实现，发现一个巨大问题，在运动中平移或者转向误差很大，需要一些pid等控制来减小误差，我发现光靠自己想会浪费很多时间，多看看报告和论文资料，来快速解决问题。RTThread操作系统还是比较难以上手。

### PID 在做车中的应用  

首先，电机的控制是通过编码器返回的实时速度与期望速度进行 PID 的调节从而实现速度上的闭环，从而实现小车的速度与我期望的速度实现真正的相同。这里我们采用了增量式 PID 进行调节，当小车比期望速度慢时，通过计算给出一个比较大的占空比，使电机加速旋转，当小车比期望速度快时，通过计算输出一个小的占空比，使电机减速，从而实现速度上的跟随。  
其次，车位矫正通过在图像中寻找到目标图片中心和我们设定的适合车进行拾取的点做 x 轴和 y 轴上两个方向的偏差，当偏差比较大时，我认为此时距离图片比较远，需要大一点的速度进行调整，当偏差较小时，我认为此时离目标图片比较近，这个时候不需要太大的速度，避免超调。在跑到目标点的过程中，这个时候车只需要给一个固定速度跑就行，因为这个时候车跑的是直线。除此之外，我们改变原有的单环控制算法，使用串级 PID 的控制思路在多个控制中，对智能车的性能指标有了显著的提升，在 M 车的控制中，引入角速度环，抗干扰效果好，再将角度环串在角速度环上，最后速度环叠加在角度环上，方向环上也是同样的方法，在方向环的控制中，会出现大量的跳变输出，对此需要对按照需要对 pid 的输出做一个递推滤波，保证输出的平稳。  
需要注意的是，串级 pid 控制中，需要先整定内环参数，内环精调，外环粗调，可以先将外环给一个固定值，整定内环参数，当调整好之后，再整定外环，每一级的输出再限幅，可以使控制精确快速不超调，并且每一级之间控制周期需要大于至少 2 倍。在方向环的控制中，使用动态 p，使 p 的大小呈二次函数关系，并乘以速度系数，对输出做限幅，经过测试，可以很快速通过图片对车身进行调整。

### 屏幕的多级显示，更直观方便调参和显示数据

通过一个结构体来进行屏幕多级的切换，方便了显示数据，但是如何调参还需要进一步优化

```c
typedef struct
{
  uint8 current; //当前状态索引号
  uint8 next;   //向下一个
  uint8 enter;  //确定
  uint8 back;   //退出
  void (*current_operation)(void); //当前状态应该执行的操作
} Menu_table;
```

**问题1**：编码值和pwm占空比和实际距离如何对应？
通过一个增量式pid，输入目标编码器值和当前编码器值，计算出占空比就可以了，不需要单独计算，编码器值可以对应实际距离

**问题2**：麦克纳姆轮如何解算x,y轴上的距离？

## 2023/2/7

加入了转向环，基本完成麦克纳姆轮的逆解算和正解算，加入了里程计和陀螺仪，可以进行下一步坐标的计算  

识别A4纸上的坐标（总钻风识别代码过于麻烦，先使用openart识别坐标）
图像如何进行预处理？  
1.大津法二值化

如何提取坐标，并且与实际对应？

1.搜索矩形，提取矩形长度和宽度  
①r.x() #获取矩形的左上角x像素坐标  
②r.y() #获取矩形的左上角y像素坐标  
③r.w() #获取矩形的宽度  
④r.h() #获取矩形的高度  

2.搜索圆点  
确定完矩形后，需要在矩形中找到所有的圆点，这时候就要用到find_circles函数，为了方便观察，同样使用draw_circle将搜索到的圆画到图像上。  
使用find_circles时需要设置一些参数：  
①roi = r.rect() #设置圆点搜索区域，这里设置为之前确定的矩形  
②threshold = 1800 #和搜索矩形中参数相同，值越小，越容易搜索圆形特征  
③r_min = 1 #设置搜索圆的最小半径  
④r_max = 5 #设置搜索圆的最大半径  
⑤r_step = 1 #设置搜索圆的步长（每移动多少像素检测一次圆）  
设置完成后运行，在图像上便能看到矩形和圆点已经被正确的标识到。  

搜索到圆形后，一般用到三个方法来获取圆形的数据  
①c.x() #获取圆形的x像素坐标  
②c.y() #获取圆形的y像素坐标  
③c.r() #获取圆形的半径  
当然，现在的坐标还只是像素坐标，还需要按照等比的方式计算为真实坐标。  
在OpenART mini中，图像的左上角为（0，0），从上向下，Y轴增加，从左向右，X轴增加，单位为像素。  

在X轴方向上的计算为（圆点X像素坐标–矩形X像素坐标）/ 矩形的宽度得到圆点在X轴上的比例，再乘上实际最大坐标35，便能得到以0为基点的X坐标，再加上1得到真实的X坐标。

在Y轴方向上的计算为（圆点Y像素坐标–矩形Y像素坐标）/ 矩形的高度得到圆点在Y轴上的比例，再乘上实际最大坐标25，因为Y轴和实际是相反的，所以还需要用25减去计算的值便能得到到以0为基点的Y坐标，再加上1得到真实的Y坐标。

## 2023/2/9

  已经实现openart识别坐标点并与单片机形成通讯，接下来如何使用这些坐标点，并跑到相应的位置还需要考虑和解决

1. 如何进行路径规划？  
2. 如何让小车进行规划后的移动？  
3. 如何进行姿态矫正？  

为了纠正由于陀螺仪和编码器积分导致的误差，车模在到达图片附近后再进行一定的矫正。矫正的误差有偏移距离误差和角度误差。图像经过二值化后，把图片的边缘凸显出来，计算出图片行列位置的重心，通过 pid 控制使重心到达理想位置。然后遍历图像，找到图片的上顶点和左右顶点，计算出角度，从而纠正车模的姿态。

OPENART连接TFT显示屏需要一个PCB转接板  
  已通过用IPS114形成显示，但是好像无法同时和串口功能同时使用？

如何减小陀螺仪的误差？

1. 零点漂移
2. 陀螺仪的误差分析（系统性误差和随机误差）
3. 加速度计数据和速度数据的滤波（减少震动）→对速度进行相应的滤波
4. 温漂  

加速度包括重力加速度，陀螺仪测量的是角速度，需要通过积分成等其他手段来获取角度。

## 2023/3/26

已经初步完成小车跑点运动，具体性能和转向方式有待提升  

接下来完成识别卡片并和小车实现通讯，同时也能够进行位姿调节，做到跑到点位然后实现检测的功能  

下一步,矫正和识别通讯都完成之后，进行搬运并且优化路径规划
  
完成以上功能后，已经完成70%，可以完成上一届比赛之后，开始进行对于目标检测的功能实现

## 2023/4/3

由于没有场地，绝大多数无法进行良好的测试，还有需要制做和调整的部分比较多，接下来这些部分就是之后所需要的主要目标：

1. 板子的焊接
虽然板子画了好了，但是在焊接过程上出现了部分问题，无法将板子完整的焊接并且进行相应的测试，具体板子性能还未知，再加上机械部分还未想好如何实现具体流程导致无法修改和改良板子的性能。

2. 部分参数的调节
对于增量式pid， 位置式pid，角度环pid的调节只是粗调，能用，但是具体性能还未测试，这些是亟待提升和优化的。同时对于摄像头来说，均匀的光线是非常重要，对于光线的调节也需要测试。

3. 识别的准确率
识别的准确率是比赛的绝对关键，但是由于光线不均匀，场地颜色不均，模型准确率较差，数据集较少等等原因导致准确率无法达到令人满意的程度，所以这个地方也有待提升。

4. 部分算法的优化和调节
对于比赛流程来说，如何实现并符合规则上的搬运也需要思考，同时动态路径规划有没有必要写，怎么优化也需要考虑。同时对于目标检测如何对应的实际的坐标图中也需要思考和编写。同时，如果里程计和陀螺仪在实际运用中如果误差过大。如何优化也是个大问题，是否选择其他的运动方式也需要考虑。同时如果实在是无法正确识别目标。那么应该如何舍弃或者跳过该目标寻找下一个目标也需要考虑。

5. 机械部分
对于上述的目标检测和图像识别都需要一个合适的高度和角度，这个如何调节也需要大量测试寻找最优值，同时也要和算法部分一一对应。同时如何搬运，使用几个维度的机械臂，如何将卡片放置到小车上，或者使用仓之类的来存放也需要再思考，并且这个也要和算法部分一一对应

在场地找到之前，可以提升的如下：

1. 板子的绘制和焊接，测试和提升
2. 图像识别准确度的提升，图片数据的增广，训练时的参数调节等
3. 对于动态路径算法的编写，可以先使用这种算法来跑，如果有更好解可以后期再修改

## 2023/4/4

今天焊了半天板子，需要改进的部分有：

1. 部分元器件的布局，有些地方过于拥挤导致焊接比较困难，容易出现虚焊的问题
2. 在姿态传感器上需要加两个螺丝来固定，放置因为运动而导致的抖动
3. 部分元器件是否还需要？部分功能布局还需改动

**主板问题**:cr引脚上电不知道是不是有问题导致下部分3v3电源无法供电，舵机电源可能也有点问题，只有2.3v但是又偶尔和电池电压一样，其余问题暂时没有发现

**驱动板问题**:右边电机正转电压正常，反转电压接近0，好像是芯片问题，暂时没有找到具体问题所在
左半部分电机正转也正常，但是切换到反转之后好像是场效应管冒烟，然后有吱吱的响声，电压正常，但是再接正转电压只会慢慢上升

## 2023/4/10

目前的问题：

1. 矫正的问题，矫正的时候由于帧率较低，而且很容易判断错误，容易照到小车的前面，所以在这一块机械结构和算法上面都得优化，然后是否需要找到角点来调节小车位姿也是一个需要考虑的问题。  

2. 场地的问题，因为没有场地，所以在测试车上无法得到良好的反馈，比如说如何确定小车当前坐标和是否跑到相应的点位，而且蓝色背景布挺重要的，可以减少外界的干扰部分，所以场地对于后期的调试非常重要

3. 板子问题，板子已经是第二版，马上就能到货，到时候在学长的帮助下将板子及时焊好，用加热台热风枪等等一定要放置虚焊等等问题，这个是未来一个星期必须完成的任务

4. 移动模式问题，现在暂定的是让小车直接平移到目标点位，但是由于场地的问题无法得到反馈，但是通过查看别的大佬去年的视频，大部分是选择先转向然后前进的行径模式，所以这个部分还值得优化。

5. 路径优化问题，路径优化这个问题已经提出很久了但是依然没有得到及时的编写，这个属实是不应该 应该及时将提出的问题解决，而不是一直拖延下去，这个是绝对需要修改的地方
 目前暂定的路径优化就是将坐标排列好之后，分别跑到相应的位置，并且搬运到相应的位置。
  后期路径规划应该是动态路径规划，先从最近的点开始，搬运到相应位置，然后这个时候通过计算得出下一个最近的点，然后重复此过程并且搬运到指定地方。

## 2023/4/29

 根据逐飞的方案，重新制定和规划方案和目标

 初步思路
初步思路如下：

1. 使用编码器和陀螺仪的数据记录，完成车模的全局定位，可以实时得到车模的当前位置以及当前角度；
2. 对于有坐标点的目标，使用OpenART mini识别坐标，并将坐标信息发送到RT1064；
3. 利用全局定位移动到目标位置附近，使用总钻风矫正车模位置，确保车模移动到图片正方向；
4. 使用OpenART mini识别图片内容，根据内容进行分类搬运；
5. 搬运也是基于全局定位实现，搬运过程中如果识别到四周的黄色边框，进行车模的位置和角度矫正；
6. 循环第3到5步，直到有框图片全部搬运完成；
7. 对于没有坐标点的图片目标板，可以基于目标检测技术，使用纯遍历或者语音识别控制车模移动到无框图片，并完成识别和搬运；
8. 所有图片搬运完成，返回车库；
里面的关键点有：全局定位、OpenART识别处理、总钻风矫正处理、OpenART目标检测、语音控制。  

### 全局定位， 运动函数接口，位置控制函数接口

全局定位放在定时器中断中运行，并实时更新自己的当前位置，不需要到达位置后手动更新  

   1. 在有目标坐标时，车模会自动移动到目标位置，如果没有目标坐标时，可以任意控制车模的移动姿态，方便矫正车身。
   对这部分的要求如下：
   2. 我可以使用函数统一控制车模的移动姿态，参数有移动速度，移动方向，自身旋转方向（后面统称为移动接口函数）。
   3. 控制过程中，调整车模的旋转方向，但整体移动方向不能收到影响。比如我设置车模的旋转角度为0度，往0度方向移动，中途我修改自身旋转角度为30度，车模的整体方向还是往0度移动而不能有角度偏移。
   4. 生成一张和场地一样等比例缩略全局地图，并显示车模在场地上的位置，车模的任意移动都实时更新到地图上。
   5. 再封装一个位置控制函数，参数为目标坐标，这个函数会计算自身的实时坐标与目标坐标的相对角度，自动调用移动控制接口移动过去


2.识别A4纸：优化的方案是在识别到外接矩形后，使用r.corners()函数对内部矩形的角点进行搜索，然后使用img1.rotation_corr()矫正内部矩形（也就是A4纸），使用矫正后的图像识别坐标信息，最后显示到图像的左上角，效果如下图所示。可以看到在A4纸倾斜的情况下也有较好的效果。已经完成

3.识别图像：调用色块识别函数，使用阈值，即可准确的找到目标图片。已经完成

4.总钻风矫正图片（能用openart来矫正？）已经完成，
但是如果角度很正，角点则会乱识别，可能需要改进，如何通过一个数据包来发送所有矫正数据也需要完成

5.目标检测和语音控制

车模控制
1.出库
2.路径规划（单搬运）
3.校准位置（图片或者边界赛道）已完成矫正和边界矫正基本程序，如何和单片机通讯，控制效果需要提升
4.搬运模式
5.检测无框卡片
6.语音控制
