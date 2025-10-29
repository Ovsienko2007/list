#include "list.h"

void add_list_elem(list_t *list, int pos, int new_elem, list_err_t *error){
    if (list == NULL){
        *error = null_ptr;
    }
    if (0 > pos || list->prev[pos] == -1 || pos > list->capacity - 2){ 
        *error = icoorect_pos;
    }
    if (*error){
        INTERNAL_DUMP(list, *error);
        return;
    }
    
    int new_free_elem = list->next[list->free_elem];

    list->prev[list->free_elem] = pos; // prev
    if (pos != list->tail){
        if (pos != 0){
            list->prev[list->next[pos]] = list->free_elem;
        }
        else{
            list->prev[list->head] = list->free_elem;
        }
    }
    else {
        list->tail = list->free_elem;
    }

    list->next[list->free_elem] = list->next[pos]; //next
    if (pos != 0){
        list->next[pos] = list->free_elem;
    }
    else{
        if (list->size != 0){
            list->next[list->free_elem] = list->head;
        }
        list->head             = list->free_elem;
    }

    list->data[list->free_elem] = new_elem;

    list->free_elem = new_free_elem;
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

void delite_list_elem(list_t *list, int pos, list_err_t *error){
    if (list == NULL){
        *error = null_ptr;
    }
    if (pos <= 0 || list->prev[pos] == -1 || pos > list->capacity - 1){ 
        *error = icoorect_pos;
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

    if (pos == list->tail){
        list->tail = new_prev;
    }
    if (pos == list->head){
        list->head = new_next;
    }

    if (verify_list(list, standart_mod)){
        *error = init_error;
        INTERNAL_DUMP(list, *error);
        return;
    }
}

void add_memory_for_list(list_t *list, list_err_t *error){
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
    list->data[list->capacity - 1] = cock;

    if (verify_list(list, standart_mod)){
        *error = init_error;
        INTERNAL_DUMP(list, *error);
        return;
    }
}

list_t init_list(list_err_t *error){
    list_t new_list = {
        .size = 0,
        .capacity = start_list_capacity,
        .data = (int *)calloc(start_list_capacity, sizeof(int)),
        .head = 0,
        .next = (int *)calloc(start_list_capacity, sizeof(int)),
        .tail = 0,
        .prev = (int *)calloc(start_list_capacity, sizeof(int)),
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

    new_list.data[0] = cock;
    new_list.data[new_list.capacity - 1] = cock;

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

    if (list->data[0] != cock){
        return left_canary_death;
    }
    if (list->data[list->capacity - 1] != cock){
        return right_canary_death;
    }
    return no_error;
}

void show_dump(list_t *list,  list_err_t error, dump_position position){
    static int num_call = 0;
    num_call++;
    FILE *file_html = fopen("DUMP/dump.html","a");//TODO remake

    fprintf(file_html, "<h3 align=\"center\"> StackDump called from %s:%d from func %s</h3>\n", position.file, position.line, position.func);
    fprintf(file_html, "<pre>\nerror: %d\n"  , error); // TODO remake
    fprintf(file_html, "The %d call\n", num_call);
    fprintf(file_html, "List[%p]:\n\n", list);

    if (error == null_ptr || error == null_ptr_inside){
        return;
    }
    fprintf(file_html, "  size:            %d\n", list->size);
    fprintf(file_html, "  capacity:        %d\n", list->capacity);
    fprintf(file_html, "  head:            %d\n", list->head);
    fprintf(file_html, "  tail:            %d\n", list->tail);
    fprintf(file_html, "  First free elem: %d\n\n", list->free_elem);

    fprintf(file_html, "  Data[%p] Next[%p] Prev[%p]\n", list->data, list->next, list->prev);

    fprintf(file_html, "<font color=\"magenta\">");
    fprintf(file_html, "  %20X %20d %20d\n", list->data[0], list->next[0], list->prev[0]);
    fprintf(file_html, "</font>");

    for (int i = 1; i < list->capacity - 1; i++){
        if (list->prev[i] == -1){
            fprintf(file_html, "<font color=\"red\">");
        }
        else{
            fprintf(file_html, "<font color=\"green\">");
        }
        fprintf(file_html, "  %20d %20d %20d\n", list->data[i], list->next[i], list->prev[i]);
        fprintf(file_html, "</font>");
        
    }
    fprintf(file_html, "<font color=\"magenta\">");
    fprintf(file_html, "  %20X %20d %20d\n", list->data[list->capacity - 1], list->next[list->capacity - 1], list->prev[list->capacity - 1]);
    fprintf(file_html, "</font></pre>\n");

    fprintf(file_html,  "<img src=\"./%d.png\" width=\"%d\" alt=\"DUMP %d\" />", num_call, image_size, num_call);

    fclose(file_html);

    char file_name[max_file_name_len] = {};
    sprintf(file_name, "DUMP/%d.dot", num_call);

    FILE *file = fopen(file_name ,"w");
    fprintf(file, "digraph structs {\n"
                  "  rankdir=LR;\n");
    fprintf(file, "el%d            [shape=record,style=\"rounded\", label=\"left | %X\"];\n",      0, list->data[0]);
    fprintf(file, "el%d            [shape=record,style=\"rounded\", label=\"right | %X\"];\n",     list->capacity - 1, list->data[list->capacity - 1]);
    fprintf(file, "Head            [shape=record,style=\"rounded\", label=\"Head | %d\"];\n",      list->head);
    fprintf(file, "Tail            [shape=record,style=\"rounded\", label=\"Tail | %d\"];\n",      list->tail);
    fprintf(file, "Free_elem       [shape=record,style=\"rounded\", label=\"Free elem | %d\"];\n", list->free_elem);
    fprintf(file, "Head->el%d      [color=\"#0ea30eff\"];\n", list->head);
    fprintf(file, "Tail->el%d      [color=\"#FF0EDD\"];\n", list->tail);
    fprintf(file, "Free_elem->el%d [color=\"#0ea30eff\"];\n", list->free_elem);
    
    for (int i = 1; i < list->capacity - 1; i++){
        fprintf(file, "el%d\t[shape=record,style=\"rounded\", label=\"  %d | {%d | %d | %d}\"];\n", i, list->data[i], list->prev[i], i, list->next[i]);
        fprintf(file, "el%d->el%d[color=\"#0ea30eff\"]\n", i, list->next[i]);
        if (list->prev[i] != -1){
            fprintf(file, "el%d->el%d[color=\"#FF0EDD\"]\n", i, list->prev[i]);
        }
    }
    
    fprintf(file, "}\n");
    fclose(file);
}