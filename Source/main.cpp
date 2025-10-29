#include <stdio.h>
#include "list.h"

int main(){
    start_dump();
    list_err_t error = no_error;
    list_t list = init_list(&error);

    USER_DUMP(&list);

    for (int i = 0; i < 5; i++){
        add_list_elem(&list, i, i, &error);    
    }
    USER_DUMP(&list);

    delete_list_elem(&list, 3, &error);
    USER_DUMP(&list);

    delete_list_elem(&list, 5, &error);
    USER_DUMP(&list);


    delete_list_elem(&list, 1, &error);
    USER_DUMP(&list);

    add_list_elem(&list, 0, 14, &error);
    USER_DUMP(&list);

    destroy_list(&list, &error);
    end_dump();
    return 0;
}