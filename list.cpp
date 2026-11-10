#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list_config.h"
#include "log.h"
#include "colours.h"
#include "dump.h"

static int c = 0;

ListError
list_init ( list_man* list )
{
    assert( list );

    fprintf( stderr, "Initializing.\n\n" );

    list->data = ( list_elem* ) calloc( MIN_LIST_SIZE, sizeof( list_elem ) );

    if ( !list->data )
    {
        printlg( "Initialization error.\n"
                 "Cannot allocate enough space, check parameters.\n\n" );

        list->status = false;
        return Failure;
    }

    list->status = true;
    list->capacity = MIN_LIST_SIZE;
    list->free = 1;

    list->data[ 0 ] = { .value = 0, .prev = 0, .next = 0 };

    for ( int i = 1; i < MIN_LIST_SIZE; i++ )
        list->data[ i ] = { .value = 0, .prev = -1, .next = ( i + 1 ) % MIN_LIST_SIZE };


    printlg( "Init result:\n\n" );
    dump_dot( list );

    return Success;
}


ListError
list_destr ( list_man* list )
{
    assert( list );

    if ( !list->status )
    {
        printlg( RED "Avoid destroing invalid list.</p>\n\n" );
        return Failure;
    }
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

    if ( !list->status )
    {
        printlg( RED "Stopping reallocation of invalid list.</p>\n\n" );
        return Failure;
    }

    int temp = list->capacity;

    list->capacity *= 2;
    list->data = ( list_elem* ) realloc( list->data, sizeof( list_elem ) * list->capacity );

    if ( !list->data )
    {
        printlg( RED "List reallocation failure.</p>\n\n" );
        list->status = false;
        return Failure;
    }

    int i = list->free;

    while ( list->data[ i ].next )
        i = list->data[ i ].next;

    list->data[ i ].next = temp;

    for ( int j = temp; j < list->capacity; i++ )
        list->data[ j ] = { .value = 0, .prev = -1, .next = ( j + 1 ) % list->capacity };

    return Success;
}


int
list_add ( list_man* list, int num, int value )
{
    assert( list );

    if ( !list->status )
    {
        printlg( RED "Avoiding adding into invalid list.</p>\n\n" );
        return Failure;
    }

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
        list->status = false;
        return Failure;
    }

    int buff = list->data[ list->free ].next;

    list->data[ list->free ] = { .value = value, .prev = num, .next = list->data[ num ].next };
    list->data[ num ].next = list->free;
    list->data[ list->data[ list->free ].next ].prev = list->free;
    list->free = buff;


    printlg( BLU "Adding new elem result:</p>\n\n" );
    dump_dot( list );

    return list->data[ num ].next;
}


int
list_pushb ( list_man* list, int value )
{
    assert( list );

    if ( !list->status )
    {
        printlg( RED "Avoiding pushingb into invalid list.</p>\n\n" );
        return Failure;
    }

    return list_add( list, list->data[ 0 ].prev, value );
}


ListError
list_extr ( list_man* list, int num )
{
    assert( list );

    if ( !list->status )
    {
        printlg( RED "Avoid extracting from invalid list.</p>\n\n" );
        return Failure;
    }

    if ( list->data[ num ].prev == -1 )
    {
        printlg( "invalid extraction number.\n"
                 "This element wasn't initialized.\n\n" );

        list->status = false;
        return Failure;
    }


    if ( list->data[ 0 ].next == list->data[ 0 ].prev && !list->data[ 0 ].prev )
    {
        list->data[ 0 ].next = 0;
        list->data[ 0 ].prev = 0;
        list->data[ num ].value = 0;

        printlg( "<p " RED "Attempt to extract from and empty - lenght list.</p>\n\n" );

        list->status = false;
        return Failure;
    }

    list->data[ list->data[ num ].prev ].next = list->data[ num ].next;
    list->data[ list->data[ num ].next ].prev = list->data[ num ].prev;

    list->data[ num ] = { .value = 0, .prev = -1, .next = list->free };
    list->data[ list->free ].prev = -1;
    list->free = num;

    printlg( BLU "Extracting result:</p>\n\n" );
    dump_dot( list );

    return Success;
}


list_data
list_get ( list_man* list, int num )
{
    assert( list );

    if ( !list->status )
    {
        printlg( RED "Avoiding getting values from invalid list.</p>\n\n" );
        return Failure;
    }

    if ( list->data[ num ].prev == -1 )
    {
        printlg( "Getter failure.\n"
                 "Attempt to get value from shattered list.\n\n" );
        list->status = false;
        return NULL;
    }

    return list->data[ num ].value;
}
