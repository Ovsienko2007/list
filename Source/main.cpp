#include <stdio.h>
#include "list.h"

int main(){
    list_err_t error = no_error;
    list_t list = init_list(&error);

    USER_DUMP(&list);

    for (int i = 0; i < 7; i++){
        add_list_elem(&list, (size_t) i, i + 2, &error);    
    }

    USER_DUMP(&list);

    printf("%d\n", list_elem_by_pos(list, 1, &error));
    printf("%d\n", list_elem_by_pos(list, -2, &error)); 

    destroy_list(&list, &error);
    
    return 0;
}
