#include <iot/util.h>

#include "contiki.h"

#include "lib/random.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static struct ctimer g_Ticker = {0};
static iotUInt32 g_TickCount = 0;
static enum iotLogLevels g_LogLevel = NONE;

struct ctimer *iot_ticker() {
  return &g_Ticker;
}

void iot_log_level(enum iotLogLevels level) { g_LogLevel = level; }

void iot_clock_tick(void *ptr) {
  IOT_LOG_INFO("%d", iot_random_in_range(100, 200));
  IOT_LOG_INFO("%d", iot_flip_coin());
  IOT_LOG_INFO("%d", g_TickCount);

  g_TickCount++;

  ctimer_reset(&g_Ticker);
}

iotInt32 iot_init() {

#ifndef DTN_IOTLAB_X86

  //clock_init();

#endif // DTN_IOTLAB_X86

  //ctimer_init();
  
  ctimer_set(iot_ticker(), 1 * CLOCK_SECOND, iot_clock_tick, (void *)NULL);
  
  return 0;
}

iotInt32 iot_uninit() { return 0; }

iotUInt64 iot_time() {
  // return clock_seconds();
  return clock_time();
}

iotBool iot_flip_coin() { return random_rand() % 2; }

iotUInt16 iot_random_in_range(iotUInt16 min, iotUInt16 max) {
  // max <= RAND_MAX < ULONG_MAX, so this is okay.
  iotUInt32 num_bins = (iotUInt32)(max - min) + 1;
  iotUInt32 num_rand = (iotUInt32)RANDOM_RAND_MAX + 1;
  iotUInt32 bin_size = num_rand / num_bins;
  iotUInt32 defect = num_rand % num_bins;

  iotUInt32 x;
  do {
    x = random_rand();
  } while (num_rand - defect <= (unsigned long)x);

  return (x / bin_size) + min;
}

static const char *LEVEL_STRING[MAX_LEVEL] = {"INFO", "DEBUG", "WARN", "ERROR"};

void iot_log(enum iotLogLevels lvl, const iotChar *filename,
             const iotChar *format, ...) {
  if (g_LogLevel == NONE) {
    return;
  }

  iotInt32 seconds = g_TickCount % SECONDS_IN_A_MINUTE;
  iotInt32 totalMinutes = g_TickCount / SECONDS_IN_A_MINUTE;
  iotInt32 minutes = totalMinutes % MINUTES_IN_AN_HOUR;
  iotInt32 hours = totalMinutes / MINUTES_IN_AN_HOUR;

  va_list args;
  printf("%s : %02d:%02d:%02d %-20s ", LEVEL_STRING[lvl], hours, minutes,
         seconds, filename);
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  printf("\n");
}
