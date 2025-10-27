/* rtos_task/Mytask.h */

#ifndef MYTASK_H_
#define MYTASK_H_

#include "bsp.h" // 包含 FreeRTOS, bsp, 和 HAL 的主要头文件

/* 包含所有需要的驱动和模块头文件 */
#include "oled.h"
#include "buletooth.h"
#include "icm42688_driver.h"
#include "wit.h"
#include "ui.h"
#include "key.h"
#include "bus.h"

/*
 * ===================================================================
 * 全局任务句柄
 * ===================================================================
 * 声明为 extern，以便 main.c (ISR) 可以访问它们
 */
extern TaskHandle_t g_ble_task_handle;
extern TaskHandle_t g_wit_task_handle;


/*
 * ===================================================================
 * 任务参数结构体
 * ===================================================================
 * 任务包装器使用这些结构体将设备句柄传递给任务
 */

typedef struct {
    BleHandle_t dev_ble;
} BleTaskParam_t;

typedef struct {
    WitHandle dev_wit;
} WitTaskParam_t;

typedef struct {
    IcmHandle_t dev_icm;
} IcmTaskParam_t;

typedef struct {
    KeyHandle_t dev_key;
} KeyScanTaskParam_t;

/*
 * ===================================================================
 * 任务创建函数
 * ===================================================================
 * 在 main.c 或 freertos.c 中调用这些函数来启动所有任务
 */

/**
 * @brief 创建蓝牙接收任务 (UART3, DMA, TaskNotify)
 */
void BleReciveTask(void);

/**
 * @brief 创建WIT姿态传感器任务 (UART1, DMA, TaskNotify)
 */
void WitReciveTask(void);

/**
 * @brief 创建ICM-42688 6轴传感器任务 (SPI, vTaskDelay)
 */
void IcmReciveTask(void);

/**
 * @brief 创建按键扫描任务 (GPIO, vTaskDelay)
 */
void KeyScanTask(void);

/**
 * @brief 创建UI刷新任务 (EventGroup)
 */
void UiUpdateTaskCreate(void);

#endif /* MYTASK_H_ */
