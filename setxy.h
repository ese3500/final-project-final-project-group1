LCD_Write_COM(0x2a);
LCD_Write_DATA_vl(x1>>8);
LCD_Write_DATA_vl(x1);
LCD_Write_DATA_vl(x2>>8);
LCD_Write_DATA_vl(x2);
LCD_Write_COM(0x2b);
LCD_Write_DATA_vl(y1>>8);
LCD_Write_DATA_vl(y1);
LCD_Write_DATA_vl(y2>>8);
LCD_Write_DATA_vl(y2);
LCD_Write_COM(0x2c);
