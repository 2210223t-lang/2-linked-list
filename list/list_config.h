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
    int num;
};


struct
elem_stat
{
    char seqfal  :1; //< Sequence fault
    char datainc :1; //< Data's inconsistency
    char fatal   :1; //< Fatal fault ( avoid printing )
    char freefal :1; //< Free value fault
    char nel     :1; //< neligible mis
    char freespac:3;
};

struct
list_man
{
    list_elem* data;
    int capacity;
    int free;
    elem_stat* status;
};
