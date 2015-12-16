#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{


    //char buffer[256];
    if (argc < 2) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       //exit(0);
    }

    printf("Luda %s\n", argv[1]);

    int type = atoi(argv[1]);

    struct iotClient c;
    struct iotBroadcaster b;
    struct iotListener l;

    iot_init();
    iot_init_networking();

    printf("Luda %d\n", type);

    if ( type == 1 )
        iot_client_create(&c);
    
    if ( type == 1 )
        iot_broadcaster_create(&b);
    
    printf("Luda %d\n", type);
    if ( type != 1 )
        iot_listener_create(&l);

    printf("Luda %d\n", type);

    for ( int i = 0; i < 15; i++ ) {
        printf("Number : %d\n", iot_random_in_range(20,40));
    }

    if ( type == 1 ) {
        iot_client_sendto( &b, &b, NULL );
        iot_client_sendto( &b, &b, NULL );
        iot_client_sendto( &b, &b, NULL );
        iot_client_sendto( &b, &b, NULL );
        iot_client_sendto( &b, &b, NULL );
        iot_client_sendto( &b, &b, NULL );
        iot_client_sendto( &b, &b, NULL );
        iot_client_sendto( &b, &b, NULL );
        iot_client_sendto( &b, &b, NULL );
        iot_client_sendto( &b, &b, NULL );
        iot_client_sendto( &b, &b, NULL );
    }
    else
        iot_client_recvfrom( &l, &l );

    if ( type == 1 )
        iot_client_close(&b);
    
    if ( type == 1 )
        iot_client_close(&c);

    if ( type != 1 )
        iot_client_close(&l);

    iot_uninit_networking();

    return 0;
}