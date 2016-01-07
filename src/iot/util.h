#ifndef __IOT_UTIL__H
#define __IOT_UTIL__H

#include <iot/types.h>

enum iotLogLevels { INFO, DEBUG, WARN, ERROR, MAX_LEVEL, NONE };

#define IOT_LOG_INFO(format, ...) iot_log(INFO, __FILE__, format, __VA_ARGS__)
#define IOT_LOG_ERROR(format, ...) iot_log(ERROR, __FILE__, format, __VA_ARGS__)
#define IOT_LOG_WARNING(format, ...)                                           \
  iot_log(WARNING, __FILE__, format, __VA_ARGS__)
#define IOT_LOG_DEBUG(format, ...) iot_log(DEBUG, __FILE__, format, __VA_ARGS__)

#define MINUTES_IN_AN_HOUR 60
#define SECONDS_IN_A_MINUTE 60

iotInt32 iot_init();
iotInt32 iot_uninit();
void iot_log_level(enum iotLogLevels level);
iotBool iot_flip_coin();
iotUInt16 iot_random_in_range(iotUInt16 min, iotUInt16 max);
iotUInt64 iot_time();
struct ctimer *iot_ticker();
void iot_error(iotChar *msg);
void iot_error_exit(iotChar *msg);
void iot_log(enum iotLogLevels lvl, const iotChar *filename,
             const iotChar *format, ...);
void iot_clock_tick(void *ptr);

#endif // __IOT_UTIL__H
