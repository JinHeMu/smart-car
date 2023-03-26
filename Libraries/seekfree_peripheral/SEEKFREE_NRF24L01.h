/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		NRF24L01����ģ��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
					���߶��壺
					------------------------------------ 
					    NRF24L01    ��Ƭ��            
					    CE          �鿴SEEKFREE_NRF24L01�ļ��ڵ�NRF_CE_PIN   �궨��
					    CSN         �鿴SEEKFREE_NRF24L01�ļ��ڵ�NRF_CSN_PIN  �궨��
					    MOSI        �鿴SEEKFREE_NRF24L01�ļ��ڵ�NRF_MOSI_PIN �궨��
					    SCK         �鿴SEEKFREE_NRF24L01�ļ��ڵ�NRF_SCK_PIN  �궨��
					                                   
					    MISO        �鿴SEEKFREE_NRF24L01�ļ��ڵ�NRF_MISO_PIN �궨��
					    IRQ         �鿴SEEKFREE_NRF24L01�ļ��ڵ�NRF_IRQ_PIN  �궨��
					------------------------------------ 
 ********************************************************************************************************************/




#ifndef _SEEKFREE_NRF24L01_h
#define _SEEKFREE_NRF24L01_h





// SPI������   SPI Commands
#define NRF_CMD_R_REGISTER          0x00    // ���Ĵ���             Read command and status registers
#define NRF_CMD_W_REGISTER          0x20    // д�Ĵ���             Write command and status registers
#define NRF_CMD_R_RX_PAYLOAD        0x61    // ��RXFIFO             Read RX-payload
#define NRF_CMD_W_TX_PAYLOAD        0xA0    // ��TXFIFO             Write TX-payload
#define NRF_CMD_FLUSH_TX            0xE1    // ��TXFIFO             Flush TX FIFO
#define NRF_CMD_FLUSH_RX            0xE2    // ��RXFIFO             Flush RX FIFO
#define NRF_CMD_REUSE_TX_PL         0xE3    // �ط�TX����           Used for a PTX device Reuse last transmitted payload.
#define NRF_CMD_R_RX_PL_WID         0x60    // ��RX���ݿ�           Read RX payload width for the top R_RX_PAYLOAD in the RX FIFO
#define NRF_CMD_W_ACK_PAYLOAD       0xA8    // ��ACK������          Write Payload to be transmitted together with ACK packet on PIPE
#define NRF_CMD_W_TX_PAYLOAD_NOACK  0xB0    // ��ACK������          Disables AUTOACK on this specific packet.
#define NRF_CMD_NOP                 0xFF    // �ղ���"������        No Operation.Might be used to read the STATUS register

// SPI�Ĵ�����ַ     Registers Addresses
#define NRF_REG_CONFIG              0x00    // ���üĴ���           Configuration Register
#define NRF_REG_EN_AA               0x01    // ʹ���Զ�Ӧ��         Enable ��Auto Acknowledgment��
#define NRF_REG_EN_RXADDR           0x02    // ʹ��RX����ͨ��       Enabled RX Addresses
#define NRF_REG_SETUP_AW            0x03    // ���õ�ַ���         Setup of Address Widths
#define NRF_REG_SETUP_RETR          0x04    // �����Զ��ط�         Setup of Automatic Retransmission
#define NRF_REG_RF_CH               0x05    // Ƶ������             RF Channel
#define NRF_REG_RF_SETUP            0x06    // RF����               RF Setup Register
#define NRF_REG_STATUS              0x07    // ״̬                 Status Register
#define NRF_REG_OBSERVE_TX          0x08    // ���ͼ��             Transmit observe register
#define NRF_REG_RPD                 0x09    // ���չ��ʼ��         Received Power Detector (nRF24L01+ only),Carrier Detect(nRF24L01 only)
#define NRF_REG_RX_ADDR_P0          0x0A    // ����ͨ��0��ַ        Receive address data pipe 0
#define NRF_REG_RX_ADDR_P1          0x0B    // ����ͨ��1��ַ        Receive address data pipe 1
#define NRF_REG_RX_ADDR_P2          0x0C    // ����ͨ��2��ַ        Receive address data pipe 2
#define NRF_REG_RX_ADDR_P3          0x0D    // ����ͨ��3��ַ        Receive address data pipe 3
#define NRF_REG_RX_ADDR_P4          0x0E    // ����ͨ��4��ַ        Receive address data pipe 4
#define NRF_REG_RX_ADDR_P5          0x0F    // ����ͨ��5��ַ        Receive address data pipe 5
#define NRF_REG_TX_ADDR             0x10    // ���͵�ַ             Transmit address
#define NRF_REG_RX_PW_P0            0x11    // ����ͨ��0���ݿ��    Number of bytes in RX payload in data pipe 0
#define NRF_REG_RX_PW_P1            0x12    // ����ͨ��1���ݿ��    Number of bytes in RX payload in data pipe 1
#define NRF_REG_RX_PW_P2            0x13    // ����ͨ��2���ݿ��    Number of bytes in RX payload in data pipe 2
#define NRF_REG_RX_PW_P3            0x14    // ����ͨ��3���ݿ��    Number of bytes in RX payload in data pipe 3
#define NRF_REG_RX_PW_P4            0x15    // ����ͨ��4���ݿ��    Number of bytes in RX payload in data pipe 4
#define NRF_REG_RX_PW_P5            0x16    // ����ͨ��5���ݿ��    Number of bytes in RX payload in data pipe 5
#define NRF_REG_FIFO_STATUS         0x17    // FIFO״̬             FIFO Status Register
#define NRF_REG_DYNPD               0x1C    // ����̬���ݰ�����   Enable dynamic payload length
#define NRF_REG_FEATURE             0x1D    // �����Ĵ���           Feature Register

// SPI�Ĵ�������     Registers Config

// 0x00 CONFIG  ���üĴ���
#define NRF_RCD_RX_DR_EN            0<<6    // bit6=MASK_RX_DR=0=ӳ��RX_DR�ж�(���ݽ������ʱIRQ���L)
#define NRF_RCD_RX_DR_DI            1<<6    // bit6=MASK_RX_DR=1=����RX_DR�ж�
#define NRF_RCD_TX_DS_EN            0<<5    // bit5=MASK_TX_DR=0=ӳ��TX_DR�ж�(���ݷ������ʱIRQ���L)
#define NRF_RCD_TX_DS_DI            1<<5    // bit5=MASK_TX_DR=1=����TX_DR�ж�
#define NRF_RCD_MAX_RT_EN           0<<4    // bit4=MASK_MAX_RT=0=ӳ��MASK_MAX_RT�ж�(�����ط�����ʱIRQ���L)
#define NRF_RCD_MAX_RT_DI           1<<4    // bit4=MASK_MAX_RT=1=����MASK_MAX_RT�ж�
#define NRF_RCD_CRC_DI              0<<3    // bit3=EN_CRC=0=����CRC
#define NRF_RCD_CRC_EN              1<<3    // bit3=EN_CRC=1=����CRC
#define NRF_RCD_CRC8                0<<2    // bit2=CRCO=0=CRC-8=1byte
#define NRF_RCD_CRC16               1<<2    // bit2=CRCO=1=CRC-16=2byte
#define NRF_RCD_PWR_DOWN            0<<1    // bit1=PWR_UP=0=��Դ�ر�ģʽ
#define NRF_RCD_PWR_UP              1<<1    // bit1=PWR_UP=1=��Դ����ģʽ
#define NRF_RCD_PRIM_PRX            1       // bit0=PRIM_RX=1=����ģʽ
#define NRF_RCD_PRIM_PTX            0       // bit0=PRIM_RX=0=����ģʽ

// 0x01 EN_AA   ʹ���Զ�Ӧ��
#define NRF_RCD_ALL_PIPE_AA_DI      0       // bit5:0=000000=����ȫ��ͨ����AA
#define NRF_RCD_PIPE5_AA_DI         0<<5    // bit5=ENAA_P5=0=����ͨ��5��AA(Auto Ack�Զ�Ӧ��)
#define NRF_RCD_PIPE5_AA_EN         1<<5    // bit5=ENAA_P5=1=����ͨ��5��AA
#define NRF_RCD_PIPE4_AA_DI         0<<4    // bit4=ENAA_P4=0=����ͨ��4��AA
#define NRF_RCD_PIPE4_AA_EN         1<<4    // bit4=ENAA_P4=1=����ͨ��4��AA
#define NRF_RCD_PIPE3_AA_DI         0<<3    // bit3=ENAA_P3=0=����ͨ��3��AA
#define NRF_RCD_PIPE3_AA_EN         1<<3    // bit3=ENAA_P3=1=����ͨ��3��AA
#define NRF_RCD_PIPE2_AA_DI         0<<2    // bit2=ENAA_P2=0=����ͨ��2��AA
#define NRF_RCD_PIPE2_AA_EN         1<<2    // bit2=ENAA_P2=1=����ͨ��2��AA
#define NRF_RCD_PIPE1_AA_DI         0<<1    // bit1=ENAA_P1=0=����ͨ��1��AA
#define NRF_RCD_PIPE1_AA_EN         1<<1    // bit1=ENAA_P1=1=����ͨ��1��AA
#define NRF_RCD_PIPE0_AA_DI         0       // bit0=ENAA_P0=0=����ͨ��0��AA
#define NRF_RCD_PIPE0_AA_EN         1       // bit0=ENAA_P0=1=����ͨ��0��AA

// 0x02 EN_RXADDR   ʹ��RX����ͨ��
#define NRF_RCD_PIPE5_RX_DI         0<<5    // bit5=ERX_P5=0=����ͨ��5
#define NRF_RCD_PIPE5_RX_EN         1<<5    // bit5=ERX_P5=1=����ͨ��5
#define NRF_RCD_PIPE4_RX_DI         0<<4    // bit4=ERX_P4=0=����ͨ��4
#define NRF_RCD_PIPE4_RX_EN         1<<4    // bit4=ERX_P4=1=����ͨ��4
#define NRF_RCD_PIPE3_RX_DI         0<<3    // bit3=ERX_P3=0=����ͨ��3
#define NRF_RCD_PIPE3_RX_EN         1<<3    // bit3=ERX_P3=1=����ͨ��3
#define NRF_RCD_PIPE2_RX_DI         0<<2    // bit2=ERX_P2=0=����ͨ��2
#define NRF_RCD_PIPE2_RX_EN         1<<2    // bit2=ERX_P2=1=����ͨ��2
#define NRF_RCD_PIPE1_RX_DI         0<<1    // bit1=ERX_P1=0=����ͨ��1
#define NRF_RCD_PIPE1_RX_EN         1<<1    // bit1=ERX_P1=1=����ͨ��1
#define NRF_RCD_PIPE0_RX_DI         0       // bit0=ERX_P0=0=����ͨ��0
#define NRF_RCD_PIPE0_RX_EN         1       // bit0=ERX_P0=1=����ͨ��0

// 0x03 SETUP_AW    ���õ�ַ���
#define NRF_RCD_AW_3byte            1       // bit1:0=01=��ַ���Ϊ3byte
#define NRF_RCD_AW_4byte            2       // bit1:0=10=��ַ���Ϊ4byte
#define NRF_RCD_AW_5byte            3       // bit1:0=11=��ַ���Ϊ5byte

// 0x04 SETUP_RETR  �����Զ��ط�
#define NRF_RCD_ARD_250us           0<<4    // bit7:4=0000=�Զ��ط���ʱΪ250us
#define NRF_RCD_ARD_500us           1<<4    // bit7:4=0001=�Զ��ط���ʱΪ500us
#define NRF_RCD_ARD_750us           2<<4    // bit7:4=0010=�Զ��ط���ʱΪ750us
#define NRF_RCD_ARD_1000us          3<<4    // bit7:4=0011=�Զ��ط���ʱΪ1000us
#define NRF_RCD_ARD_1250us          4<<4    // bit7:4=0100=�Զ��ط���ʱΪ1250us
#define NRF_RCD_ARD_1500us          5<<4    // bit7:4=0101=�Զ��ط���ʱΪ1500us
#define NRF_RCD_ARD_1750us          6<<4    // bit7:4=0110=�Զ��ط���ʱΪ1750us
#define NRF_RCD_ARD_2000us          7<<4    // bit7:4=0111=�Զ��ط���ʱΪ2000us
#define NRF_RCD_ARD_2250us          8<<4    // bit7:4=1000=�Զ��ط���ʱΪ2250us
#define NRF_RCD_ARD_2500us          9<<4    // bit7:4=1001=�Զ��ط���ʱΪ2500us
#define NRF_RCD_ARD_2750us          10<<4   // bit7:4=1010=�Զ��ط���ʱΪ2750us
#define NRF_RCD_ARD_3000us          11<<4   // bit7:4=1011=�Զ��ط���ʱΪ3000us
#define NRF_RCD_ARD_3250us          12<<4   // bit7:4=1100=�Զ��ط���ʱΪ3250us
#define NRF_RCD_ARD_3500us          13<<4   // bit7:4=1101=�Զ��ط���ʱΪ3500us
#define NRF_RCD_ARD_3750us          14<<4   // bit7:4=1110=�Զ��ط���ʱΪ3750us
#define NRF_RCD_ARD_4000us          15<<4   // bit7:4=1111=�Զ��ط���ʱΪ4000us
#define NRF_RCD_ARC_DI              0       // bit3:0=0000=�����Զ��ط�
#define NRF_RCD_ARC_1               1       // bit3:0=0001=�Զ��ط�1��
#define NRF_RCD_ARC_2               2       // bit3:0=0010=�Զ��ط�2��
#define NRF_RCD_ARC_3               3       // bit3:0=0011=�Զ��ط�3��
#define NRF_RCD_ARC_4               4       // bit3:0=0100=�Զ��ط�4��
#define NRF_RCD_ARC_5               5       // bit3:0=0101=�Զ��ط�5��
#define NRF_RCD_ARC_6               6       // bit3:0=0110=�Զ��ط�6��
#define NRF_RCD_ARC_7               7       // bit3:0=0111=�Զ��ط�7��
#define NRF_RCD_ARC_8               8       // bit3:0=1000=�Զ��ط�8��
#define NRF_RCD_ARC_9               9       // bit3:0=1001=�Զ��ط�9��
#define NRF_RCD_ARC_10              10      // bit3:0=1010=�Զ��ط�10��
#define NRF_RCD_ARC_11              11      // bit3:0=1011=�Զ��ط�11��
#define NRF_RCD_ARC_12              12      // bit3:0=1100=�Զ��ط�12��
#define NRF_RCD_ARC_13              13      // bit3:0=1101=�Զ��ط�13��
#define NRF_RCD_ARC_14              14      // bit3:0=1110=�Զ��ط�14��
#define NRF_RCD_ARC_15              15      // bit3:0=1111=�Զ��ط�15��

// 0x05 RF_CH   Ƶ������    (Freq=2400+RF_CH[MHz],2400~2525MHz)
#define NRF_RCD_RF_CH_max           125     // bit6:0   ����Ƶ�����ֵ
#define NRF_RCD_RF_CH_min           0       // bit6:0   ����Ƶ����Сֵ

// 0x06 RF_SETUP    RF����
#define NRF_RCD_CONT_WAVE_DI        0<<7    // bit7=CONT_WAVE=0=���������ز�����
#define NRF_RCD_CONT_WAVE_EN        1<<7    // bit7=CONT_WAVE=1=���������ز�����
#define NRF_RCD_RF_DR_250kbps       4<<3    // bit5&bit3=10=250kbps
#define NRF_RCD_RF_DR_1Mbps         0<<3    // bit5&bit3=00=1Mbps
#define NRF_RCD_RF_DR_2Mbps         1<<3    // bit5&bit3=01=2Mbps
#define NRF_RCD_PLL_LOCK_DI         0<<4    // bit4=PLL_LOCK=0=������PLL
#define NRF_RCD_PLL_LOCK_EN         1<<4    // bit4=PLL_LOCK=1=����PLL
#define NRF_RCD_RF_PWR_0dBm         3<<1    // bit2:1=RF_PWR=11=0dBm
#define NRF_RCD_RF_PWR_n6dBm        2<<1    // bit2:1=RF_PWR=10=-6dBm
#define NRF_RCD_RF_PWR_n12dBm       1<<1    // bit2:1=RF_PWR=01=-12dBm
#define NRF_RCD_RF_PWR_n18dBm       0<<1    // bit2:1=RF_PWR=00=-18dBm

// 0x07 STATUS  ״̬
#define NRF_RCD_RX_DR               1<<6    // bit6=1=RX_DR=���ݽ�������ж�(д1���)
#define NRF_RCD_TX_DR               1<<5    // bit5=1=TX_DR=���ݷ�������ж�(д1���)
#define NRF_RCD_MAX_RT              1<<4    // bit4=1=MAX_RT=�ط������ж�(д1���)
#define NRF_RCD_RX_PIPE0_DR         0<<1    // bit3:1=000=���յ������ݰ�Ϊͨ��0
#define NRF_RCD_RX_PIPE1_DR         1<<1    // bit3:1=001=���յ������ݰ�Ϊͨ��1
#define NRF_RCD_RX_PIPE2_DR         2<<1    // bit3:1=010=���յ������ݰ�Ϊͨ��2
#define NRF_RCD_RX_PIPE3_DR         3<<1    // bit3:1=011=���յ������ݰ�Ϊͨ��3
#define NRF_RCD_RX_PIPE4_DR         4<<1    // bit3:1=100=���յ������ݰ�Ϊͨ��4
#define NRF_RCD_RX_PIPE5_DR         5<<1    // bit3:1=101=���յ������ݰ�Ϊͨ��5
#define NRF_RCD_RX_FIFO_EMPTY       7<<1    // bit3:1=111=RX FIFOΪ��
#define NRF_RCD_TX_FIFO_FULL        1       // bit0=1=TX FIFO����

// 0x08 OBSERVE_TX  ���ͼ��
#define NRF_RCD_PLOS_CNT_MAX        15      // bit7:4   �������������ֵ    
#define NRF_RCD_PLOS_CNT_MIN        0       // bit7:4   ������������Сֵ
#define NRF_RCD_ARC_CNT_MAX         15      // bit3:0   �ط����������ֵ    
#define NRF_RCD_ARC_CNT_MIN         0       // bit3:0   �ط���������Сֵ

// 0x09 RPD ���չ��ʼ��
#define NRF_RCD_RPD_n64dBm_UP       1       // bit0=1=���չ���>=-64dBm
#define NRF_RCD_RPD_n64dbm_DOWN     0       // bit0=0=���չ���<-64dBm

// 0x11 RX_PW_PX    ����ͨ��X���ݿ��
#define NRF_RCD_RX_PW_PX_MAX        32      // bit5:0    ͨ��X���ݿ�����ֵ
#define NRF_RCD_RX_PW_PX_MIN        1       // bit5:0    ͨ��X���ݿ����Сֵ
#define NRF_RCD_RX_PW_PX_DI         0       // bit5:0    ͨ��Xδʹ��

// 0x17 FIFO_STATUS FIFO״̬
#define NRF_RCD_TX_REUSE            1<<6    // bit6=1=�豸�����ط�ģʽ
#define NRF_RCD_TX_FIFO_S_FULL      1<<5    // bit5=1=TX FIFO����
#define NRF_RCD_TX_FIFO_S_EMPTY     1<<4    // bit4=1=TX FIFO������
#define NRF_RCD_RX_FIFO_S_FULL      1<<1    // bit1=1=RX FIFO����
#define NRF_RCD_RX_FIFO_S_EMPTY     1       // bit0=1=RX FIFO������

// 0x1C Enable dynamic payload length   ʹ�ܶ�̬���ݰ�����
#define NRF_RCD_PIPE5_DPL_EN        1<<5    // bit5=1=����ͨ��5�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE5_DPL_DI        0<<5    // bit5=0=�ر�ͨ��5�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE4_DPL_EN        1<<4    // bit4=1=����ͨ��4�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE4_DPL_DI        0<<4    // bit4=0=�ر�ͨ��4�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE3_DPL_EN        1<<3    // bit3=1=����ͨ��3�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE3_DPL_DI        0<<3    // bit3=0=�ر�ͨ��3�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE2_DPL_EN        1<<2    // bit2=1=����ͨ��2�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE2_DPL_DI        0<<2    // bit2=0=�ر�ͨ��2�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE1_DPL_EN        1<<1    // bit1=1=����ͨ��1�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE1_DPL_DI        0<<1    // bit1=0=�ر�ͨ��1�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE0_DPL_EN        1       // bit0=1=����ͨ��0�Ķ�̬���ݰ�����
#define NRF_RCD_PIPE0_DPL_DI        0       // bit0=0=�ر�ͨ��0�Ķ�̬���ݰ�����

// 0x1D Feature Register    �����Ĵ���
#define NRF_RCD_DPL_EN              1<<2    // bit2=1=����̬���ݰ�����
#define NRF_RCD_DPL_DI              0<<2    // bit2=0=��ֹ��̬���ݰ�����
#define NRF_RCD_ACK_PAY_EN          1<<1    // bit1=1=�����ACK������
#define NRF_RCD_ACK_PAY_DI          0<<1    // bit1=0=��ֹ��ACK������
#define NRF_RCD_DYN_ACK_PAY_EN      1       // bit1=1=������ACK������
#define NRF_RCD_DYN_ACK_PAY_DI      0       // bit1=0=��ֹ��ACK������





#define NRF_RF_CH       0                   // RF��Ƶͨ�� 0~125
#define	NRF_ADDR_WIDTH  5                   // RF��ַ��� 1~5byte
#define NRF_DATA_WIDTH  32                  // RF���ݿ�� 1~32byte


//-------�궨������--------
//ʹ�õ���ģ��SPI�����ſ����������������
#define NRF_CE_PIN          B17
#define NRF_CSN_PIN         B21
#define NRF_MOSI_PIN        B29
#define NRF_SCK_PIN         B22
                            
#define NRF_MISO_PIN        B14
#define NRF_IRQ_PIN         B15



// �û����� 
extern uint8 nrf_rx_buff[];                 // �������ݻ�����
extern uint8 nrf_tx_buff[];                 // �������ݻ�����


// �û�����
void  nrf_init(void);
void  nrf_send_packet(uint8*);
uint8 nrf_rece_packet(uint8*);


#endif

