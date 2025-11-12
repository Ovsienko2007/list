#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define USER_DUMP(list)             show_dump(list, no_error, dump_position {__FILE__, __FUNCTION__,  __LINE__})
#define INTERNAL_DUMP(list, error)  show_dump(list, error,    dump_position {__FILE__, __FUNCTION__,  __LINE__})

static const int    kStartListCapacity  = 16;
static const int    kCanary             = (int) 0xDEADBEEF;
static const int    kImageSize          = 1600;
static const int    kMaxFileNameLen     = 16;
static const size_t kNoPtr              = SIZE_MAX;

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
    node_is_not_valid  = 8,
    cycle_is_in_list   = 9,
};

enum verify_mod{
    eStandartMod = 0,
    eInitMod     = 1,
};

struct list_t{
    size_t        size;
    size_t        capacity;
    stack_elem_t *data;
    size_t       *next;
    size_t       *prev;
    size_t        free_elem;
};

struct dump_position{
    const char *file;
    const char *func;
    int         line;
};

list_t init_list(list_err_t *error);
void destroy_list(list_t *list, list_err_t *error);

void add_list_elem(list_t *list, size_t pos, stack_elem_t new_elem, list_err_t *error);
void delete_list_elem(list_t *list, size_t pos, list_err_t *error);

stack_elem_t list_elem_by_pos(list_t list, int pos, list_err_t *error);

list_err_t verify_list(list_t *list, verify_mod mod);

void start_dump();
void end_dump();
void show_dump(list_t *list,  list_err_t error, dump_position position);
#endif
