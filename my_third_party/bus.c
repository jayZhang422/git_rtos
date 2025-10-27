#include "bus.h"

struct BusDevice{

    SubscriberCb subscribers[MAX_TOPICS][MAX_SUBSCRIBERS];//二维存放函数的数组，知道哪个主题谁是第几个订阅者
    uint8_t is_ready;
    uint8_t sub_count[MAX_TOPICS];//当前每个主题的订阅数量
    SemaphoreHandle_t BUS_mutex;//互斥量

};

static struct BusDevice g_my_busDevice;
//构造实体
BusHandle_t Bus_getInstance(void){
    return &g_my_busDevice;
}
//初始化全部赋值0
void Bus_Init(BusHandle_t handle){

    handle->is_ready = 1;
    handle->BUS_mutex = xSemaphoreCreateMutex();
    for(int i=0;i<MAX_TOPICS;i++){

        handle->sub_count[i] = 0;
    
    for(int j=0;j<MAX_SUBSCRIBERS;j++){
        handle->subscribers[i][j] = NULL;

    }
}
}
//发布函数，遍历该主题下的所有订阅者，如果当前有订阅就通过 handle->subscribers[topic][i](topic, data);将数据传给订阅者。
//subscribers实际是由函数指针创建的二维数组，因此可以直接传给函数相应值
void Bus_publish(BusHandle_t handle,Topic_t topic, void *data){

    if(handle == NULL || !handle->is_ready){
        return;
    }
   
    xSemaphoreTake(handle->BUS_mutex,portMAX_DELAY);
    for(int i=0;i<MAX_SUBSCRIBERS;i++){
     if (handle->subscribers[topic][i] != NULL) {
        handle->subscribers[topic][i](topic, data);
     
     }

    }
    xSemaphoreGive(handle->BUS_mutex);

}
//订阅函数，遍历该主题下的所有订阅者，如果当前有空位就将函数指针赋值给相应位置， handle->subscribers[topic][i] = cb;cb就是传入的函数地址
void Bus_subscribe(BusHandle_t handle,Topic_t topic, SubscriberCb cb){
     if(handle == NULL || !handle->is_ready){
        return;
    }
     
    for(int i=0;i<MAX_SUBSCRIBERS;i++){
        if(handle->subscribers[topic][i] == NULL&& handle->sub_count[topic]<MAX_SUBSCRIBERS){
             handle->subscribers[topic][i] = cb;
              handle->sub_count[topic]++;
             break;
        }
       

    }

}
