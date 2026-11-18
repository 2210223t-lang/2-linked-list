#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log.h"
#include "../list/list_config.h"
#include "colours.h"



/**
 * @brief Standard djb2 hashing function
 */
static uint32_t
hash_djb2 ( unsigned char *str, size_t size )
{
    uint32_t hash = 5381;

    for ( size_t i = 0; i < size; i++ )
        hash = ( ( hash << 5 ) + hash ) + *( str++ ); /* hash * 33 + c */

    return hash;
}


static uint32_t
hash_calc ( list_elem* elem )
{
    assert( elem );

    uint32_t hash = hash_djb2( ( unsigned char* ) &elem->next, sizeof( elem->next ) );
    hash += hash_djb2( ( unsigned char* ) &elem->prev,  sizeof ( elem->prev  ) );
    hash += hash_djb2( ( unsigned char* ) &elem->value, sizeof ( elem->value ) );

    return hash;
}


void
hash_fill ( list_man* list )
{
    assert( list );
    assert( list );

    for ( int i = 1; i < list->capacity; i++ )
        list->data[ i ].hash = hash_calc( list->data + i );
}


static void
assert_my( list_man* list )
{
    assert( list );

    bool stat = true;

    if ( !list->data )
    {
        printlg( RED "List->data null pointer.</p>\n\n" );
        stat = false;
    }

    if ( !stat )
        abort();
}



static void
list_consist ( list_man* list )
{
    assert( list );

    if ( list->data[0].stat.fatal )
        return;

    for ( int index = 0; index < list->capacity; index++ )
        if ( list->data[ index ].next !=  0 && !list->data[ index ].stat.fatal &&
             list->data[ index ].prev != -1 && index != list->data[ list->data[ index ].next ].prev )
        {
            printlg( RED "---| Detected list inconsistency in %d elem |---\n\n"
                         "From %4d elem: [%d] --> [%d] --> [%d]\n"
                         "From next elem: [%d] --> [%d] --> [%d]\n"
                         "------------------------------------------------</p>\n\n",
                         index, index, list->data[ index ].prev, index, list->data[ index ].next,
                         list->data[ list->data[ index ].next ].prev, list->data[ index ].next, list->data[ list->data[ index ].next ].next );
            list->data[ index ].stat.seqfal = true;
            list->data[0].stat.seqfal = true;
        }
        else if ( list->data[ index ].next !=  0 && !list->data[ index ].stat.fatal &&
                  list->data[ index ].prev == -1 && -1 != list->data[ list->data[ index ].next ].prev )
        {
            printlg( RED "---| Detected free elem list inconsistensy |---\n\n"
                         "From %d elem: [%d] --> [%d].\n"
                         "While [%d] elem's prev value: %d.\n"
                         "-----------------------------------------------</p>\n\n",
                         index, index, list->data[ index ].next, list->data[ index ].next,
                         list->data[ list->data[ index ].next ].prev );

            list->data[ index ].stat.seqfal = true;
            list->data[0].stat.seqfal = true;
        }
}


static void
data_plausibility ( list_man* list )
{
    assert( list );

    if ( list->capacity <= 0 )
    {
        printlg( RED "---| Failure detected |---\n\n"
                     "Negative capacity value: %d.\n"
                     "--------------------------</p>\n\n", list->capacity );
        list->data[0].stat.fatal = true;
    }
    else if ( list->free >= list->capacity || list->free < 0 )
    {
        printlg( RED "---| Failure detected |---\n\n"
                     "Incorrect list->free value: %d.\n"
                     "While capacity value: %d.\n"
                     "--------------------------</p>\n\n", list->free, list->capacity );
        list->data[0].stat.freefal = true;
    }


    if ( list->data[0].stat.fatal )
        return;

    for ( int i = 0; i < list->capacity; i++ )
    {
        if ( list->data[ i ].prev < -1 || list->data[ i ].prev >= list->capacity ||
             list->data[ i ].next <  0 || list->data[ i ].next >= list->capacity )
        {
            printlg( RED "---| Detected data inplausibility |---\n\n"
                         "Check %d elem prev/next value.\n"
                         "--------------------------------------</p>\n\n", i );

            list->data[0].stat.fatal = true;
        }
        if ( list->data[ i ].num != i )
        {
            printlg( RED "---| Detected data inplausibility |---\n\n"
                         "Check %d elem num value.\n"
                         "--------------------------------------</p>\n\n", i );
            list->data[i].stat.nel = true;
        }

    }
}



static void
hash_check ( list_man* list )
{
    assert( list );
    assert_my( list );

    uint32_t temp = 0;

    for ( int i = 1; i < list->capacity; i++ )
    {
        temp = hash_calc( list->data + i );
        if ( temp != list->data[ i ].hash )
        {
            list->data[ i ].stat.hashfal = true;
            list->data[ 0 ].stat.hashfal = true;
        }
    }

}


void
verify ( list_man* list )
{
    assert( list );
    assert_my( list );

    data_plausibility( list );

    list_consist( list );

    hash_check( list );
}
