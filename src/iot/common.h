#ifndef __IOT_COMMON__H
#define __IOT_COMMON__H

#include <iot/types.h>
#include <iot/networking.h>
#include <iot/dtn.h>
#include <iot/util.h>
#include <iot/packet.h>

struct iotClient;
struct iotDataMule;

struct iotClient* iot_client_ref();
struct iotDataMule* iot_mule_ref();

#endif// __IOT_COMMON__H
