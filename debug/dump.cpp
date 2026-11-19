#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "../list/list_config.h"
#include "log.h"
#include "colours.h"


static size_t dump_id = 0;

#define PREV list->data[ index ].prev
#define NEXT list->data[ index ].next



static int
find_elem ( list_man* list, int index_find, bool is_prev )
{
    assert( list );

    if ( is_prev )
    {
        for ( int index = 1; index < list->capacity; index++ )
            if ( !list->data[ index ].stat.hashfal && PREV == index_find )
                return index;
    }
    else
        for ( int index = 1; index < list->capacity; index++ )
            if ( !list->data[ index ].stat.hashfal && NEXT == index_find )
                return index;
    return -1;
}


void
dump_colour ( list_man* list, int index, FILE* img, bool is_node )
{
    assert( list );
    assert( img );

    if ( list->data[ 0 ].stat.fatal )
        return;

    fprintf( img, "style = filled, " );

    if ( list->data[ index ].stat.datainc + list->data[ index ].stat.freefal +
         list->data[ index ].stat.fatal   + list->data[ index ].stat.hashfal && index )
        fprintf( img, "color = \"#f9003e\", fontcolor = \"#fffbfb\" ]\n" );
    else if ( !index )
        fprintf( img, "color = \"#b4da08\" ]\n" );
    else if ( index == list->data[ 0 ].next || index == list->data[ 0 ].prev )
        fprintf( img, "color = \"#00c6c9\" ]\n" );
    else if ( list->data[ index ].prev == -1 )
        fprintf( img, "color = \"#6c6c6c\" ]\n" );
    else
        fprintf( img, "color = \"#3ff407\" ]\n" );
}


static void
sequ_restore ( list_man* list, int index, FILE* img )
{
    assert( list );

    list_elem* curr = list->data + index;

    int prev_correct = find_elem( list, index, true );

    if ( prev_correct != -1 && list->data[ index ].stat.hashfal && prev_correct > index )
        fprintf( img, "n%d -> n%d [ color = \"#3ff407\" ]\n", index, prev_correct );
    else if ( !list->data[ index ].stat.hashfal && PREV != -1 && NEXT != 0 )
        fprintf( img, "n%d -> n%d [ color = \"#3ff407\" ]\n", index, NEXT );
    else if ( !list->data[ index ].stat.hashfal && PREV == -1 && NEXT != 0 )
        fprintf( img, "n%d -> n%d [ color = \"#6c6c6c\" ]\n", index, NEXT );

    int next_correct = find_elem( list, index, false );

    if ( next_correct != -1 && PREV >= 0 && PREV < list->capacity && list->data[ PREV ].next != index &&
         list->data[ index ].stat.hashfal && next_correct > index )
        fprintf( img, "n%d -> n%d [ color = \"#3ff407\" ]\n", next_correct, index );
    else if ( !list->data[ index ].stat.hashfal && PREV != -1 && PREV > index)
    {
        fprintf( img, "n%d -> n%d [ color = \"#3ff407\" ]\n", PREV, index );
    }
}


void
dump_elems ( list_man* list, FILE* img )
{
    assert( list );
    assert( img );

    if ( list->data[ 0 ].stat.fatal )
        return;

    for ( int index = 0; index < list->capacity; index++ )
    {
        fprintf( img, "n%d [ label = \"number = %d\\nvalue = %lld\\nprev = %d\\nnext = %d\", "
                      "shape = Mrecord, fontsize = 14,",
                       index, list->data[ index ].num,
                       list->data[ index ].value, PREV, NEXT );
        dump_colour( list, index, img, true );
    }

    if ( list->data[ 0 ].stat.hashfal )
        fprintf( img, "out [ label = \"OUT of array\", shape = Mrecord, fontsize = 14, style = filled, color = \"#f9003e\", fontcolor = \"#fffbfb\" ]\n " );
}


void //< dumps list for the 1st time ( prints )
dump_prep ( list_man* list, FILE* img )
{
    assert( list );
    assert( img );

    for ( int i = 1; i < list->capacity; i++ )
        fprintf( img, "n%d -> n%d [ style = invis, weight = 10000 ]\n", i - 1, i );
}


void
dump_seq ( list_man* list, FILE* img )
{
    assert( list );
    assert( img );

    for ( int index = 1; index < list->capacity; index++ )
    {
        sequ_restore( list, index, img );
        if ( list->data[ index ].stat.hashfal && ( ( NEXT >= list->capacity || NEXT < 0 ) ||
           ( ( PREV < 0 && PREV != -1 ) || PREV > list->capacity ) ) )
        {
            if ( NEXT >= list->capacity || NEXT < 0 )
                fprintf( img, "n%d -> out [ color = \"#f9003e\" ]\n", index );
            if ( ( PREV < 0 && PREV != -1 ) || PREV > list->capacity )
                fprintf( img, "out -> n%d [ color = \"#f9003e\" ]\n", index );
        }
        else if ( list->data[ index ].stat.hashfal )
        {
            if ( index != list->data[ NEXT ].prev )
            {
                fprintf( img, "n%d -> n%d [ ", index, NEXT );
                dump_colour( list, index, img, false );
            }
            if ( PREV != -1 && index != list->data[ PREV ].next )
            {
                fprintf( img, "n%d -> n%d [ ", PREV, index );
                dump_colour( list, index, img, false );
            }
        }
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
        *( ( char* ) &list->data[ i ].stat ) = 0;
}


void
print_meta ( list_man* list )
{
    assert( list );

    printlg( BLU "\n\n===| List metadata |===\n"
                 "Capacity value: %d\n", list->capacity );
    if ( list->capacity < 0 )
        printlg( "^^^^^^^^^^^^^^\n" );
    printlg("Free value: %d\n", list->free );
    if ( list->free < 0 || list->free > list->capacity || list->data[ list->free ].prev != -1 )
        printlg( "^^^^^^^^^^^^^^\n" );
    printlg( "Head: %d\n"
             "Tail: %d</p>\n",
              list->data[ 0 ].next, list->data[ 0 ].prev );
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

    if ( list->data[ 0 ].stat.fatal )
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
    printlg( "<hr>\n" );

    status_clean( list );
}
