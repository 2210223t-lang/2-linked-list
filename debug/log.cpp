#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>

#include "log.h"

static FILE* logger_stream = stderr;//< Standard stream, which will be used if user didn't used logger_open


/**
 * @brief Changes output stream to a log - file
 */
int
logger_open ()
{
    logger_stream = fopen( logger_name, "w" );

    printlg( "<pre>" );
    return ( logger_stream ) ? 0 : -1;
}

/**
 * @brief fprintf( logger_stream ) realization
 */
void
printlg ( const char* text, ... )
{
    assert( text );

    va_list args;
    va_start( args, text );
    vfprintf( logger_stream, text, args );
    va_end( args );

    fflush( logger_stream );
}

/**
 * @brief Closes logger_stream and sets it to stderr
 */
void
logger_close ( void )
{
    // printlg( "<\\pre>" );

    if ( logger_stream != stderr )
        fclose( logger_stream );


    logger_stream = stderr;
}


/**
 * @brief Debugging puts function
 */
void
putslg_debug ( const char* a )
{

    while ( *a != '\n' && *a )

        switch ( *a )
        {
            case '\n':
                printlg ( "\\n" );
                a++;
                break;

            case '\a':
                printlg( "\\a" );
                a++;
                break;

            case '\t':
                printlg( "\\t" );
                a++;
                break;

            case '\b':
                printlg( "\\b" );
                a++;
                break;

            case '\r':
                printlg( "\\r" );
                a++;
                break;

            default:
                printlg ( "%s", *( a++ ) );
                break;
        };
}
