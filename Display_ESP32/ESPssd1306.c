/*
*  ESP32 Driver for SSD1306 
*
*
*/

#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "ESPssd1306.h"

esp_err_t i2cinit(void){
      i2c_config_t conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = 21,         
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_io_num = 22,         
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 400000,  
    
};

esp_err_t err = i2c_param_config(I2C_NUM_0, &conf);
if (err != ESP_OK) {
        return err;
    }

return i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

void led_cmd(uint8_t comd){

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, 0x78,1);
  i2c_master_write_byte(cmd, 0x00,1);
  i2c_master_write_byte(cmd, comd,1);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);

}


void ssd1306_init(void){
    led_cmd(0xAE); //turn display off
    led_cmd(0x2E); //stop scrolling
    led_cmd(0xD5); //display update clock frequency
    led_cmd(0x80); //Oscillator frequency
    led_cmd(0xA8); //set multiplex ratio
    led_cmd(0x3F); //64-1
    led_cmd(0xD3); //set display offset
    led_cmd(0x00); //default offset
    led_cmd(0x40); //set start line default-0
    led_cmd(0x8D); //charge pump
    led_cmd(0x14); //enable internal charge pump
    led_cmd(0xA1); // column address 127 is mapped to SEG0 
    led_cmd(0xC8); //remapped mode Scan from COM[63] to COM0
    led_cmd(0xDA); //set COM pins
    led_cmd(0x12); //COM configuration
    led_cmd(0x81); //set contrast control
    led_cmd(0xCF); //contrast
    led_cmd(0xD9); //set pre-charge period
    led_cmd(0xF1); //pre-charge settings
    led_cmd(0xDB); //set vcom deselect level
    led_cmd(0x40); //set deselect level
    led_cmd(0xA4); //resume to ram content
    led_cmd(0xA6); //normal display 
    led_cmd(0xAF); //display on

}


void clearEntireDisplay(void){
   led_cmd(0xAE);
   led_cmd(0x20);
   led_cmd(0x00);
   led_cmd(0xA4);
   led_cmd(0xAF);
   led_cmd(0x22);
   led_cmd(0);
   led_cmd(7);
   led_cmd(0x21);
   led_cmd(0);
   led_cmd(127);

   i2c_cmd_handle_t cmd = i2c_cmd_link_create();
   i2c_master_start(cmd);
   i2c_master_write_byte(cmd, 0x78,1);
   

   for(int i = 0; i < 1023; i++){
       i2c_master_write_byte(cmd, 0xC0,1);
       i2c_master_write_byte(cmd, 0x00,1);
   }
   i2c_master_write_byte(cmd, 0x40,1);
   i2c_master_write_byte(cmd, 0x00,1);
   i2c_master_stop(cmd);
   i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
   i2c_cmd_link_delete(cmd);
}


void set_Cursor(int x, int y) { // x ranges: 0-127, y ranges: 0-7 Horizontal mode
    led_cmd(0x20);
    led_cmd(0x00);
    led_cmd(0x21);
    led_cmd(x);
    led_cmd(127);
    led_cmd(0x22);
    led_cmd(y);
    led_cmd(7);

}

void led_data(uint8_t data){
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, 0x78,1);
  i2c_master_write_byte(cmd, 0x40,1);
  i2c_master_write_byte(cmd, data,1);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
}


// void draw_Zero(int xPos, int yPos){

//   if(yPos > 5){
//     yPos = 5;
//   }

//   set_Cursor(xPos,yPos);
//   led_data(0xFC);
//   led_data(0xFE);
//   led_data(0x07);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x07);
//   led_data(0xFE);
//   led_data(0xFC);
//   set_Cursor(xPos,yPos+1);

//   led_data(0x7F);
//   led_data(0xFF);
//   led_data(0xC0);
//   led_data(0x80);
//   led_data(0x80);
//   led_data(0x80);
//   led_data(0x80);
//   led_data(0x80);
//   led_data(0xC0);
//   led_data(0xFF);
//   led_data(0x7F);
//   set_Cursor(xPos,yPos+2);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);


// }


// void draw_One(int xPos, int yPos){
//     set_Cursor(xPos,yPos);
//     led_data(0x00);
//     led_data(0x00);
//     led_data(0x00);
//     led_data(0x0C);
//     led_data(0x0E);
//     led_data(0xFF);
//     led_data(0xFF);
//     led_data(0x00);
//     led_data(0x00);
//     led_data(0x00);
//     led_data(0x00);
//     set_Cursor(xPos,yPos+1);
//     led_data(0x00);
//     led_data(0x00);
//     led_data(0x80);
//     led_data(0x80);
//     led_data(0x80);
//     led_data(0xFF);
//     led_data(0xFF);
//     led_data(0x80);
//     led_data(0x80);
//     led_data(0x80);
//     led_data(0x00);
//     set_Cursor(xPos,yPos+2);
//     led_data(0x00);
//     led_data(0x00);
//     led_data(0x01);
//     led_data(0x01);
//     led_data(0x01);
//     led_data(0x01);
//     led_data(0x01);
//     led_data(0x01);
//     led_data(0x01);
//     led_data(0x01);
//     led_data(0x00);


// }

// void draw_Three(int xPos, int yPos){
//   set_Cursor(xPos,yPos);
//   led_data(0x0C);
//   led_data(0x0E);
//   led_data(0x07);
//   led_data(0x03);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0xC7);
//   led_data(0xFE);
//   led_data(0x7C);
//   set_Cursor(xPos,yPos +1);
//   led_data(0x60);
//   led_data(0xE0);
//   led_data(0xC0);
//   led_data(0x80);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0xC3);
//   led_data(0xFF);
//   led_data(0x7E);
//   set_Cursor(xPos,yPos+2);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x00);
//   led_data(0x00);



// }

// void draw_Two(int xPos, int yPos){
//   set_Cursor(xPos,yPos);
//   led_data(0x0C);
//   led_data(0x0E);
//   led_data(0x07);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x87);
//   led_data(0xFE);
//   led_data(0xFC);
//   set_Cursor(xPos,yPos +1);
//   led_data(0x80);
//   led_data(0xC0);
//   led_data(0xE0);
//   led_data(0xF0);
//   led_data(0xB8);
//   led_data(0x9C);
//   led_data(0x8E);
//   led_data(0x87);
//   led_data(0x83);
//   led_data(0x81);
//   led_data(0x80);
//   set_Cursor(xPos,yPos+2);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
// }

// void draw_Four(int xPos, int yPos){
//   set_Cursor(xPos,yPos);
//   led_data(0xFF);
//   led_data(0xFF);
//   led_data(0x80);
//   led_data(0x80);
//   led_data(0x80);
//   led_data(0x80);
//   led_data(0x80);
//   led_data(0x80);
//   led_data(0x80);
//   led_data(0xFF);
//   led_data(0xFF);
//   set_Cursor(xPos,yPos+1);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0xFF);
//   led_data(0xFF);
//   set_Cursor(xPos,yPos+2);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x01);
//   led_data(0x01);
// }

// void draw_Five(int xPos, int yPos){
//   set_Cursor(xPos,yPos);
//   led_data(0xFF);
//   led_data(0xFF);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   set_Cursor(xPos,yPos+1);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0xFF);
//   led_data(0xFF);
//   set_Cursor(xPos,yPos+2);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);


// }

// void draw_Six(int xPos, int yPos){
//   set_Cursor(xPos,yPos);
//   led_data(0xFC);
//   led_data(0xFE);
//   led_data(0x87);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x87);
//   led_data(0x0E);
//   led_data(0x0C);
//   set_Cursor(xPos,yPos+1);
//   led_data(0x7F);
//   led_data(0xFF);
//   led_data(0xC1);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0xC3);
//   led_data(0xFF);
//   led_data(0x7E);
//   set_Cursor(xPos,yPos+2);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x00);
//   led_data(0x00);

// }

// void draw_Seven(int xPos, int yPos){
//   set_Cursor(xPos,yPos);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0x03);
//   led_data(0xC3);
//   led_data(0xF3);
//   led_data(0x3F);
//   led_data(0x0F);
//   set_Cursor(xPos,yPos+1);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0xFC);
//   led_data(0xFF);
//   led_data(0x03);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   set_Cursor(xPos,yPos+2);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x00);

// }

// void draw_Eight(int xPos, int yPos){
//   set_Cursor(xPos,yPos);
//   led_data(0x3C);
//   led_data(0xFE);
//   led_data(0xC7);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0xC7);
//   led_data(0xFE);
//   led_data(0x3C);
//   set_Cursor(xPos,yPos+1);
//   led_data(0x7C);
//   led_data(0xFF);
//   led_data(0xC3);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0xC3);
//   led_data(0xFF);
//   led_data(0x7C);
//   set_Cursor(xPos,yPos+2);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x00);
//   led_data(0x00);
// }

// void draw_Nine(int xPos, int yPos){
//   set_Cursor(xPos,yPos);
//   led_data(0x7C);
//   led_data(0xFE);
//   led_data(0xC7);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0x83);
//   led_data(0xC7);
//   led_data(0xFE);
//   led_data(0xFC);
//   set_Cursor(xPos,yPos+1);
//   led_data(0x60);
//   led_data(0xE0);
//   led_data(0xC1);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0x81);
//   led_data(0xC1);
//   led_data(0xFF);
//   led_data(0x7F);
//   set_Cursor(xPos,yPos+2);
//   led_data(0x00);
//   led_data(0x00);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x01);
//   led_data(0x00);
//   led_data(0x00);

// }


// void draw_Num(int num, int xPos,int yPos){
//   switch (num)
//   {
//   case 0:
//     draw_Zero(xPos,yPos);
//     break;
//   case 1:
//     draw_One(xPos,yPos);
//     break;
//   case 2:
//     draw_Two(xPos,yPos);
//     break;
//   case 3:
//     draw_Three(xPos,yPos);
//     break;
//   case 4:
//     draw_Four(xPos, yPos);
//     break;
//   case 5:
//     draw_Five(xPos,yPos);
//     break;
//   case 6:
//     draw_Six(xPos,yPos);
//     break;
//   case 7:
//     draw_Seven(xPos,yPos);
//     break;
//   case 8:
//     draw_Eight(xPos,yPos);
//     break;
//   case 9:
//     draw_Nine(xPos,yPos);
//     break;
//   }
// }

void dra_five(int xPos, int yPos){
    set_Cursor(xPos,yPos);
    led_data(0xF8);
    led_data(0xFC);
    led_data(0x8C);
    led_data(0x8C);
    led_data(0x8C);
    led_data(0x8C);
    led_data(0x8C);
    led_data(0x8C);
    led_data(0x8C);
    led_data(0x8C);
    set_Cursor(xPos,yPos+1);
    led_data(0x31);
    led_data(0x31);
    led_data(0x31);
    led_data(0x31);
    led_data(0x31);
    led_data(0x31);
    led_data(0x31);
    led_data(0x31);
    led_data(0x3F);
    led_data(0x3F);

}

void dra_two(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x30);
  led_data(0x38);
  led_data(0x1C);
  led_data(0x0C);
  led_data(0x0C);
  led_data(0x8C);
  led_data(0xCC);
  led_data(0xDC);
  led_data(0x78);
  led_data(0x30);
  set_Cursor(xPos,yPos+1);
  led_data(0x30);
  led_data(0x38);
  led_data(0x3C);
  led_data(0x3E);
  led_data(0x37);
  led_data(0x33);
  led_data(0x31);
  led_data(0x30);
  led_data(0x30);
  led_data(0x30);
  
}

void dra_four(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0xFC);
  led_data(0xFC);
  led_data(0x80);
  led_data(0x80);
  led_data(0x80);
  led_data(0x80);
  led_data(0x80);
  led_data(0x80);
  led_data(0xFC);
  led_data(0xFC);
  set_Cursor(xPos,yPos+1);
  led_data(0x01);
  led_data(0x01);
  led_data(0x01);
  led_data(0x01);
  led_data(0x01);
  led_data(0x01);
  led_data(0x01);
  led_data(0x01);
  led_data(0x3F);
  led_data(0x3F);
}

void dra_six(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0xF0);
  led_data(0xF8);
  led_data(0x9C);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0x9C);
  led_data(0x38);
  led_data(0x30);
  set_Cursor(xPos,yPos+1);
  led_data(0x0F);
  led_data(0x1F);
  led_data(0x39);
  led_data(0x31);
  led_data(0x31);
  led_data(0x31);
  led_data(0x31);
  led_data(0x3B);
  led_data(0x1F);
  led_data(0x0E);
}

void dra_seven(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x0C);
  led_data(0x0C);
  led_data(0x0C);
  led_data(0x0C);
  led_data(0x0C);
  led_data(0x0C);
  led_data(0x0C);
  led_data(0xCC);
  led_data(0xFC);
  led_data(0x3C);
  set_Cursor(xPos,yPos+1);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x30);
  led_data(0x3C);
  led_data(0x0F);
  led_data(0x03);
  led_data(0x00);
  led_data(0x00);
}

void dra_zero(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0xF0);
  led_data(0xF8);
  led_data(0x1C);
  led_data(0x0C);
  led_data(0x0C);
  led_data(0x0C);
  led_data(0x0C);
  led_data(0x1C);
  led_data(0xF8);
  led_data(0xF0);
  set_Cursor(xPos,yPos+1);
  led_data(0x0F);
  led_data(0x1F);
  led_data(0x38);
  led_data(0x30);
  led_data(0x30);
  led_data(0x30);
  led_data(0x30);
  led_data(0x38);
  led_data(0x1F);
  led_data(0x0F);
}

void dra_one(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x00);
  led_data(0x00);
  led_data(0x30);
  led_data(0x38);
  led_data(0xFC);
  led_data(0xFC);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  set_Cursor(xPos,yPos+1);
  led_data(0x00);
  led_data(0x00);
  led_data(0x30);
  led_data(0x30);
  led_data(0x3F);
  led_data(0x3F);
  led_data(0x30);
  led_data(0x30);
  led_data(0x00);
  led_data(0x00);
}

void dra_three(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x30);
  led_data(0x38);
  led_data(0x1C);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0xDC);
  led_data(0xF8);
  led_data(0x70);
  set_Cursor(xPos,yPos+1);
  led_data(0x0C);
  led_data(0x1C);
  led_data(0x38);
  led_data(0x31);
  led_data(0x31);
  led_data(0x31);
  led_data(0x31);
  led_data(0x3B);
  led_data(0x1F);
  led_data(0x0E);
  

}

void dra_nine(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x70);
  led_data(0xF8);
  led_data(0xDC);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0xDC);
  led_data(0xF8);
  led_data(0xF0);
  set_Cursor(xPos,yPos+1);
  led_data(0x0C);
  led_data(0x1C);
  led_data(0x39);
  led_data(0x31);
  led_data(0x31);
  led_data(0x31);
  led_data(0x31);
  led_data(0x39);
  led_data(0x1F);
  led_data(0x0F);
  
}

void dra_eight(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x70);
  led_data(0xF8);
  led_data(0xDC);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0x8C);
  led_data(0xDC);
  led_data(0xF8);
  led_data(0x70);
  set_Cursor(xPos,yPos+1);
  led_data(0x0E);
  led_data(0x1F);
  led_data(0x3B);
  led_data(0x31);
  led_data(0x31);
  led_data(0x31);
  led_data(0x31);
  led_data(0x3B);
  led_data(0x1F);
  led_data(0x0E);
}

void draw_Numm(int num, int xPos,int yPos){
  switch (num)
  {
  case 0:
    dra_zero(xPos,yPos);
    break;
  case 1:
    dra_one(xPos,yPos);
    break;
  case 2:
    dra_two(xPos,yPos);
    break;
  case 3:
    dra_three(xPos,yPos);
    break;
  case 4:
    dra_four(xPos, yPos);
    break;
  case 5:
    dra_five(xPos,yPos);
    break;
  case 6:
    dra_six(xPos,yPos);
    break;
  case 7:
    dra_seven(xPos,yPos);
    break;
  case 8:
    dra_eight(xPos,yPos);
    break;
  case 9:
    dra_nine(xPos,yPos);
    break;
  case 10:
    dra_blank(xPos,yPos);
    break;
  }
}

void dra_blank(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  set_Cursor(xPos,yPos+1);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
}

void draw_Z(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x40);
  led_data(0x40);
  led_data(0x40);
  led_data(0x40);
  led_data(0x40);
  led_data(0xC0);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  set_Cursor(xPos,yPos+1);
  led_data(0x30);
  led_data(0x28);
  led_data(0x24);
  led_data(0x22);
  led_data(0x21);
  led_data(0x20);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
}

void draw_Y(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0xF8);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0xF8);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  set_Cursor(xPos,yPos+1);
  led_data(0x00);
  led_data(0x01);
  led_data(0x3E);
  led_data(0x3E);
  led_data(0x01);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
}

void draw_X(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x78);
  led_data(0x80);
  led_data(0x00);
  led_data(0x80);
  led_data(0x78);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  set_Cursor(xPos,yPos+1);
  led_data(0x3C);
  led_data(0x02);
  led_data(0x01);
  led_data(0x02);
  led_data(0x3C);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
}

void draw_Point(int xPos, int yPos){
  set_Cursor(xPos-2,yPos-1);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  set_Cursor(xPos-2,yPos);
  led_data(0x00);
  led_data(0x00);
  led_data(0x30);
  led_data(0x30);
  led_data(0x00);
  led_data(0x00);

}

void dra_OneDigitBlank(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x00);
  led_data(0x00);
  set_Cursor(xPos,yPos+1);
  led_data(0x00);
  led_data(0x00);
}

void dra_Negative(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x80);
  led_data(0x80);
  led_data(0x80);
  led_data(0x80);
  led_data(0x80);
  set_Cursor(xPos,yPos+1);
  led_data(0x01);
  led_data(0x01);
  led_data(0x01);
  led_data(0x01);
  led_data(0x01);
}

void erase_Negative(int xPos, int yPos){
  set_Cursor(xPos,yPos);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  set_Cursor(xPos,yPos+1);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
}

void draw_Title(void)
{
  set_Cursor(0,0);
  led_data(0x00);
  led_data(0xC0);
  led_data(0x30);
  led_data(0x2C);
  led_data(0x2C);
  led_data(0x30);
  led_data(0xC0);
  led_data(0x00);

  led_data(0x70);
  led_data(0x88);
  led_data(0x88);
  led_data(0x88);
  led_data(0x00);

  led_data(0x70);
  led_data(0x88);
  led_data(0x88);
  led_data(0x88);
  led_data(0x00);

  led_data(0x70);
  led_data(0xA8);
  led_data(0xA8);
  led_data(0xA8);
  led_data(0x30);
  led_data(0x00);

  led_data(0x04);
  led_data(0xFC);
  led_data(0x00);
  led_data(0x00);

  led_data(0x10);
  led_data(0x10);
  led_data(0x10);
  led_data(0x10);
  led_data(0x10);
  led_data(0x00);
  led_data(0x00);

  led_data(0xF8);
  led_data(0x08);
  led_data(0x10);
  led_data(0x10);
  led_data(0x08);
  led_data(0xF8);
  led_data(0x00);

  led_data(0x80);
  led_data(0x40);
  led_data(0x20);
  led_data(0x10);
  led_data(0x08);
  led_data(0x00);

  led_data(0x90); //S
  led_data(0xA8);
  led_data(0xA8);
  led_data(0x48);
  led_data(0x00);

  led_data(0x10);//Carrot
  led_data(0x08);
  led_data(0x10);
  led_data(0x00);

  led_data(0x90);
  led_data(0xC8);//
  led_data(0xA8);
  led_data(0x90);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);
  led_data(0x00);

  led_data(0x30);
  led_data(0x48);
  led_data(0x84);
  led_data(0xA4);
  led_data(0xA4);
  led_data(0x68);
  led_data(0x00);

  led_data(0x18);
  led_data(0xA0);
  led_data(0xA0);
  led_data(0x78);
  led_data(0x00);

  led_data(0xF0);
  led_data(0x20);
  led_data(0x10);
  led_data(0x10);
  led_data(0x20);
  led_data(0x00);

  led_data(0x60);
  led_data(0x90);
  led_data(0x90);
  led_data(0x60);
  led_data(0x00);
  led_data(0x00);

  led_data(0x10);
  led_data(0x10);
  led_data(0x10);
  led_data(0x10);
  led_data(0x10);
  led_data(0x00);
  led_data(0x00);

  led_data(0x60);
  led_data(0x90);
  led_data(0x90);
  led_data(0x90);
  led_data(0x50);
  led_data(0xFC);
  led_data(0x00);
  
  led_data(0xF8);
  led_data(0x50);
  led_data(0x48);
  led_data(0x48);
  led_data(0x48);
  led_data(0x30);
  led_data(0x00);

  led_data(0x90); //S
  led_data(0xA8);
  led_data(0xA8);
  led_data(0x48);
  led_data(0x00);

}
