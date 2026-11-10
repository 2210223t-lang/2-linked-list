#pragma once

#define logger_name "log.html"


int
logger_open ();

void
printlg ( const char* text, ... );

void
logger_close ( void );

void
putslg_debug ( const char* a );
