#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define USER_DUMP(list)             show_dump(list, no_error, dump_position {__FILE__, __FUNCTION__,  __LINE__})
#define INTERNAL_DUMP(list, error)  show_dump(list, error,    dump_position {__FILE__, __FUNCTION__,  __LINE__})

static const int   kStartListCapacity  = 16;
static const int   kCanary             = 0xDEADBEEF; // TODO implicit cast (unsigned to signed)
static const int   kImageSize          = 1600;
static const int   kMaxFileNameLen     = 16;

typedef int stack_elem_t;

enum list_err_t{
    no_error           = 0,
    init_error         = 1,
    null_ptr           = 2,
    null_ptr_inside    = 3,
    left_canary_death  = 4,
    right_canary_death = 5,
    realloc_error      = 6,
    incorect_pos       = 7,
};

enum verify_mod{
    standart_mod = 0,
    init_mod     = 1,
};

struct list_t{
    int size; // TODO use size_t for sizes. int is only 32-bit long
    int capacity; // TODO size_t
    int *data;
    int *next;
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

void add_list_elem(list_t *list, int pos, stack_elem_t new_elem, list_err_t *error);
void delete_list_elem(list_t *list, int pos, list_err_t *error);

stack_elem_t list_elem_by_pos(list_t list, int pos, list_err_t *error);

list_err_t verify_list(list_t *list, verify_mod mod);

void start_dump();
void end_dump();
void show_dump(list_t *list,  list_err_t error, dump_position position);
#endif
