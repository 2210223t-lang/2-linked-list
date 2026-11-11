#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log.h"
#include "../list/list_config.h"
#include "colours.h"



void
list_consist ( list_man* list )
{
    assert( list );

    if ( list->status[ 0 ].fatal )
        return;

    for ( int index = 0; index < list->capacity; index++ )
        if ( list->data[ index ].next !=  0 && !list->status[ index ].fatal &&
             list->data[ index ].prev != -1 && index != list->data[ list->data[ index ].next ].prev )
        {
            printlg( RED "---| Detected list inconsistency in %d elem |---\n\n"
                         "From %4d elem: [%d] --> [%d] --> [%d]\n"
                         "From next elem: [%d] --> [%d] --> [%d]\n"
                         "------------------------------------------------</p>\n\n",
                         index, index, list->data[ index ].prev, index, list->data[ index ].next,
                         list->data[ list->data[ index ].prev ].prev, list->data[ index ].next, list->data[ list->data[ index ].prev ].next );
            list->status[ index ].seqfal = true;
            list->status[ 0 ].seqfal = true;
        }
        else if ( list->data[ index ].next !=  0 && !list->status[ index ].fatal &&
                  list->data[ index ].prev == -1 && -1 != list->data[ list->data[ index ].next ].prev )
        {
            printlg( RED "---| Detected free elem list inconsistensy |---\n\n"
                         "From %d elem: [%d] --> [%d].\n"
                         "While [%d] elem's prev value: %d.\n"
                         "-----------------------------------------------</p>\n\n",
                         index, index, list->data[ index ].next, list->data[ index ].next,
                         list->data[ list->data[ index ].next ].prev );

            list->status[ index ].seqfal = true;
            list->status[ 0 ].seqfal = true;
        }
}


void
data_plausibility ( list_man* list )
{
    assert( list );

    if ( list->capacity <= 0 )
    {
        printlg( RED "---| Failure detected |---\n\n"
                     "Negative capacity value: %d.\n"
                     "--------------------------</p>\n\n", list->capacity );
        list->status[ 0 ].fatal = true;
    }
    else if ( list->free >= list->capacity )
    {
        printlg( RED "---| Failure detected |---\n\n"
                     "Incorrect list->free value: %d.\n"
                     "While capacity value: %d.\n"
                     "--------------------------</p>\n\n", list->free, list->capacity );
        list->status[ 0 ].freefal = true;
    }


    if ( list->status[ 0 ].fatal )
        return;

    for ( int i = 1; i < list->capacity; i++ )
    {
        if ( list->data[ i ].prev < -1 || list->data[ i ].prev >= list->capacity ||
             list->data[ i ].next <  0 || list->data[ i ].next >= list->capacity )
        {
            printlg( RED "---| Detected data inplausibility |---\n\n"
                         "Check %d elem prev/next value.\n"
                         "--------------------------------------</p>\n\n", i );

            list->status[ i ].fatal = true;
        }

    }
}


void
verify ( list_man* list )
{
    assert( list );

    data_plausibility( list );

    list_consist( list );

}
