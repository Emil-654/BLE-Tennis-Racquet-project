#include "esp_err.h"


#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B

 typedef enum {
  GYRO_250_DEG,  // +/- 250 deg/s 
  GYRO_500_DEG,  // +/- 500 deg/s
  GYRO_1000_DEG, // +/- 1000 deg/s
  GYRO_2000_DEG, // +/- 2000 deg/s
} gyro_range_t;

typedef enum {
  ACCEL_2_G,  // +/- 2g 
  ACCEL_4_G,  // +/- 4g
  ACCEL_8_G,  // +/- 8g
  ACCEL_16_G, // +/- 16g
} accel_range_t;

typedef struct Gyro_Data{
    int16_t gx;
    int16_t gy;
    int16_t gz;
  
} Gyro_Data;

typedef struct Acc_Data{
    int16_t ax;
    int16_t ay;
    int16_t az;
    
} Acc_Data;


extern gyro_range_t g_Gyro_Range;
extern accel_range_t g_Accel_Range;

extern int16_t g_XOffset;
extern int16_t g_YOffset;
extern int16_t g_ZOffset;

esp_err_t i2cinit(void);

void MPU6050init(gyro_range_t grange, accel_range_t arange);

void writeMPU6050_8(uint8_t addr, uint8_t conf);

void readMPU6050_8(uint8_t addr,uint8_t *dataa);

void setGyroRange(gyro_range_t range);

void setAccelRange(accel_range_t range);

void burstReadAccel(Acc_Data *dataA);

void burstReadGyro(Gyro_Data *dataG);