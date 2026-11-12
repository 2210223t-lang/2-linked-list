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

    list->data   = ( list_elem* ) calloc( MIN_LIST_SIZE, sizeof( list_elem ) );
    list->status = ( elem_stat* ) calloc( MIN_LIST_SIZE, sizeof( elem_stat ) );

    if ( !list->data || !list->status )
    {
        printlg( "Initialization error.\n"
                 "Cannot allocate enough space, check parameters.\n\n" );

        list->status[ 0 ].fatal = true;
        return Failure;
    }

    list->status[ 0 ].fatal = false;
    list->capacity = MIN_LIST_SIZE;
    list->free = 1;

    list->data[ 0 ] = { .value = 0, .prev = 0, .next = 0 };

    for ( int i = 1; i < MIN_LIST_SIZE; i++ )
        list->data[ i ] = { .value = 0, .prev = -1, .next = ( i + 1 ) % MIN_LIST_SIZE };


    printlg( "Init result:\n\n" );
    verify( list );
    dump_dot( list );

    return Success;
}


ListError
list_destr ( list_man* list )
{
    assert( list );

    if ( list->status[ 0 ].fatal )
        return Failure;

    if ( !list->data )
    {
        printlg( RED "Attempt to destroy null-pointer list.</p>\n\n" );
        return Failure;
    }

    free( list->data );
    return Success;
}


static ListError
reall_list ( list_man* list )
{
    assert( list );

    if ( list->status[ 0 ].fatal )
        return Failure;

    list_elem* data_temp = 0;
    elem_stat* stat_temp = 0;
    int cap_temp = 0;

    cap_temp = 2 * list->capacity;
    data_temp = ( list_elem* ) realloc( list->data,   sizeof( list_elem ) * list->capacity );
    stat_temp = ( elem_stat* ) realloc( list->status, sizeof( elem_stat ) * list->capacity );

    if ( !data_temp || !stat_temp )
    {
        printlg( RED "List reallocation failure.</p>\n\n" );
        list->status[ 0 ].fatal = true;
        return Failure;
    }

    if ( data_temp ) list->data = data_temp;
    if ( stat_temp ) list->status = stat_temp;
    list->capacity = cap_temp;

    list->free = cap_temp;

    for ( int j = list->free; j < list->capacity; j++ )
    {
        *( ( char* ) list->status + j ) = 0;
        list->data[ j ] = { .value = 0, .prev = -1, .next = ( j + 1 ) % list->capacity };
    }

    return Success;
}


int
list_add ( list_man* list, int num, int value )
{
    assert( list );

    printlg( BLU "Adding new elem result:</p>\n\n" );
    verify( list );

    if ( list->status[ 0 ].freefal )
    {
        dump_dot( list );
        return Failure;
    }
    else if ( list->status[ 0 ].fatal )
        return Failure;

    if ( !list->free )
    {
        printlg( BLU "Proceeding list reallocation upward.</p>\n\n" );
        if ( reall_list( list ) == Failure )
            return Failure;
    }

    if ( list->data[ num ].prev == -1 )
    {
        printlg( RED "invalid element number.\n"
                     "Previous element isn't initialized. Number: %d</p>\n\n", num );
        list->status[ 0 ].fatal = true;
        return Failure;
    }

    int buff = list->data[ list->free ].next;

    list->data[ list->free ] = { .value = value, .prev = num, .next = list->data[ num ].next };
    list->data[ num ].next = list->free;
    list->data[ list->data[ list->free ].next ].prev = list->free;
    list->free = buff;

    verify( list );
    dump_dot( list );

    return list->data[ num ].next;
}


int
list_pushb ( list_man* list, int value )
{
    assert( list );

    verify( list );


    if ( list->status[ 0 ].freefal )
    {
        dump_dot( list );
        return Failure;
    }
    else if ( list->status[ 0 ].fatal )
        return Failure;

    return list_add( list, list->data[ 0 ].prev, value );
}


ListError
list_extr ( list_man* list, int num )
{
    assert( list );

    printlg( BLU "Extracting result:</p>\n\n" );
    verify( list );

    if ( list->status[ 0 ].fatal )
        return Failure;

    if ( list->data[ num ].prev == -1 )
    {
        printlg( "invalid extraction number.\n"
                 "This element wasn't initialized.\n\n" );

        list->status[ 0 ].fatal = true;
        return Failure;
    }


    if ( list->data[ 0 ].next == list->data[ 0 ].prev && !list->data[ 0 ].prev )
    {
        list->data[ 0 ].next = 0;
        list->data[ 0 ].prev = 0;
        list->data[ num ].value = 0;

        printlg( "<p " RED "Attempt to extract from and empty - lenght list.</p>\n\n" );

        list->status[ 0 ].fatal = true;
        return Failure;
    }

    list->data[ list->data[ num ].prev ].next = list->data[ num ].next;
    list->data[ list->data[ num ].next ].prev = list->data[ num ].prev;

    list->data[ num ] = { .value = 0, .prev = -1, .next = list->free };
    list->free = num;

    dump_dot( list );

    return Success;
}


list_data
list_get ( list_man* list, int num )
{
    assert( list );

    if ( list->status[ 0 ].fatal )
        return Failure;

    if ( list->data[ num ].prev == -1 )
    {
        printlg( "Getter failure.\n"
                 "Attempt to get value from shattered list.\n\n" );
        list->status[ 0 ].fatal = true;
        return NULL;
    }

    return list->data[ num ].value;
}
