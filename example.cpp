#include <stdio.h>

#include "list/list.h"
#include "list/list_config.h"
#include "debug/dump.h"
#include "debug/log.h"


int
main ( void )
{
    logger_open();
    list_man list = {};
    list_init( &list );


    for ( int i = 0; i < 5; i++ )
        list_pushb( &list, i );

    list_extr( &list, 4 );

    dump_dot( &list );

    logger_close();
    return 0;
}
