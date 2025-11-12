#include "list.h"

static FILE *html_stream = nullptr;

#define COL_NEXT_BACKGROUND    "#7df879ff"
#define COL_NEXT_ARROW         "#0ea30eff"
#define COL_PREV_BACKGROUND    "#f879e7ff"
#define COL_PREV_ARROW         "#ff0eddff"
#define COL_BOTH_ARROW         "#2841ceff"
#define COL_VALUE_BACKGROUND   "#7f8cd6be"
#define COL_CURRENT_BACKGROUND "#d3cd77ff"

static const char *dump_file_position  = "DUMP/dump.html";

static void create_dot(int num_call, list_t *list);
static void create_html(int num_call, list_t *list, list_err_t error, dump_position position);
static const char *get_error_str(list_err_t error);

void show_dump(list_t *list,  list_err_t error, dump_position position){
    static int num_call = 0;
    num_call++;

    if (num_call == 1){
        start_dump();
        atexit(end_dump);
    }

    create_html(num_call, list, error, position);
    if (error != null_ptr && error != null_ptr_inside){
        create_dot(num_call, list);
    }
}

static void create_html(int num_call, list_t *list, list_err_t error, dump_position position){
    if (!html_stream) return;

    fprintf(html_stream, "<h3 align=\"center\"> StackDump called from %s:%d from func %s</h3>\n", position.file, position.line, position.func);

    fprintf(html_stream, "<pre style=\"font-size: 11pt\">\n <font color=\"red\">Error: %s</font>\n"  , get_error_str(error));
    fprintf(html_stream, "The %d call\n", num_call);
    fprintf(html_stream, "List[<font color=\"blue\">%p</font>]:\n\n", list);

    if (error == null_ptr || error == null_ptr_inside) return;

    fprintf(html_stream, "  size:            %lu\n", list->size);
    fprintf(html_stream, "  capacity:        %lu\n", list->capacity);
    fprintf(html_stream, "  head:            %lu\n", list->next[0]);
    fprintf(html_stream, "  tail:            %lu\n", list->prev[0]);
    fprintf(html_stream, "  First free elem: %lu\n\n", list->free_elem);

    fprintf(html_stream, "  Data[<font color=\"blue\">%p</font>] Next[<font color=\"blue\">%p</font>] Prev[<font color=\"blue\">%p</font>]\n",
                          list->data, list->next, list->prev);

    fprintf(html_stream, "<font color=\"magenta\">");
    
    if (list->prev[0] != kNoPtr){
        fprintf(html_stream, "  %20X %20lu %20lu\n", list->data[0], list->next[0], list->prev[0]);
    }
    else{
        fprintf(html_stream, "  %20X %20lu %20d\n", list->data[0], list->next[0], -1);
    }

    fprintf(html_stream, "</font>");

    for (size_t pos = 1; pos < list->capacity - 1; pos++){
        if (list->prev[pos] == kNoPtr){
            fprintf(html_stream, "<font color=\"red\">");
        }
        else{
            fprintf(html_stream, "<font color=\"green\">");
        }
        if (list->prev[pos] != kNoPtr){
            fprintf(html_stream, "  %20d %20lu %20lu\n", list->data[pos], list->next[pos], list->prev[pos]);
        }
        else{
            fprintf(html_stream, "  %20d %20lu %20d\n", list->data[pos], list->next[pos], -1);
        }
        fprintf(html_stream, "</font>");
        
    }
    fprintf(html_stream, "<font color=\"magenta\">");
    
    if (list->prev[list->capacity - 1] != kNoPtr){
        fprintf(html_stream, "  %20X %20lu %20lu\n", list->data[list->capacity - 1], list->next[list->capacity - 1], list->prev[list->capacity - 1]);
    }
    else{
        fprintf(html_stream, "  %20X %20lu %20d\n", list->data[list->capacity - 1], list->next[list->capacity - 1], -1);
    }

    fprintf(html_stream, "</font></pre>\n");

    fprintf(html_stream,  "<img src=\"./%d.png\" width=\"%d\" alt=\"DUMP %d\" />", num_call, kImageSize, num_call);
}

static void create_dot(int num_call, list_t *list){
    
    char file_name[kMaxFileNameLen] = {};
    sprintf(file_name, "DUMP/%d.dot", num_call);

    FILE *dot_stream = fopen(file_name ,"w");
    fprintf(dot_stream, "digraph structs {\n"
                  "nodesep=0.5\n");

                  
    fprintf(dot_stream, "Head  [shape=\"plaintext\", label=<\n"
                  "       <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">"
                  "       <TR><TD BGCOLOR=\"" COL_VALUE_BACKGROUND "\"> HEAD </TD></TR>"
                  "       <TR><TD BGCOLOR=\"" COL_NEXT_BACKGROUND "\">%lu</TD></TR></TABLE>>];\n", list->next[0]);
    fprintf(dot_stream, "Tail            [shape=\"plaintext\", label=<\n"
                  "       <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">"
                  "       <TR><TD BGCOLOR=\"" COL_VALUE_BACKGROUND "\"> TAIL </TD></TR>"
                  "       <TR><TD BGCOLOR=\"" COL_PREV_BACKGROUND  "\">%lu</TD></TR></TABLE>>];\n", list->prev[0]);
    fprintf(dot_stream, "Head->%lu [color=\"" COL_NEXT_ARROW "\"]\n", list->next[0]);
    fprintf(dot_stream, "Tail->%lu [color=\"" COL_PREV_ARROW "\"]\n", list->prev[0]);
    fprintf(dot_stream, "Free_elem       [shape=\"plaintext\", label=<\n"
                  "       <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">"
                  "       <TR><TD BGCOLOR=\"" COL_VALUE_BACKGROUND "\"> FREE ELEM </TD></TR>"
                  "       <TR><TD BGCOLOR=\"" COL_NEXT_BACKGROUND "\">%lu</TD></TR></TABLE>>];\n", list->free_elem);
    fprintf(dot_stream, "Free_elem->%lu [color=\"" COL_PREV_ARROW "\"];\n", list->free_elem);
    
    for (size_t list_pos = 0; list_pos < list->capacity; list_pos++){
        fprintf(dot_stream, 
        "%lu [shape=\"plaintext\", label=<\n"
        "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n"
        "    <TR>\n", list_pos);

        if (list_pos == 0 || list_pos == list->capacity - 1){
            fprintf(dot_stream, "<TD BGCOLOR=\"" COL_VALUE_BACKGROUND "\" COLSPAN=\"3\">%X</TD>\n", list->data[list_pos]);
        }
        else{
            fprintf(dot_stream, "<TD BGCOLOR=\"" COL_VALUE_BACKGROUND "\" COLSPAN=\"3\">%d</TD>\n", list->data[list_pos]);
        }
        
        fprintf(dot_stream,
        "    </TR>\n"
        "    <TR>\n");

        if (list->prev[list_pos] != kNoPtr){
            fprintf(dot_stream,
            "        <TD BGCOLOR=\"" COL_PREV_BACKGROUND    "\"> %lu </TD>\n", list->prev[list_pos]);
        } 
        else{
            fprintf(dot_stream,
            "        <TD BGCOLOR=\"" COL_PREV_BACKGROUND    "\"> %d  </TD>\n", -1);
        }
        
        fprintf(dot_stream,
        "        <TD BGCOLOR=\"" COL_CURRENT_BACKGROUND "\"> %lu </TD>\n"
        "        <TD BGCOLOR=\"" COL_NEXT_BACKGROUND    "\"> %lu </TD>\n"
        "    </TR>\n"
        "</TABLE>\n"
        ">];\n", list_pos, list->next[list_pos]);

        if (list_pos != 0 && list_pos != list->capacity - 1){
            if (list->prev[list->next[list_pos]] == list_pos){
                if (list->next[list_pos] != 0){
                    fprintf(dot_stream, "%lu->%lu[color=\"" COL_BOTH_ARROW "\"] [dir=\"both\"]\n", list_pos, list->next[list_pos]);
                }
            }
            else{
                if (list->next[list_pos] != 0 && list->next[list_pos] != list->capacity - 1){
                    fprintf(dot_stream, "%lu->%lu[color=\"" COL_NEXT_ARROW "\"]\n", list_pos, list->next[list_pos]);
                }
                if (list->prev[list_pos] != kNoPtr && list->prev[list_pos] != 0){
                    fprintf(dot_stream, "%lu->%lu[color=\"" COL_PREV_ARROW "\"]\n", list_pos, list->prev[list_pos]);
                }
            }
        }
    }

    fprintf(dot_stream, "{\n"
                  "rank=same;\n");
    for (size_t list_pos = 0; list_pos < list->capacity; list_pos++){
        fprintf(dot_stream, "%lu", list_pos);
        if (list_pos < list->capacity - 1){
            fprintf(dot_stream, ",");
        }
    }
    fprintf(dot_stream, ";\n}\n");
    
    fprintf(dot_stream, "}\n");
    fclose(dot_stream);
}



void start_dump(){
    html_stream = fopen(dump_file_position,"w");
    fprintf(html_stream, "<html lang=\"en\">\n"
                         "<head>\n"
                         "  <meta charset=\"UTF-8\">\n"
                         "  <title>Dump HTML</title>\n"
                         "</head>\n"
                         "<body>\n");
}   

void end_dump(){
    if (!html_stream) return;

    fprintf(html_stream, "\n</body>\n"
                         "</html>\n");
                        
    fclose(html_stream);
}

static const char *get_error_str(list_err_t error){
    switch(error){
        case no_error:
            return "There is no error";
        case init_error:
            return "Initialisation error";
        case null_ptr:
            return "List is null ptr";
        case null_ptr_inside:
            return "One of list inside lists is NULL";
        case left_canary_death:
            return "Left canary had been murdered";
        case right_canary_death:
            return "Right canary had been murdered";
        case realloc_error:
            return "Realloc error";
        case incorect_pos:
            return "Incorect position";
        case node_is_not_valid:
            return "Node is not valid";
        case cycle_is_in_list:
            return "Cycle is in_list";
        default:
            break;
    }
    return NULL;
}
