#include <stdio.h>
#include "list.h"

void start_dump();
void end_dump();

int main(){
    start_dump();
    list_err_t error = no_error;
    list_t list = init_list(&error);

    USER_DUMP(&list);

    for (int i = 0; i < 5; i++){
        add_list_elem(&list, i, i, &error);    
    }
    USER_DUMP(&list);

    delite_list_elem(&list, 3, &error);
    USER_DUMP(&list);

    delite_list_elem(&list, 5, &error);
    USER_DUMP(&list);


    delite_list_elem(&list, 1, &error);
    USER_DUMP(&list);

    add_list_elem(&list, 0, 14, &error);
    USER_DUMP(&list);

    destroy_list(&list, &error);
    end_dump();
    return 0;
}

void start_dump(){
    FILE *file_html = fopen("DUMP/dump.html","w"); // TODO remake
    fprintf(file_html,  "<html lang=\"en\">\n"
                        "<head>\n"
                        "  <meta charset=\"UTF-8\">\n"
                        "  <title>Dump HTML</title>\n"
                        "</head>\n"
                        "<body>\n");

    fclose(file_html);

}   

void end_dump(){
    FILE *file_html = fopen("DUMP/dump.html","a"); // TODO remake
    fprintf(file_html,  "</body>\n"
                        "</html>\n");
                        
    fclose(file_html);

}

