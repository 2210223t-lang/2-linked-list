#include <stdio.h>

#include "list/list.h"
#include "list/list_config.h"
#include "debug/dump.h"
#include "debug/log.h"


void
pollute ( list_man* list )
{

}


int
main ( void )
{
    logger_open();
    list_man list = {};
    list_init( &list );

    for ( int i = 1; i <= 4; i++ )
        list_pushb( &list, i );

    list.data[3].next = 9;
    list_pushb( &list, 5 );

    logger_close();
    return 0;
}
