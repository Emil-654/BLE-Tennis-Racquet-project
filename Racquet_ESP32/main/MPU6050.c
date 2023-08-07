/*
*  ESP32 Driver for MPU-6050 sensor
*
*
*/


#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "MPU6050.h"

gyro_range_t g_Gyro_Range;
accel_range_t g_Accel_Range;

int16_t g_XOffset = -137;
int16_t g_YOffset = -125;
int16_t g_ZOffset = -126;
int16_t a_ZOffset = -2458;

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


void MPU6050init(gyro_range_t grange, accel_range_t arange){
  writeMPU6050_8(SMPLRT_DIV, 0);
  writeMPU6050_8(CONFIG,0);
  setGyroRange(grange);
  setAccelRange(arange);
  writeMPU6050_8(PWR_MGMT_1,0x01);
  vTaskDelay(100/ portTICK_PERIOD_MS);
}



void writeMPU6050_8(uint8_t addr, uint8_t conf)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, 0xD0, 1);
  i2c_master_write_byte(cmd, addr, 1);
  i2c_master_write_byte(cmd, conf, 1);
  i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
}

void setGyroRange(gyro_range_t range){
    uint8_t rango = 0;
    rango |= range;
    rango <<= 3;

    writeMPU6050_8(GYRO_CONFIG, rango);
    g_Gyro_Range = range;
}

void setAccelRange(accel_range_t range){
    uint8_t rango = 0;
    rango |= range;
    rango <<= 3;

    writeMPU6050_8(ACCEL_CONFIG, rango);
    g_Accel_Range = range;
}


void readMPU6050_8(uint8_t addr,uint8_t *dataa){
  
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0xD0,1);
    i2c_master_write_byte(cmd, addr,1);
    i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);


  cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0xD1,1);
    i2c_master_read_byte(cmd,dataa,1);
     i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);
}


void burstReadAccel(Acc_Data *dataA){
  uint8_t bufferG[6];
  int16_t rawAccX, rawAccY, rawAccZ;

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0xD0,1);
    i2c_master_write_byte(cmd, 0x3B,1);
    i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);

  cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0xD1,1);
    i2c_master_read(cmd,bufferG,5,0);
    i2c_master_read_byte(cmd, &bufferG[5],1);
    i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);


  rawAccX = bufferG[0] << 8 | bufferG[1];
  rawAccY = bufferG[2] << 8 | bufferG[3];
  rawAccZ = bufferG[4] << 8 | bufferG[5];

  switch (g_Accel_Range)
  {
  case ACCEL_2_G:
    rawAccZ -= (2*a_ZOffset);
    break;
  case ACCEL_4_G:
    rawAccZ -= (a_ZOffset);
    break;
  case ACCEL_8_G:
    rawAccZ -= (a_ZOffset/2);
    break;
  case ACCEL_16_G:
    rawAccZ -= (a_ZOffset/4);
    break;
  } 

  dataA->ax = rawAccX;
  dataA->ay = rawAccY;
  dataA->az = rawAccZ;

}

void burstReadGyro(Gyro_Data *dataG){

  uint8_t bufferG[6];
  int16_t rawGyroX, rawGyroY, rawGyroZ;

  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0xD0,1);
    i2c_master_write_byte(cmd, 0x43,1);
    i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);

  cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0xD1,1);
    i2c_master_read(cmd,bufferG,5,0);
    i2c_master_read_byte(cmd, &bufferG[5],1);
    i2c_master_stop(cmd);
  i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
  i2c_cmd_link_delete(cmd);


  rawGyroX = bufferG[0] << 8 | bufferG[1];
  rawGyroY = bufferG[2] << 8 | bufferG[3];
  rawGyroZ = bufferG[4] << 8 | bufferG[5];

  switch (g_Gyro_Range)
  {
  case GYRO_250_DEG:
    rawGyroX -= (2*g_XOffset);
    rawGyroY -= (2*g_YOffset);
    rawGyroZ -= (2*g_ZOffset);
    break;
  case GYRO_500_DEG:
    rawGyroX -= g_XOffset;
    rawGyroY -= g_YOffset;
    rawGyroZ -= g_ZOffset;
    break;
  case GYRO_1000_DEG:
    rawGyroX -= (g_XOffset/2);
    rawGyroY -= (g_YOffset/2);
    rawGyroZ -= (g_ZOffset/2);
    break;
  case GYRO_2000_DEG:
    rawGyroX -= (g_XOffset/4);
    rawGyroY -= (g_YOffset/4);
    rawGyroZ -= (g_ZOffset/4);
    break;
  } 


  dataG->gx = rawGyroX;
  dataG->gy = rawGyroY;
  dataG->gz = rawGyroZ;

}