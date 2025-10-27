#ifndef BUS_H_
#define BUS_H_

#include "bsp.h"

#define MAX_SUBSCRIBERS 10//每个主题最大订阅数量
#define MAX_TOPICS 10//最大主题数量

struct BusDevice;//先定义，只暴露这一个接口，告诉编译器一定有这个玩意
// struct Message;
typedef enum{

    TOPIC_KEY,
    TOPIC_ICM,
    TOPIC_BLE,
    TOPIC_WIT


}Topic_t;//所有可订阅的主题

typedef struct BusDevice* BusHandle_t;//创建一个结构体指针，编译器只分配四字节空间，也是为了后来操作，事物能够找到结构体

typedef void (*SubscriberCb)(Topic_t topic, void *data);//函数指针，包括订阅哪个主题，要接收啥消息，void* 可以转为任何数据

void Bus_Init(BusHandle_t handle);//初始化，让每个主题的订阅数量为0，所有订阅者为空

void Bus_publish(BusHandle_t handle,Topic_t topic, void *data);//通过主题发布消息

BusHandle_t Bus_getInstance(void);//到时候创建任务实际获得的实例，能够操作一切函数

void Bus_subscribe(BusHandle_t handle,Topic_t topic, SubscriberCb cb);//订阅主题，以及是哪个函数订阅的




#endif /* BUS_H_ */
