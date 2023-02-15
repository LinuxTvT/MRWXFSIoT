#ifndef MSQRCODE_H
#define MSQRCODE_H

void (*SetMSide)(int int_Side);			//设置条码单位长度,取值范围1-2,默认2
void (*SetLMargin)(int int_Margin);		//设置左边距,取值范围0-27,默认0
void (*SetBaudRate)(int int_BaudRate);	//设置串口波特率
void (*SetComPort)(int int_ComPort);	//设置串口
int (*PrintQRCode)(char* cData);		//打印二维码

#endif


