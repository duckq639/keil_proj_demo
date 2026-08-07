#ifndef ALARM_SYS_H
#define ALARM_SYS_H

#include "led.h"
#include "buzzer.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum{
    IDEL,
    RUN,
    ALARM,
}alarm_sys_mode;

void alarm_sys_func(alarm_sys_mode target_mode);


#ifdef __cplusplus
}
#endif

#endif