#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "list_config.h"
#include "log.h"


static size_t dump_id = 0;



void
dump_color ( list_man* list, int index, FILE* img )
{
    assert( list );
    assert( img );

    fprintf( img, "style = filled, " );

    if ( !index )
        fprintf( img, "color = lightyellow ]\n" );
    else if ( index == list->data[ 0 ].next || index == list->data[ 0 ].prev )
        fprintf( img, "color = lightblue ]\n" );
    else if ( list->data[ index ].prev == -1 )
        fprintf( img, "color = lightgray ]\n" );
    else
        fprintf( img, "color = \"#C8E6C9\" ]\n" );
}


void
dump_elems ( list_man* list, FILE* img )
{
    assert( list );
    assert( img );
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

    int elem = list->data[ 0 ].next;

    while ( list->data[ elem ].next )
    {
        fprintf( img, "n%d -> n%d [", elem, list->data[ elem ].next );
        dump_color( list, elem, img );
        elem = list->data[ elem ].next;
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
dump_dot ( list_man* list )
{
    assert( list );

    //---------Preparations-----------

    dump_id++;

    char dot_path[ PATH_MAX ] = "";

    snprintf( dot_path, PATH_MAX, "list_log/img/%zd.dot", dump_id );

    FILE* img = fopen( dot_path, "w" );

    //---------Dumping--------------

    fprintf( img, "digraph {\n"
                  "rankdir = LR\n" );

    dump_elems( list, img );

    dump_prep( list, img );

    dump_seq( list, img );

    fprintf( img, "}\n" );

    fclose( img );

    convert_svg();

    logger_inc();
}
