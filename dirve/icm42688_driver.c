#include "icm42688_driver.h"
#include "MahonyAHRS.h"
#include "bus.h"
struct IcmDevice{
    FLOAT_ANGLE att_angle;
    FLOAT_XYZ accel_f;
    FLOAT_XYZ gyro_rad_f;
    float m_gyro_bias[3];
    int16_t m_raw_accel[3], m_raw_gyro[3];  
    float m_accel_g[3], m_gyro_dps[3];
    uint8_t is_ready;
    BusHandle_t bushandle;
};

static struct IcmDevice g_my_icm_device;

void ICM42688_WriteRegister(uint8_t reg, uint8_t data) {
   uint8_t tx_buffer[2] = {reg & 0x7F, data};
    bsp_cs_low();
    bsp_imu_transmit(tx_buffer);
    bsp_cs_pull();
}
uint8_t ICM42688_ReadRegister(uint8_t reg) {

    uint8_t tx_buffer[2] = {reg | 0x80, 0x00}; 
    uint8_t rx_buffer[2] = {0};

    bsp_cs_low();
    bsp_imu_RT (tx_buffer, rx_buffer);
    bsp_cs_pull();
    return rx_buffer[1];

}
IcmHandle_t Icm_getInstance(void){

    return &g_my_icm_device;

}
void IcmInit(IcmHandle_t handle,BusHandle_t bus_handle)
{
    handle->is_ready = 1;
    uint8_t whoami;
    uint8_t tx_data;
    handle->bushandle = bus_handle;
    
    ICM42688_WriteRegister(PWR_MGMT0_REG, 0x00);
    vTaskDelay(10);

   
    whoami = ICM42688_ReadRegister(WHO_AM_I_REG);
    if(whoami != 0x47) { 
        return;
    }

   
    tx_data = 0x0F; 
    ICM42688_WriteRegister(PWR_MGMT0_REG, tx_data);

   
    tx_data = (0x03 << 5) | 0x06; 
    ICM42688_WriteRegister(GYRO_CONFIG0_REG, tx_data);

    
    tx_data = (0x01 << 5) | 0x06; 
    ICM42688_WriteRegister(ACCEL_CONFIG0_REG, tx_data);

    
    tx_data = 0x03; 
    ICM42688_WriteRegister(FIFO_CONFIG_REG, tx_data);

    vTaskDelay(100); 
    CalibrateGyroscope(handle) ;

}

void ReadSensorData(IcmHandle_t handle,int16_t* accel, int16_t* gyro){
    if(handle == NULL||!handle->is_ready){
        return;
    }
    accel[0] = (int16_t)((ICM42688_ReadRegister(0x1F) << 8) | ICM42688_ReadRegister(0x20));
    accel[1] = (int16_t)((ICM42688_ReadRegister(0x21) << 8) | ICM42688_ReadRegister(0x22));
    accel[2] = (int16_t)((ICM42688_ReadRegister(0x23) << 8) | ICM42688_ReadRegister(0x24));

    
    gyro[0] = (int16_t)((ICM42688_ReadRegister(0x25) << 8) | ICM42688_ReadRegister(0x26));
    gyro[1] = (int16_t)((ICM42688_ReadRegister(0x27) << 8) | ICM42688_ReadRegister(0x28));
    gyro[2] = (int16_t)((ICM42688_ReadRegister(0x29) << 8) | ICM42688_ReadRegister(0x2A));



}

void ConvertRawData(IcmHandle_t handle,int16_t raw_accel[3], int16_t raw_gyro[3], float* accel_g, float* gyro_dps){

    if(handle == NULL||!handle->is_ready){
        return;
    }
       for(int i = 0; i < 3; i++) {
        accel_g[i] = (float)raw_accel[i] / 4096.0f; 
    }

    
    for(int i = 0; i < 3; i++) {
        gyro_dps[i] = (float)raw_gyro[i] / 65.5f; 
}




}
float ICM42688_ReadTemperature(IcmHandle_t handle){

    if(handle == NULL||!handle->is_ready){
        return 0.0f;
    }
     int16_t temp_raw = (int16_t)((ICM42688_ReadRegister(0x1D) << 8) | ICM42688_ReadRegister(0x1E));

 
    return ((float)temp_raw / TEMP_SENSITIVITY) + TEMP_OFFSET;

}
void LowPassFilterGyro(IcmHandle_t handle,float* gyro_dps){

    if(handle == NULL||!handle->is_ready){
        return;
    }
    static float prev_gyro_dps[3] = {0};  
    for (int i = 0; i < 3; i++) {
        gyro_dps[i] = GYRO_FILTER_ALPHA * gyro_dps[i] + (1.0f - GYRO_FILTER_ALPHA) * prev_gyro_dps[i];
        prev_gyro_dps[i] = gyro_dps[i];
}



}
void CalibrateGyroscope(IcmHandle_t handle){
    if(handle == NULL||!handle->is_ready){
        return;
    }
    int32_t gyro_offset_raw[3] = {0};
    int16_t raw_gyro_temp[3];
    int16_t dummy_raw_accel[3] = {0};
    float dummy_accel_g[3];

    for (int i = 0; i < 200; i++) {
        ReadSensorData(handle,NULL, raw_gyro_temp); 
        gyro_offset_raw[0] += raw_gyro_temp[0];
        gyro_offset_raw[1] += raw_gyro_temp[1];
        gyro_offset_raw[2] += raw_gyro_temp[2];
        vTaskDelay(5);
    }

    raw_gyro_temp[0] = gyro_offset_raw[0] / 200;
    raw_gyro_temp[1] = gyro_offset_raw[1] / 200;
    raw_gyro_temp[2] = gyro_offset_raw[2] / 200;

   
    float bias_dps[3];
    ConvertRawData(handle,dummy_raw_accel, raw_gyro_temp, dummy_accel_g, bias_dps);

    handle->m_gyro_bias[0] = bias_dps[0];
    handle->m_gyro_bias[1] = bias_dps[1];
    handle->m_gyro_bias[2] = bias_dps[2];
}
void PreventChange(IcmHandle_t handle , float angle){

     if(handle == NULL||!handle->is_ready){
        return;
    }
    if(angle > 180.0f) angle += 360.0f;
    if(angle < -180.0f) angle -= 360.0f;

}
void UpdateAttitude(IcmHandle_t handle){

     if(handle == NULL||!handle->is_ready){
        return;
    }
    ReadSensorData(handle,handle->m_raw_accel, handle->m_raw_gyro);
    
    ConvertRawData(handle,handle->m_raw_accel, handle->m_raw_gyro, handle->m_accel_g, handle->m_gyro_dps);

   
    handle->m_gyro_dps[0] -= handle->m_gyro_bias[0];
    handle->m_gyro_dps[1] -= handle->m_gyro_bias[1];
    handle->m_gyro_dps[2] -= handle->m_gyro_bias[2];

    
    LowPassFilterGyro(handle,handle ->m_gyro_dps);

  

    
    handle->gyro_rad_f.X = handle->m_gyro_dps[0] * (M_PI / 180.0f);
    handle->gyro_rad_f.Y = handle->m_gyro_dps[1] * (M_PI / 180.0f);
    handle->gyro_rad_f.Z = handle->m_gyro_dps[2] * (M_PI / 180.0f);

    
    handle->accel_f.X = handle->m_accel_g[0];
    handle->accel_f.Y = handle->m_accel_g[1];
    handle->accel_f.Z = handle->m_accel_g[2];

    
    IMUupdate(&handle->gyro_rad_f, &handle->accel_f, &handle->att_angle);

    PreventChange(handle,handle->att_angle.rol);
    PreventChange(handle,handle->att_angle.pit);   
    PreventChange(handle,handle->att_angle.yaw);
    
    vTaskDelay(10);



}
void IcmTask(IcmHandle_t handle){

  BusHandle_t bus = handle->bushandle;
  IcmInit(handle,bus);
  for(;;){
    UpdateAttitude(handle);
    Bus_publish(bus,TOPIC_ICM,&handle->att_angle);
    vTaskDelay(10);
  }

}
