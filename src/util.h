#ifndef __IOT_UTIL__H
#define __IOT_UTIL__H

int iot_init();
int iot_random_in_range(int min, int max);
void iot_error(char* msg);
void iot_error_exit(char* msg);

#endif// __IOT_UTIL__H