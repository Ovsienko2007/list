#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define USER_DUMP(list)             show_dump(list, no_error, dump_position {__FILE__, __FUNCTION__,  __LINE__})
#define INTERNAL_DUMP(list, error) show_dump(list, error,    dump_position {__FILE__, __FUNCTION__,  __LINE__})

static const int start_list_capacity = 16;
static const int cock                = 0xDEADBEEF;
static const int image_size          = 1600;
static const int max_file_name_len   = 16;


enum list_err_t{
    no_error           = 0,
    init_error         = 1,
    null_ptr           = 2,
    null_ptr_inside    = 3,
    left_canary_death  = 4,
    right_canary_death = 5,
    realloc_error      = 6,
    icoorect_pos       = 7,
};

enum verify_mod{
    standart_mod = 0,
    init_mod     = 1,
};

struct list_t{
    int size;
    int capacity;
    int *data;

    int head;
    int *next;
    int tail;
    int *prev;
    int free_elem;
};

struct dump_position{
    const char *file;
    const char *func;
    int line;
};

list_t init_list(list_err_t *error);
void destroy_list(list_t *list, list_err_t *error);

void add_list_elem(list_t *list, int pos, int new_elem, list_err_t *error);
void add_memory_for_list(list_t *list, list_err_t *error);
void delite_list_elem(list_t *list, int pos, list_err_t *error);

list_err_t verify_list(list_t *list, verify_mod mod);
void show_dump(list_t *list,  list_err_t error, dump_position position);
#endif