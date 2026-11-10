#pragma once

#include "list_config.h"


/**
 * @brief Initializes 2-linked list
 *
 * @param[ in out ] list what variable to init
 *
 * @return Exit status
 */
ListError
list_init ( list_man* list );

/**
 * @brief Destroys 2-linked list
 *
 * @param[ in ] list which list to destroy
 *
 * @return Exit status
 */
ListError
list_destr ( list_man* list );

/**
 * @brief Adds one more element after num element
 *
 * @param[ in out ] list which list to change
 *
 * @param[ in ] num number of elem
 *
 * @param[ in ] value value of element
 *
 * @return Coef of the value param
 */
int
list_add ( list_man* list, int num, int value );

/**
 * @brief Destroys num element
 *
 * @param[ in out ] list which list to change
 *
 * @param[ in ] num number of elem
 *
 * @return Exit status
 */
ListError
list_extr ( list_man* list, int num );

/**
 * @brief standart Getter from 2-linked list
 *
 * @param[ out ] error Exit status
 *
 * @return Value of got elem
 */
list_data
list_get ( list_man* list, int num );

int
list_pushb ( list_man* list, int value );
