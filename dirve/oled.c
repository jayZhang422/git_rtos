#include "oled.h"
#include "oledfont.h"
#include "bus.h"
struct OledDevice{

    uint8_t m_ucaOledRam[8][128];/*显存*/

    uint8_t is_ready;/*初始化完成标志*/

   SemaphoreHandle_t OLED_mutex;/*互斥量*/

   BusHandle_t bushandle;


};


static struct OledDevice g_my_oled_device;

OledHandle_t Oled_getInstance(void){

    return &g_my_oled_device;


}
//外部无需访问，不需要在.h文件中声明
void OLED_WriteByte(uint8_t dat,uint8_t cmd){
  if(cmd == OLED_CMD)
    {
        bsp_oled_transmit_cmd(dat,1);  
    }
    else
    {
        bsp_oled_transmit_data(&dat,1);  
    }
}
void Oled_Init(OledHandle_t handle){

     handle->is_ready = 1;
     vTaskDelay(100);
     #ifdef  SSD1315
	
	OLED_WriteByte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WriteByte(0x00,OLED_CMD);//---set low column address
	OLED_WriteByte(0x10,OLED_CMD);//---set high column address
	OLED_WriteByte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteByte(0x81,OLED_CMD);//--set contrast control register
	OLED_WriteByte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WriteByte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WriteByte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WriteByte(0xA6,OLED_CMD);//--set normal display
	OLED_WriteByte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WriteByte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WriteByte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteByte(0x00,OLED_CMD);//-not offset
	OLED_WriteByte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WriteByte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WriteByte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WriteByte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteByte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WriteByte(0x12,OLED_CMD);
	OLED_WriteByte(0xDB,OLED_CMD);//--set vcomh
	OLED_WriteByte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WriteByte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteByte(0x00,OLED_CMD);//
	OLED_WriteByte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WriteByte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WriteByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WriteByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
	OLED_WriteByte(0xAF,OLED_CMD);
	
#else 

    OLED_WriteByte(0xAE,OLED_CMD);//--display off
    OLED_WriteByte(0x00,OLED_CMD);//---set low column address
    OLED_WriteByte(0x10,OLED_CMD);//---set high column address
    OLED_WriteByte(0x40,OLED_CMD);//--set start line address
    OLED_WriteByte(0xB0,OLED_CMD);//--set page address
    OLED_WriteByte(0x81,OLED_CMD); // contract control
    OLED_WriteByte(0xFF,OLED_CMD);//--128
    OLED_WriteByte(0xA1,OLED_CMD);//set segment remap
    OLED_WriteByte(0xA6,OLED_CMD);//--normal / reverse
    OLED_WriteByte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WriteByte(0x3F,OLED_CMD);//--1/32 duty
    OLED_WriteByte(0xC8,OLED_CMD);//Com scan direction
    OLED_WriteByte(0xD3,OLED_CMD);//-set display offset
    OLED_WriteByte(0x00,OLED_CMD);//

    OLED_WriteByte(0xD5,OLED_CMD);//set osc division
    OLED_WriteByte(0x80,OLED_CMD);//

    OLED_WriteByte(0xD8,OLED_CMD);//set area color mode off
    OLED_WriteByte(0x05,OLED_CMD);//

    OLED_WriteByte(0xD9,OLED_CMD);//Set Pre-Charge Period
    OLED_WriteByte(0xF1,OLED_CMD);//

    OLED_WriteByte(0xDA,OLED_CMD);//set com pin configuartion
    OLED_WriteByte(0x12,OLED_CMD);//

    OLED_WriteByte(0xDB,OLED_CMD);//set Vcomh
    OLED_WriteByte(0x30,OLED_CMD);//

    OLED_WriteByte(0x8D,OLED_CMD);//set charge pump enable
    OLED_WriteByte(0x14,OLED_CMD);//

    OLED_WriteByte(0x20,OLED_CMD);    // Set Memory Addressing Mode (20h)
    OLED_WriteByte(0x00,OLED_CMD);    // Set 页地址模式(A[1:0]=10b) 水平地址模式(A[1:0]=00b) 垂直地址模式(A[1:0]=01b)

    OLED_WriteByte(0xAF,OLED_CMD);//--turn on oled panel

#endif

}
void OLED_Fill(OledHandle_t handle,uint8_t ucData){

    if(handle == NULL||!handle->is_ready){
        return;

    }
    uint8_t *puc;
    uint16_t i;
    puc = &handle->m_ucaOledRam[0][0];

    for(i=0; i<1024; i++)
    {
        *puc++ = ucData;
    }
}

void OLED_ShowChar(OledHandle_t handle,uint8_t x,uint8_t y,uint8_t chr,uint8_t size1){

    if(handle == NULL||!handle->is_ready){
        return;

    }

   
    uint8_t i,m,temp,size2,chr1;
    uint8_t y0=y;
    size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
    chr1=chr-' ';  //计算偏移后的值
    for(i=0; i<size2; i++)
    {
        // temp=asc2_1608[chr1][i];//调用1608字体
		if(size1==12)
       {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
       {temp=asc2_1608[chr1][i];} //调用1608字体
//		else if(size1==24)
//        {temp=asc2_2412[chr1][i];} //调用2412字体
//		else return;
        for(m=0; m<8; m++)         //写入数据
        {
            if(temp&0x80)OLED_DrawPoint(handle,x,y);
            else OLED_ClearPoint(handle,x,y);
            temp<<=1;
            y++;
            if((y-y0)==size1)
            {
                y=y0;
                x++;
                break;
            }
        }
    }

}
void OLED_ClearPoint(OledHandle_t handle,uint8_t x,uint8_t y)
{
     if(handle == NULL||!handle->is_ready){
        return;

    }
    uint8_t ucTmp;
    uint8_t ucY,ucZ;
    if(x>127 || y>63)
    {
        return;
    }
    else
    {
        ucY = y / 8;
        ucZ = y % 8;
        ucTmp =handle-> m_ucaOledRam[ucY][x];
        ucTmp &= ~(0x01 << ucZ);
        handle-> m_ucaOledRam[ucY][x] = ucTmp;
    }
}

void OLED_ShowString(OledHandle_t handle,uint8_t x,uint8_t y,char *chr,uint8_t size1){

      if(handle == NULL||!handle->is_ready){
        return;

    }
      while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
    {
        OLED_ShowChar(handle,x,y,*chr,size1);
        x+=size1/2;
        if(x>128-size1)  //换行
        {
            x=0;
            y+=2;
        }
        chr++;
    }
}
void OLED_Refresh(OledHandle_t handle){
     if(handle == NULL||!handle->is_ready){
        return;

    }
    uint8_t* buffer_start_address = &handle->m_ucaOledRam[0][0];
    bsp_oled_transmit_data( buffer_start_address,1024);  
    vTaskDelay(10);


}

void OLED_DrawPoint(OledHandle_t handle,uint8_t x,uint8_t y){
{
    if(handle == NULL||!handle->is_ready){
        return;

    }
    uint8_t ucTmp;
    uint8_t ucY,ucZ;
    if(x>127 || y>63)
    {
        return;
    }
    else
    {
        ucY = y / 8;
        ucZ = y % 8;
        ucTmp = handle->m_ucaOledRam[ucY][x];
        ucTmp |= (0x01 << ucZ);
         handle->m_ucaOledRam[ucY][x] = ucTmp;
    }
}

}
void OLED_ColorTurn(OledHandle_t handle,uint8_t i){

    if(handle == NULL||!handle->is_ready){
        return;
    }

    if(i==0)
    {
        OLED_WriteByte(0xA6,OLED_CMD);//正常显示
    }
    if(i==1)
    {
        OLED_WriteByte(0xA7,OLED_CMD);//反色显示
    }



}
void OLED_ReverseArea(OledHandle_t handle,int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
     if(handle == NULL||!handle->is_ready){
        return;
    }
	int16_t i, j;
	
	for (j = Y; j < Y + Height; j ++)		//遍历指定页
	{
		for (i = X; i < X + Width; i ++)	//遍历指定列
		{
			if (i >= 0 && i <= 127 && j >=0 && j <= 63)			//超出屏幕的内容不显示
			{
				handle->m_ucaOledRam[j / 8][i] ^= 0x01 << (j % 8);	//将显存数组指定数据取反
			}
		}
	}
}
void OLED_Draw_Picture(OledHandle_t handle,uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t mode, unsigned const char p[])
{
      if(handle == NULL||!handle->is_ready){
        return;
    }
 char temp, t;
 unsigned int j=0;
 unsigned char x,y;
  
  if((y1-y0)%8!=0) y1 =(y1-y0)/8 + 1; 
	else y1 = y1 / 8;
	for(y=0;y<y1;y++)
	{
    for(x=x0;x<x1;x++)
	    {      
	    	temp = p[j++]; 
				for(t=0;t<8;t++)
				{
					if(temp&0x01)OLED_DrawPoint(handle,x,y*8+y0+t);
					else OLED_DrawPoint(handle,x,y*8+y0+t);
					temp>>=1;					
				}
	    }
	}

}
