#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log.h"
#include "../list/list_config.h"



bool
list_consist ( list_man* list, int index )
{
    assert( list );

    if ( list->data[ index ].prev != -1 && index != list->data[ list->data[ index ].next ].prev &&
         list->data[ index ].next !=  0 )
    {
        printlg( "Detected list inconsistency in %d elem.\n\n", index );
        return false;
    }

    return true;
}


bool
free_elem_consist ( list_man* list, int index )
{
    assert( list );

    if ( list->data[ index ].prev == -1 && list->data[ list->data[ index ].next ].prev &&
         list->data[ index ].next !=  0 )
    {
        printlg( "Detected list's free elem inconsistency in %d elem.\n", index );
        return false;
    }
    return true;

}

bool
data_plausibility ( list_man* list )
{
    assert( list );

    if ( list->capacity <= 0 )
    {
        printlg( "---| Failure detected |---\n\n"
                 "Negative capacity value.\n"
                 "--------------------------\n\n" );
        fprintf( stderr, "Negative capacity value.\n\n" );
        return false;
    }


    return true;
}


ListError
verify ( list_man* list )
{
    assert( list );

    ListError status = Success;
    int i = 1;





    return Success;
}
