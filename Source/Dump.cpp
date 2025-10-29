#include "list.h"

static void creat_dot(int num_call, list_t *list);
static void creat_html(int num_call, list_t *list, list_err_t error, dump_position position);

void show_dump(list_t *list,  list_err_t error, dump_position position){
    static int num_call = 0;
    num_call++;
    
    creat_html(num_call, list, error, position);
    if (error != null_ptr && error != null_ptr_inside){
        creat_dot(num_call, list);
    }
}

static void creat_html(int num_call, list_t *list, list_err_t error, dump_position position){
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
    fprintf(file_html, "  head:            %d\n", list->next[0]);
    fprintf(file_html, "  tail:            %d\n", list->prev[0]);
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
}

static void creat_dot(int num_call, list_t *list){
    char file_name[max_file_name_len] = {};
    sprintf(file_name, "DUMP/%d.dot", num_call);

    FILE *file = fopen(file_name ,"w");
    fprintf(file, "digraph structs {\n");
    fprintf(file, "Head            [shape=record,label=\"{Head | %d}\"];\n",      list->next[0]);
    fprintf(file, "Tail            [shape=record,label=\"{Tail | %d}\"];\n",      list->prev[0]);
    fprintf(file, "Head->%d [color=\"#ff0eddff\"]\n", list->next[0]);
    fprintf(file, "Tail->%d [color=\"#0ea30eff\"]\n", list->prev[0]);
    fprintf(file, "Free_elem       [shape=record, label=\"{Free elem | %d}\"];\n", list->free_elem);
    fprintf(file, "Free_elem->%d [color=\"#0ea30eff\"];\n", list->free_elem);
    
    for (int list_pos = 0; list_pos < list->capacity; list_pos++){
        fprintf(file, 
        "%d [shape=\"plaintext\", label=<"
        "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">"
        "    <TR>", list_pos);

        if (list_pos == 0 || list_pos == list->capacity - 1){
            fprintf(file, "<TD COLSPAN=\"3\">%X</TD>", list->data[list_pos]);
        }
        else{
            fprintf(file, "<TD COLSPAN=\"3\">%d</TD>", list->data[list_pos]);
        }
        
        fprintf(file,
        "    </TR>"
        "    <TR>"
        "        <TD>%d</TD>"
        "        <TD>%d</TD>"
        "        <TD>%d</TD>"
        "    </TR>"
        "</TABLE>"
        ">];", list->prev[list_pos], list_pos, list->next[list_pos]);

        if (list_pos != 0 && list_pos != list->capacity - 1){
            if (list->next[list_pos] != 0 && list->next[list_pos] != list->capacity - 1){
                fprintf(file, "%d->%d[color=\"#0ea30eff\"]\n", list_pos, list->next[list_pos]);
            }
            if (list->prev[list_pos] != -1 && list->prev[list_pos] != 0){
                fprintf(file, "%d->%d[color=\"#ff0eddff\"]\n", list_pos, list->prev[list_pos]);
            }
        }
    }

    fprintf(file, "{\n"
                  "rank=same;\n");
    for (int list_pos = 0; list_pos < list->capacity; list_pos++){
        fprintf(file, "%d", list_pos);
        if (list_pos < list->capacity - 1){
            fprintf(file, ",");
        }
    }
    fprintf(file, ";\n}\n");
    
    fprintf(file, "}\n");
    fclose(file);
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