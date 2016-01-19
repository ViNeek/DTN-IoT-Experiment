#ifndef __IOT_TYPES__H
#define __IOT_TYPES__H

#define IOT_TRUE 1
#define IOT_FALSE 0

#ifndef NULL
#define NULL ( (void*)0 )
#endif

typedef char				iotChar;	
typedef unsigned char   	iotByte;	
typedef unsigned short		iotUInt16;
typedef short				iotInt16;
#if TARGET!=sky
typedef unsigned int		iotUInt32;
typedef int					iotInt32;
#else
typedef unsigned short		iotUInt32;
typedef short				iotInt32;
#endif
typedef unsigned long long	iotUInt64;
typedef long long			iotInt64; 
typedef iotUInt32			iotBool; 

#endif//__IOT_TYPES__H