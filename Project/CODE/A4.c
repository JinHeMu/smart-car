#include "A4.H"




//------------------����ͷ����--------------//

uint8 image_threshold = 46;  //ͼ����ֵ
uint8 dis_image[MT9V03X_CSI_W][MT9V03X_CSI_H];
uint8 output_img[120][188];

/*! 
 *  @brief      ��򷨶�ֵ��0.8ms����
 *  @date:   2018-10  
 *  @since      v1.2
 *  *image ��ͼ���ַ
 *  width:  ͼ���
 *  height��ͼ���
 *  @author     ZС��
 */

  
uint8 otsuThreshold(uint8 *image1, uint16 width, uint16 height)
{
    #define GrayScale 256
		uint8 *img_data;
    int pixelCount[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ���ظ���
    float pixelPro[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ�����ر���
    int i,j;   
    int Sumpix = width * height;   //�����ص�
    uint8 threshold = 0;
    img_data = image1;  //ָ���������ݵ�ָ��


    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)img_data[i * width + j]]++;  //������ֵ��Ϊ����������±�
          //   pixelCount[(int)image[i][j]]++;    ������ָ�������
        }
    }
    float u = 0;  
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / Sumpix;   //����ÿ������������ͼ���еı���  
        u += i * pixelPro[i];  //��ƽ���Ҷ�
    }

      
    float maxVariance=0.0;  //�����䷽��
    float w0 = 0, avgValue  = 0;  //w0 ǰ������ ��avgValue ǰ��ƽ���Ҷ�
    for(int i = 0; i < 256; i++)     //ÿһ��ѭ������һ��������䷽����� (����for����Ϊ1��)
    {  
        w0 += pixelPro[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷ�������ռ����ͼ��ı�����ǰ������
        avgValue  += i * pixelPro[i];        
        
        float variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //��䷽��   
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

