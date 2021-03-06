#ifndef __LOWPOWER_H
#define __LOWPOWER_H


#define LOWPOWER_STATUS_ON  0
#define LOWPOWER_STATUS_OFF 1

#define  SLEEP_VERF_STATUS                   LOWPOWER_STATUS_OFF
#define  SLEEP_REGULATOR_STATUS              LOWPOWER_STATUS_OFF
#define  SLEEP_WAIT_VERF_UP_STATUS           LOWPOWER_STATUS_OFF
#define  SLEEP_FLASH_STATUS                  LOWPOWER_STATUS_OFF

#define  STOP_VERF_STATUS                   LOWPOWER_STATUS_OFF
#define  STOP_REGULATOR_STATUS              LOWPOWER_STATUS_OFF
#define  STOP_WAIT_VERF_UP_STATUS           LOWPOWER_STATUS_OFF



#define  STANDBY_VERF_STATUS                   LOWPOWER_STATUS_OFF
#define  STANDBY_REGULATOR_STATUS              LOWPOWER_STATUS_OFF
#define  STANDBY_WAIT_VERF_UP_STATUS           LOWPOWER_STATUS_OFF


void lowpower_sleepMode(float second);
void lowpower_sleepMode_noTimer();
void lowpower_change_to_sleepMode();

void lowpower_stopMode(float second);
void lowpower_stopMode_noTimer();
void lowpower_change_to_stopMode();


void lowpower_set_always_stop (uint8_t isAlwaysStop);
uint8_t lowpower_get_always_stop ();



#endif