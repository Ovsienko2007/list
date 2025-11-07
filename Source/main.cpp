#include <stdio.h>
#include "list.h"

int main(){
    // TODO this is not a good approach. What if I forget to call start_dump() or end_dump()?
    // I suggest you to choose one of the following approaches:
    // 1) You automatically execute fopen() + start_dump() before main (think how you can do it) 
    // and end_dump() at the exit of the program. In this case user should not worry about starting logs
    // properly. But they also can't change log file's name which is bad.
    //
    // 2) User executes fopen() + start_dump() in main, but end_dump() executes automatically.
    // This approach allows user to change log file's name. But in the other hand, user should consider
    // correctly setting the logs up
    //
    // In both cases you will need atexit() function and maybe global static variable for saving FILE *
    start_dump();
    list_err_t error = no_error;
    list_t list = init_list(&error);
    USER_DUMP(&list);

    for (int i = 0; i < 7; i++){
        add_list_elem(&list, i, i + 2, &error);    
    }
    USER_DUMP(&list);
    printf("%d\n", list_elem_by_pos(list, 1, &error));
    printf("%d\n", list_elem_by_pos(list, -2, &error));

    destroy_list(&list, &error);
    
    end_dump();
    return 0;
}
