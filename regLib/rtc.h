#ifndef __RTC_H
#define __RTC_H

#define RTC_RANGE_MODE_HZ  0
#define RTC_RANGE_MODE_KHZ 1

void rtc_allow();
void rtc_disAllow();
void rtc_Init();
void rtc_DeInit();
void rtc_wakeup_interrupt_setting(float second);

#endif