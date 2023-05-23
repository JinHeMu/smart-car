#include "A4.H"




//------------------摄像头参数--------------//

uint8 image_threshold = 46;  //图像阈值
uint8 dis_image[MT9V03X_CSI_W][MT9V03X_CSI_H];
uint8 output_img[120][188];

/*! 
 *  @brief      大津法二值化0.8ms程序
 *  @date:   2018-10  
 *  @since      v1.2
 *  *image ：图像地址
 *  width:  图像宽
 *  height：图像高
 *  @author     Z小旋
 */

  
uint8 otsuThreshold(uint8 *image1, uint16 width, uint16 height)
{
    #define GrayScale 256
		uint8 *img_data;
    int pixelCount[GrayScale] = {0};//每个灰度值所占像素个数
    float pixelPro[GrayScale] = {0};//每个灰度值所占总像素比例
    int i,j;   
    int Sumpix = width * height;   //总像素点
    uint8 threshold = 0;
    img_data = image1;  //指向像素数据的指针


    //统计灰度级中每个像素在整幅图像中的个数  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)img_data[i * width + j]]++;  //将像素值作为计数数组的下标
          //   pixelCount[(int)image[i][j]]++;    若不用指针用这个
        }
    }
    float u = 0;  
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / Sumpix;   //计算每个像素在整幅图像中的比例  
        u += i * pixelPro[i];  //总平均灰度
    }

      
    float maxVariance=0.0;  //最大类间方差
    float w0 = 0, avgValue  = 0;  //w0 前景比例 ，avgValue 前景平均灰度
    for(int i = 0; i < 256; i++)     //每一次循环都是一次完整类间方差计算 (两个for叠加为1个)
    {  
        w0 += pixelPro[i];  //假设当前灰度i为阈值, 0~i 灰度像素所占整幅图像的比例即前景比例
        avgValue  += i * pixelPro[i];        
        
        float variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //类间方差   
        if(variance > maxVariance) 
        {  
            maxVariance = variance;  
            threshold = i;  
        }  
    } 

//		
//		for (i = 0; i < height; i++)
//    {
//        for (j = 0; j < width; j++)
//        {
//					if(img_data[i * width + j]<= 50)
//					{
//						img_data[i * width + j]=0;
//					}else
//					{
//						img_data[i * width + j]=255;
//					}
//        }
//    }
		rt_kprintf("threshold:%d", threshold);

    return threshold;
    
}

void Binarization(uint8 input_pic[120][188], uint8 output_pic[120][188], int threshold)
{
		for(int i=0;i<120;i++)
	{
		for(int j=0;j<188;j++)
		{
			if(input_pic[i][j]>threshold)
				output_pic[i][j] = 255;
			else
				output_pic[i][j] = 0;		
		}
	
	}
}

//void mt9v03x_init()
//{
//	mt9v03x_csi_init();
//}

