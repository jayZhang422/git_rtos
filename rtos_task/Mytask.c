/* rtos_task/Mytask.c */

#include "Mytask.h"

/*
 * ===================================================================
 * 全局任务句柄定义
 * ===================================================================
 */
 TaskHandle_t g_ble_task_handle = NULL;
 TaskHandle_t g_wit_task_handle = NULL;


/*
 * ===================================================================
 * 蓝牙任务 (BLE)
 * ===================================================================
 */

/**
 * @brief BLE 任务的包装器
 * @note  这个函数被 xTaskCreate 调用。它负责获取总线句柄、
 * 初始化驱动，然后调用驱动的无限循环。
 */
static void BLE_Task_Wrapper(void *argument)
{
    BleTaskParam_t* params = (BleTaskParam_t*)argument;
    BleHandle_t handle = params->dev_ble;
    BusHandle_t bus = Bus_getInstance(); // 获取总线单例

    // 初始化蓝牙驱动，传入总线句柄
    BleInit(handle, bus);
    
    // 调用蓝牙驱动中的主循环 (该函数内包含 for(;;) 和 ulTaskNotifyTake)
    ble_task(handle);
}

/**
 * @brief 创建蓝牙任务
 */
void BleReciveTask(void)
{
    static BleTaskParam_t ble_task_params; // 使用 static 确保参数在任务启动后依然有效
    ble_task_params.dev_ble = Ble_getInstace();

    xTaskCreate(BLE_Task_Wrapper,     // 任务函数
                "Ble_task",           // 任务名
                256,                  // 堆栈大小 (words) - 256*4=1024字节，strtod需要较大堆栈
                &ble_task_params,     // 传递给任务的参数
                3,                    // 优先级
                &g_ble_task_handle);  // [!!关键!!] 保存任务句柄，供ISR使用
}


/*
 * ===================================================================
 * WIT 传感器任务
 * ===================================================================
 */

/**
 * @brief WIT 任务的包装器
 */
static void WIT_Task_Wrapper(void *argument)
{
    WitTaskParam_t* params = (WitTaskParam_t*)argument;
    WitHandle handle = params->dev_wit;
    BusHandle_t bus = Bus_getInstance();

    // 初始化WIT驱动，传入总线句柄
    WitInit(handle, bus);

    // 调用WIT驱动中的主循环 (该函数内包含 for(;;) 和 ulTaskNotifyTake)
    WitTask(handle);
}

/**
 * @brief 创建WIT任务
 */
void WitReciveTask(void)
{
    static WitTaskParam_t wit_task_params;
    wit_task_params.dev_wit = Wit_getInstace();

    xTaskCreate(WIT_Task_Wrapper,
                "Wit_task",
                256,                  // 堆栈大小 (words) - 浮点数解析需要较大堆栈
                &wit_task_params,
                3,
                &g_wit_task_handle);  // [!!关键!!] 保存任务句柄，供ISR使用
}


/*
 * ===================================================================
 * ICM-42688 传感器任务
 * ===================================================================
 */

/**
 * @brief ICM 任务的包装器
 */
static void ICM_Task_Wrapper(void *argument)
{
    IcmTaskParam_t* params = (IcmTaskParam_t*)argument;
    IcmHandle_t handle = params->dev_icm;
    BusHandle_t bus = Bus_getInstance();

    // 初始化ICM驱动，传入总线句柄
    IcmInit(handle, bus);
    
    // 调用ICM驱动中的主循环 (该函数内包含 for(;;) 和 vTaskDelay)
    IcmTask(handle);
}

/**
 * @brief 创建ICM任务
 */
void IcmReciveTask(void)
{
    static IcmTaskParam_t icm_task_params;
    icm_task_params.dev_icm = Icm_getInstance();

    xTaskCreate(ICM_Task_Wrapper,
                "Icm_task",
                256,                  // 堆栈大小 (words) - MahonyAHRS 解算需要较大堆栈
                &icm_task_params,
                3,
                NULL);                // 此任务不被ISR通知，无需保存句柄
}


/*
 * ===================================================================
 * 按键扫描任务
 * ===================================================================
 */

/**
 * @brief Key 任务的包装器
 */
static void Key_Task_Wrapper(void *argument)
{
    KeyScanTaskParam_t* params = (KeyScanTaskParam_t*)argument;
    KeyHandle_t handle = params->dev_key;
    BusHandle_t bus = Bus_getInstance();

    // KeyInit(handle, bus); // KeyTask 内部已经调用了 KeyInit
    
    // 调用Key驱动中的主循环
    KeyTask(handle);
}

/**
 * @brief 创建按键扫描任务
 */
void KeyScanTask(void)
{
    static KeyScanTaskParam_t key_scan_params;
    key_scan_params.dev_key = Key_getInstance();

    xTaskCreate(Key_Task_Wrapper,
                "Key_task",
                128,                  // 堆栈大小 (words) - 128*4=512字节，对于简单IO任务足够
                &key_scan_params,
                3,
                NULL);                // 无需保存句柄
}


/*
 * ===================================================================
 * UI 刷新任务
 * ===================================================================
 */

/**
 * @brief 创建UI刷新任务
 * @note  此任务没有包装器，直接创建。
 * UiUpdateTask 内部会自行获取 Oled、Bus 等句柄。
 */
void UiUpdateTaskCreate(void)
{
    // 栈大小 256 words (1KB) 应该足够
    xTaskCreate(UiUpdateTask,   // 任务函数 (在 ui.c 中定义)
                "Ui_Task",      // 任务名
                256,            // 堆栈大小
                NULL,           // 无参数
                2,              // 优先级设为 2 (低于传感器)
                NULL);          // 无需保存句柄
}
