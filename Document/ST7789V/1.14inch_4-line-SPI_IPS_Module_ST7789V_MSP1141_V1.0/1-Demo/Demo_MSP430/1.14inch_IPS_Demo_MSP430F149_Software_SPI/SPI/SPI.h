//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��MSP430F149,����ԭ��MSP430������,��Ƶ8MHZ������8MHZ
//QDtech-TFTҺ������ for MSP430 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/08/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//     LCDģ��                STM32��Ƭ��
//      VCC          ��          3.3V         //��Դ
//      GND          ��          GND          //��Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ4����SPI����
//     LCDģ��                STM32��Ƭ��    
//       SDA         ��          P31         //Һ����SPI��������д�ź�
//=======================================Һ���������߽���==========================================//
//     LCDģ�� 					      STM32��Ƭ�� 
//       BLK         ��          P20         //Һ������������źţ��������Ҫ���ƣ���3.3V
//       SCL         ��          P33         //Һ����SPI����ʱ���ź�
//       DC          ��          P21         //Һ��������/��������ź�
//       RES         ��          P22         //Һ������λ�����ź�
//       CS          ��          P23         //Һ����Ƭѡ�����ź�
//=========================================������������=========================================//
//���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
//	   LCDģ��                STM32��Ƭ�� 
//      T_IRQ        ��          PC10         //�����������ж��ź�
//      T_DO         ��          PC2          //������SPI���߶��ź�
//      T_DIN        ��          PC3          //������SPI����д�ź�
//      T_CS         ��          PC13         //������Ƭѡ�����ź�
//      T_CLK        ��          PC0          //������SPI����ʱ���ź�
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/
#include  <msp430x14x.h>
#include "type.h"

#ifndef _SPI_H_
#define _SPI_H_

//�����Գ���ʹ�õ������SPI�ӿ�����
//SPIʱ���ź��Լ�SPI����д�ź����Ŷ����Ը���

#define SPI_SCLK BIT3    //P33
#define SPI_MOSI BIT1    //P31

//Һ�����ƿ���1�������궨��

#define	SPI_MOSI_SET  	P3OUT |= SPI_MOSI    
#define	SPI_SCLK_SET  	P3OUT |= SPI_SCLK     


//Һ�����ƿ���0�������궨��

#define	SPI_MOSI_CLR  	P3OUT &= ~SPI_MOSI    
#define	SPI_SCLK_CLR  	P3OUT &= ~SPI_SCLK
 
void SPI_WriteByte(u8 Byte);
void SPI_Init(void);

#endif
