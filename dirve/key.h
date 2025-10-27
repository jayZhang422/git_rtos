#ifndef KEY_H_
#define KEY_H_

#include "bsp.h"
#include "bus.h"
#define Key1 GPIO_PIN_14
#define Key1_GPIO_Port GPIOC
#define Key2 GPIO_PIN_13
#define Key2_GPIO_Port GPIOC
#define Key3 GPIO_PIN_1
#define Key3_GPIO_Port GPIOA

struct KeyDevice;

typedef enum{

key1_pressed,
key2_pressed,
key3_pressed,
key_null

}KeyState_t;

typedef struct KeyDevice* KeyHandle_t;

void KeyInit(KeyHandle_t handle,BusHandle_t bus_handle);

void KeyScan(KeyHandle_t handle);

KeyHandle_t Key_getInstance(void);
void KeyTask(KeyHandle_t handle);
#endif /* KEY_H_ */
