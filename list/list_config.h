#pragma once

#include <cstdint>
#include <stdlib.h>

typedef int64_t list_data;

/// Minimum 2-linked list size
#define MIN_LIST_SIZE 8

enum
ListError
{
    Failure = -1,
    Success = -2,
};

struct
list_elem
{
    list_data value;
    int prev;
    int next;
};

struct
list_man
{
    list_elem* data;
    int capacity;
    int free;
    bool status;
};
