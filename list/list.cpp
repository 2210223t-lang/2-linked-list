#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list_config.h"
#include "../debug/log.h"
#include "../debug/colours.h"
#include "../debug/dump.h"
#include "../debug/debug.h"


static int c = 0;



ListError
list_init ( list_man* list )
{
    assert( list );

    list->data = ( list_elem* ) calloc( MIN_LIST_SIZE, sizeof( list_elem ) );

    if ( !list->data )
    {
        printlg( "Initialization error.\n"
                 "Cannot allocate enough space, check parameters.\n\n" );
        return Failure;
    }

    list->data[0].stat.fatal = false;
    list->capacity = MIN_LIST_SIZE;
    list->free = 1;

    list->data[ 0 ] = { .value = 0, .prev = 0, .next = 0, .num = 0 };

    for ( int i = 1; i < MIN_LIST_SIZE; i++ )
        list->data[ i ] = { .value = 0, .prev = -1, .next = ( i + 1 ) % MIN_LIST_SIZE, .num = i };

    hash_fill( list );

    printlg( "Init result:\n\n" );
    dump_dot( list );

    return Success;
}


ListError
list_destr ( list_man* list )
{
    assert( list );

    if ( !list->data )
    {
        printlg( RED "Attempt to destroy null-pointer list.</p>\n\n" );
        return Failure;
    }

    free( list->data );
    *list = {};
    return Success;
}


static ListError
reall_list ( list_man* list )
{
    assert( list );

    if ( list->data[0].stat.fatal || list->data[ 0 ].stat.hashfal )
        return Failure;

    list_elem* data_temp = 0;
    int cap_temp = 0;

    cap_temp = 2 * list->capacity;
    data_temp = ( list_elem* ) realloc( list->data,   sizeof( list_elem ) * cap_temp );

    if ( !data_temp )
    {
        printlg( RED "List reallocation failure.</p>\n\n" );
        list->data[0].stat.fatal = true;
        return Failure;
    }

    if ( data_temp ) list->data = data_temp;
    list->capacity = cap_temp;

    list->free = cap_temp / 2;

    for ( int j = list->free; j < list->capacity; j++ )
    {
        *( ( char* ) &list->data[ j ].stat ) = 0;
        list->data[ j ] = { .value = 0, .prev = -1, .next = ( j + 1 ) % list->capacity, .num = j };
    }

    return Success;
}


int
list_add ( list_man* list, int num, int value )
{
    assert( list );
    assert( num >= 0 && num < list->capacity );

    printlg( BLU "Adding new elem result:</p>\n\n" );
    verify( list );

    if ( list->data[ 0 ].stat.freefal || list->data[ 0 ].stat.hashfal )
    {
        dump_dot( list );
        return Failure;
    }
    else if ( list->data[0].stat.fatal )
        return Failure;

    if ( !list->free )
    {
        printlg( BLU "Proceeding list reallocation upward.</p>\n\n" );
        if ( reall_list( list ) == Failure )
            return Failure;
    }

    if ( list->data[ num ].prev == -1 )
    {
        printlg( RED "Invalid element number.\n"
                     "Previous element isn't initialized. Number: %d</p>\n\n", num );
        list->data[0].stat.fatal = true;
        return Failure;
    }

    int buff = list->data[ list->free ].next;

    list->data[ list->free ] = { .value = value, .prev = num, .next = list->data[ num ].next, .num = list->free };
    list->data[ num ].next = list->free;
    list->data[ list->data[ list->free ].next ].prev = list->free;
    list->free = buff;

    hash_fill( list );

    dump_dot( list );

    return list->data[ num ].next;
}


int
list_pushb ( list_man* list, int value )
{
    assert( list );

    verify( list );


    if ( list->data[ 0 ].stat.freefal || list->data[ 0 ].stat.hashfal )
    {
        dump_dot( list );
        return Failure;
    }
    else if ( list->data[ 0 ].stat.fatal )
        return Failure;

    hash_fill ( list );

    return list_add( list, list->data[ 0 ].prev, value );
}


ListError
list_extr ( list_man* list, int num )
{
    assert( list );
    assert( num >= 0 && num < list->capacity );

    printlg( BLU "Extracting result:</p>\n\n" );
    verify( list );

    if ( list->data[0].stat.fatal || list->data[ 0 ].stat.hashfal || list->data[ 0 ].stat.freefal )
        return Failure;

    if ( list->data[ num ].prev == -1 || num <= 0 || num >= list->capacity )
    {
        printlg( RED "---| Invalid extraction number |---\n"
                     "This element wasn't initialized.\n"
                     "Extraction num: %d\n"
                     "Capacity: %d\n"
                     "===================================</p>\n\n",
                     num, list->capacity );

        dump_dot( list );
        list->data[0].stat.fatal = true;
        return Failure;
    }


    if ( list->data[ 0 ].next == list->data[ 0 ].prev && !list->data[ 0 ].prev )
    {
        list->data[ 0 ].next = 0;
        list->data[ 0 ].prev = 0;
        list->data[ num ].value = 0;

        printlg( "<p " RED "Attempt to extract from and empty - lenght list.</p>\n\n" );

        list->data[0].stat.fatal = true;
        return Failure;
    }

    list->data[ list->data[ num ].prev ].next = list->data[ num ].next;
    list->data[ list->data[ num ].next ].prev = list->data[ num ].prev;

    list->data[ num ] = { .value = 0, .prev = -1, .next = list->free, .num = num };
    list->free = num;

    hash_fill( list );

    dump_dot( list );

    return Success;
}


list_data
list_get ( list_man* list, int num )
{
    assert( list );
    assert( num >= 0 && num < list->capacity );

    if ( list->data[0].stat.fatal )
        return Failure;

    if ( list->data[ num ].prev == -1 )
    {
        printlg( "Getter failure.\n"
                 "Attempt to get value from shattered list.\n\n" );
        list->data[0].stat.fatal = true;
        return NULL;
    }

    return list->data[ num ].value;
}
