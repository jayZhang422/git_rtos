#ifndef ICM42688_DRIVER_H
#define ICM42688_DRIVER_H

#include "bsp.h"
#include "bus.h"

#define WHO_AM_I_REG 0x75
#define PWR_MGMT0_REG 0x4E
#define GYRO_CONFIG0_REG 0x4F
#define ACCEL_CONFIG0_REG 0x50
#define FIFO_CONFIG_REG 0x16
#define FIFO_DATA_REG 0x34
#define TEMP_SENSITIVITY 132.48f  
#define TEMP_OFFSET      25.0f  
#define GYRO_FILTER_ALPHA 0.1f
#define M_PI 3.1415926535
struct IcmDevice;

typedef struct IcmDevice* IcmHandle_t;


void IcmInit(IcmHandle_t handle,BusHandle_t bus_handle);

void ReadSensorData(IcmHandle_t handle,int16_t* accel, int16_t* gyro);
void ConvertRawData(IcmHandle_t handle,int16_t raw_accel[3], int16_t raw_gyro[3], float* accel_g, float* gyro_dps);
float ICM42688_ReadTemperature(IcmHandle_t handle);
void LowPassFilterGyro(IcmHandle_t handle,float* gyro_dps);
void CalibrateGyroscope(IcmHandle_t handle);
void PreventChange(IcmHandle_t handle , float angle);
void UpdateAttitude(IcmHandle_t handle);
IcmHandle_t Icm_getInstance(void);
void IcmTask(IcmHandle_t handle);
#endif // ICM_42688_H
