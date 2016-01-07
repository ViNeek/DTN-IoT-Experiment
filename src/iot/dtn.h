#ifndef __IOT_DTN__H
#define __IOT_DTN__H

#include <iot/types.h>

#include "contiki.h"
#include "net/rime.h"
#include "random.h"

struct iotClient {
  struct unicast_conn m_UC;
  struct broadcast_conn m_BC;
} iotClient;


struct iotDataMule {
  struct unicast_conn m_UC;
  struct broadcast_conn m_BC;
} iotDataMule;

iotInt32 iot_mule_create(struct iotDataMule *dm);
iotInt32 iot_client_create(struct iotClient* p);

#endif // __IOT_DTN__H
