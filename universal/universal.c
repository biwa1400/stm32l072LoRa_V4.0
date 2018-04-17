#include "stm32l0xx.h"
#include <string.h>

uint8_t* bufReverse(uint8_t* buf, unsigned int size)
{
		uint8_t convertArea[size];
		for(int i=0;i<size;i++)
		{
			convertArea[i]=buf[size-i-1];
		}
		memcpy(buf,convertArea,size);
		return buf;
}