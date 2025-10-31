#include "list.h"

static void add_memory_for_list(list_t *list, list_err_t *error);

void add_list_elem(list_t *list, int pos, stack_elem_t new_elem, list_err_t *error){
    if (list == NULL){
        *error = null_ptr;
    }
    if (0 > pos || list->prev[pos] == -1 || pos > list->capacity - 2){ 
        *error = incorect_pos;
    }
    if (*error){
        INTERNAL_DUMP(list, *error);
        return;
    }
    
    int new_free_elem = list->next[list->free_elem];
    int new_next    = list->next[pos];

    list->prev[list->next[pos]] = list->free_elem;
    list->next[pos]             = list->free_elem;

    list->next[list->free_elem] = new_next;
    list->prev[list->free_elem] = pos;
    list->data[list->free_elem] = new_elem;

    list->free_elem             = new_free_elem;
    list->size++;

    if (list->size + 3 == list->capacity){
        add_memory_for_list(list, error);
        
    }
    if (verify_list(list, standart_mod)){
        *error = init_error;
        INTERNAL_DUMP(list, *error);
        return;
    }
}

void delete_list_elem(list_t *list, int pos, list_err_t *error){
    if (list == NULL){
        *error = null_ptr;
    }
    if (pos <= 0 || list->prev[pos] == -1 || pos > list->capacity - 1){ 
        *error = incorect_pos;
    }
    if (*error){
        INTERNAL_DUMP(list, *error);
        return;
    }

    int new_next = list->next[pos];
    int new_prev = list->prev[pos];

    list->prev[pos] = -1;
    list->next[pos] = list->free_elem;
    list->free_elem = pos;

    
    list->next[new_prev] = new_next;
    list->prev[new_next] = new_prev;

    if (verify_list(list, standart_mod)){
        *error = init_error;
        INTERNAL_DUMP(list, *error);
        return;
    }
}

stack_elem_t list_elem_by_pos(list_t list, int pos, list_err_t *error){
    if (pos < 0){
        pos = list.size + pos + 1;
    }
    if (pos <= 0 || pos > list.capacity - 1){ 
        *error = incorect_pos;
        INTERNAL_DUMP(&list, *error);
        return 0;
    }

    int ret_point_value = 0;
    if (pos * 2 > list.size){
        pos = list.size - pos + 1;
        for (int list_pos = 0; list_pos < pos; list_pos++){
            ret_point_value = list.prev[ret_point_value];
        }
    }
    else{
        for (int list_pos = 0; list_pos < pos; list_pos++){
            ret_point_value = list.next[ret_point_value];
        }
    }
    

    if (verify_list(&list, standart_mod)){
        *error = init_error;
        INTERNAL_DUMP(&list, *error);
        return 0;
    }
    return list.data[ret_point_value];
}

static void add_memory_for_list(list_t *list, list_err_t *error){
    list->capacity *= 2;
    int *new_data = (int *)realloc(list->data, list->capacity * sizeof(int));
    int *new_next = (int *)realloc(list->next, list->capacity * sizeof(int));
    int *new_prev = (int *)realloc(list->prev, list->capacity * sizeof(int));

    if (!new_data || !new_prev || !new_prev){
        *error = realloc_error;
        INTERNAL_DUMP(list, *error);
        return;
    }
    list->data = new_data;
    list->next = new_next;
    list->prev = new_prev;

    for (int list_pos = list->size + 2; list_pos < list->capacity - 1; list_pos++){
        list->next[list_pos] = list_pos + 1;
        list->prev[list_pos] = -1;
    }
    list->data[list->capacity - 1] = kCanary;

    if (verify_list(list, standart_mod)){
        *error = init_error;
        INTERNAL_DUMP(list, *error);
        return;
    }
}

list_t init_list(list_err_t *error){
    list_t new_list = {
        .size = 0,
        .capacity = kStartListCapacity,
        .data = (int *)calloc(kStartListCapacity, sizeof(int)),
        .next = (int *)calloc(kStartListCapacity, sizeof(int)),
        .prev = (int *)calloc(kStartListCapacity, sizeof(int)),
        .free_elem = 1,
    };
    if (verify_list(&new_list, init_mod)){
        *error = init_error;
        INTERNAL_DUMP(&new_list, *error);
        return new_list;
    }

    for (int list_pos = 1; list_pos < new_list.capacity; list_pos++){
        new_list.next[list_pos] = list_pos + 1;
        new_list.prev[list_pos] = -1;
    }

    new_list.data[0] = kCanary;
    new_list.data[new_list.capacity - 1] = kCanary;

    if (verify_list(&new_list, init_mod)){
        *error = init_error;
        INTERNAL_DUMP(&new_list, *error);
        return new_list;
    }
    return new_list;
}

void destroy_list(list_t *list, list_err_t *error){
    if (list == NULL){
        *error = null_ptr;
        INTERNAL_DUMP(list, *error);
        return;
    }
    free(list->data);
    free(list->next);
    free(list->prev);
    return;
}

list_err_t verify_list(list_t *list, verify_mod mod){
    if (list == NULL){
        return null_ptr;
    }

    if (list->data == NULL || list->next == NULL || list->prev == NULL){
        return null_ptr_inside;
    }
    if (mod == init_mod) return no_error;

    if (list->data[0] != kCanary){
        return left_canary_death;
    }
    if (list->data[list->capacity - 1] != kCanary){
        return right_canary_death;
    }
    return no_error;
}