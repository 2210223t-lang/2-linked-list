#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "../list/list_config.h"
#include "log.h"
#include "colours.h"


static size_t dump_id = 0;



void
dump_color ( list_man* list, int index, FILE* img )
{
    assert( list );
    assert( img );

    if ( list->status[ 0 ].fatal )
        return;

    fprintf( img, "style = filled, " );

    if ( *( ( char* ) list->status + index ) && index )
        fprintf( img, "color = \"#a9062f\" ]\n" );
    else if ( !index )
        fprintf( img, "color = \"#b4da08\" ]\n" );
    else if ( index == list->data[ 0 ].next || index == list->data[ 0 ].prev )
        fprintf( img, "color = \"#00c6c9\" ]\n" );
    else if ( list->data[ index ].prev == -1 )
        fprintf( img, "color = \"#9781db\" ]\n" );
    else
        fprintf( img, "color = \"#3ff407\" ]\n" );
}


void
dump_elems ( list_man* list, FILE* img )
{
    assert( list );
    assert( img );

    if ( list->status[ 0 ].fatal )
        return;

    for ( int index = 0; index < list->capacity; index++ )
    {
        fprintf( img, "n%d [ label = \"value = %lld\\nprev = %d\\nnext = %d\", "
                      "shape = Mrecord, fontsize = 14,",
                       index, list->data[ index ].value, list->data[ index ].prev, list->data[ index ].next );
        dump_color( list, index, img );
    }
}


void //< dumps list for the 1st time ( prints )
dump_prep ( list_man* list, FILE* img )
{
    assert( list );
    assert( img );

    for ( int i = 1; i < list->capacity; i++ )
        fprintf( img, "n%d -> n%d [ style = invis, weight = 100 ]\n", i - 1, i );
}


void
dump_seq ( list_man* list, FILE* img )
{
    assert( list );
    assert( img );

    for ( int i = 1; i < list->capacity; i++ )
        if ( !list->status[ i ].fatal && list->data[ i ].next )
        {
            fprintf( img, "n%d -> n%d [", i, list->data[ i ].next );
            dump_color( list, i, img );
        }
}


void
convert_svg ( void )
{
    char command[ PATH_MAX ] = "";

    snprintf( command, PATH_MAX, "dot -Tsvg list_log/img/%zd.dot -o list_log/svg/%zd.svg", dump_id, dump_id );

    system( command );
}


void
logger_inc ( void )
{
    printlg("<img src=\"list_log/svg/%zd.svg\" alt=\"List dump %zd\">\n", dump_id, dump_id);
}


void
status_clean( list_man* list )
{
    assert( list );

    for ( int i = 0; i < list->capacity; i++ )
        *( ( char* ) list->status + i ) = 0;

}


void
print_meta ( list_man* list )
{
    assert( list );

    printlg( BLU "\n\n===| List metadata |===\n"
                 "Capacity value: %d\n"
                 "Free value: %d</p>\n",
                 list->capacity, list->free );

}


void
dump_dot ( list_man* list )
{
    assert( list );

    //---------Preparations-----------

    dump_id++;

    char dot_path[ PATH_MAX ] = "";

    snprintf( dot_path, PATH_MAX, "list_log/img/%zd.dot", dump_id );

    FILE* img = fopen( dot_path, "w" );

    //---------Dumping--------------

    if ( list->status[ 0 ].fatal )
    {
        printlg( RED "Cannot paint graph for the fatal fault of programm.</p>\n\n" );
        return;
    }

    fprintf( img, "digraph {\n"
                  "rankdir = LR\n" );

    dump_elems( list, img );

    dump_prep( list, img );

    dump_seq( list, img );

    fprintf( img, "}\n" );

    fclose( img );

    convert_svg();

    print_meta( list );

    logger_inc();

    printlg( "==================================\n\n\n" );

    status_clean( list );
}
