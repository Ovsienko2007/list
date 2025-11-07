#include "list.h"

#define COL_NEXT_BACKGROUND    "#7df879ff"
#define COL_NEXT_ARROW         "#0ea30eff"
#define COL_PREV_BACKGROUND    "#f879e7ff"
#define COL_PREV_ARROW         "#ff0eddff"
#define COL_BOTH_ARROW         "#2841ceff"
#define COL_VALUE_BACKGROUND   "#7f8cd6ff"
#define COL_CURRENT_BACKGROUND "#d3cd77ff"

static const char *dump_file_position  = "DUMP/dump.html";

static void create_dot(int num_call, list_t *list);
static void create_html(int num_call, list_t *list, list_err_t error, dump_position position);
static const char *get_error_str(list_err_t error);

void show_dump(list_t *list,  list_err_t error, dump_position position){
    static int num_call = 0;
    num_call++;

    create_html(num_call, list, error, position);
    if (error != null_ptr && error != null_ptr_inside){
        create_dot(num_call, list);
    }
}

static void create_html(int num_call, list_t *list, list_err_t error, dump_position position){
    FILE *file_html = fopen(dump_file_position,"a");
    fprintf(file_html, "<h3 align=\"center\"> StackDump called from %s:%d from func %s</h3>\n", position.file, position.line, position.func);

    fprintf(file_html, "<pre style=\"font-size: 11pt\">\n <font color=\"red\">Error: %s</font>\n"  , get_error_str(error));
    fprintf(file_html, "The %d call\n", num_call);
    fprintf(file_html, "List[<font color=\"blue\">%p</font>]:\n\n", list);

    if (error == null_ptr || error == null_ptr_inside){
        return;
    }
    fprintf(file_html, "  size:            %lu\n", list->size);
    fprintf(file_html, "  capacity:        %lu\n", list->capacity);
    fprintf(file_html, "  head:            %lu\n", list->next[0]);
    fprintf(file_html, "  tail:            %lu\n", list->prev[0]);
    fprintf(file_html, "  First free elem: %lu\n\n", list->free_elem);

    fprintf(file_html, "  Data[<font color=\"blue\">%p</font>] Next[<font color=\"blue\">%p</font>] Prev[<font color=\"blue\">%p</font>]\n",
                          list->data, list->next, list->prev);

    fprintf(file_html, "<font color=\"magenta\">");
    
    if (list->prev[0] != SIZE_MAX){
        fprintf(file_html, "  %20d %20lu %20lu\n", list->data[0], list->next[0], list->prev[0]);
    }
    else{
        fprintf(file_html, "  %20d %20lu %20d\n", list->data[0], list->next[0], -1);
    }

    fprintf(file_html, "</font>");

    for (size_t pos = 1; pos < list->capacity - 1; pos++){
        if (list->prev[pos] == SIZE_MAX){
            fprintf(file_html, "<font color=\"red\">");
        }
        else{
            fprintf(file_html, "<font color=\"green\">");
        }
        if (list->prev[pos] != SIZE_MAX){
            fprintf(file_html, "  %20d %20lu %20lu\n", list->data[pos], list->next[pos], list->prev[pos]);
        }
        else{
            fprintf(file_html, "  %20d %20lu %20d\n", list->data[pos], list->next[pos], -1);
        }
        fprintf(file_html, "</font>");
        
    }
    fprintf(file_html, "<font color=\"magenta\">");
    
    if (list->prev[list->capacity - 1] != SIZE_MAX){
        fprintf(file_html, "  %20d %20lu %20lu\n", list->data[list->capacity - 1], list->next[list->capacity - 1], list->prev[list->capacity - 1]);
    }
    else{
        fprintf(file_html, "  %20d %20lu %20d\n", list->data[list->capacity - 1], list->next[list->capacity - 1], -1);
    }

    fprintf(file_html, "</font></pre>\n");

    fprintf(file_html,  "<img src=\"./%d.png\" width=\"%d\" alt=\"DUMP %d\" />", num_call, kImageSize, num_call);

    fclose(file_html);
}

static void create_dot(int num_call, list_t *list){
    char file_name[kMaxFileNameLen] = {};
    sprintf(file_name, "DUMP/%d.dot", num_call);

    FILE *file = fopen(file_name ,"w");
    fprintf(file, "digraph structs {\n"
                  "nodesep=0.5\n");

                  
    fprintf(file, "Head  [shape=\"plaintext\", label=<\n"
                  "       <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">"
                  "       <TR><TD BGCOLOR=\"" COL_VALUE_BACKGROUND "\"> HEAD </TD></TR>"
                  "       <TR><TD BGCOLOR=\"" COL_NEXT_BACKGROUND "\">%lu</TD></TR></TABLE>>];\n", list->next[0]);
    fprintf(file, "Tail            [shape=\"plaintext\", label=<\n"
                  "       <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">"
                  "       <TR><TD BGCOLOR=\"" COL_VALUE_BACKGROUND "\"> TAIL </TD></TR>"
                  "       <TR><TD BGCOLOR=\"" COL_PREV_BACKGROUND  "\">%lu</TD></TR></TABLE>>];\n", list->prev[0]);
    fprintf(file, "Head->%lu [color=\"" COL_NEXT_ARROW "\"]\n", list->next[0]);
    fprintf(file, "Tail->%lu [color=\"" COL_PREV_ARROW "\"]\n", list->prev[0]);
    fprintf(file, "Free_elem       [shape=\"plaintext\", label=<\n"
                  "       <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">"
                  "       <TR><TD BGCOLOR=\"" COL_VALUE_BACKGROUND "\"> FREE ELEM </TD></TR>"
                  "       <TR><TD BGCOLOR=\"" COL_NEXT_BACKGROUND "\">%lu</TD></TR></TABLE>>];\n", list->free_elem);
    fprintf(file, "Free_elem->%lu [color=\"" COL_PREV_ARROW "\"];\n", list->free_elem);
    
    for (size_t list_pos = 0; list_pos < list->capacity; list_pos++){
        fprintf(file, 
        "%lu [shape=\"plaintext\", label=<\n"
        "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n"
        "    <TR>\n", list_pos);

        if (list_pos == 0 || list_pos == list->capacity - 1){
            fprintf(file, "<TD BGCOLOR=\"" COL_VALUE_BACKGROUND "\" COLSPAN=\"3\">%X</TD>\n", list->data[list_pos]);
        }
        else{
            fprintf(file, "<TD BGCOLOR=\"" COL_VALUE_BACKGROUND "\" COLSPAN=\"3\">%d</TD>\n", list->data[list_pos]);
        }
        
        fprintf(file,
        "    </TR>\n"
        "    <TR>\n");

        if (list->prev[list_pos] != SIZE_MAX){
            fprintf(file,
            "        <TD BGCOLOR=\"" COL_PREV_BACKGROUND    "\"> %lu </TD>\n", list->prev[list_pos]);
        } 
        else{
            fprintf(file,
            "        <TD BGCOLOR=\"" COL_PREV_BACKGROUND    "\"> %d  </TD>\n", -1);
        }
        
        fprintf(file,
        "        <TD BGCOLOR=\"" COL_CURRENT_BACKGROUND "\"> %lu </TD>\n"
        "        <TD BGCOLOR=\"" COL_NEXT_BACKGROUND    "\"> %lu </TD>\n"
        "    </TR>\n"
        "</TABLE>\n"
        ">];\n", list_pos, list->next[list_pos]);

        if (list_pos != 0 && list_pos != list->capacity - 1){
            if (list->prev[list->next[list_pos]] == list_pos){
                if (list->next[list_pos] != 0){
                    fprintf(file, "%lu->%lu[color=\"" COL_BOTH_ARROW "\"] [dir=\"both\"]\n", list_pos, list->next[list_pos]);
                }
            }
            else{
                if (list->next[list_pos] != 0 && list->next[list_pos] != list->capacity - 1){
                    fprintf(file, "%lu->%lu[color=\"" COL_NEXT_ARROW "\"]\n", list_pos, list->next[list_pos]);
                }
                if (list->prev[list_pos] != SIZE_MAX && list->prev[list_pos] != 0){
                    fprintf(file, "%lu->%lu[color=\"" COL_PREV_ARROW "\"]\n", list_pos, list->prev[list_pos]);
                }
            }
        }
    }

    fprintf(file, "{\n"
                  "rank=same;\n");
    for (size_t list_pos = 0; list_pos < list->capacity; list_pos++){
        fprintf(file, "%lu", list_pos);
        if (list_pos < list->capacity - 1){
            fprintf(file, ",");
        }
    }
    fprintf(file, ";\n}\n");
    
    fprintf(file, "}\n");
    fclose(file);
}


// TODO Too many fopens. Consider passing stream right to the function
// or storing it in global static variable.
// This way also limits you to use only files for dumps. Linux allows you 
// to open internet connections, pseudo-files, serial ports and etc as files  SIZE_MAX
void start_dump(){
    FILE *file_html = fopen(dump_file_position,"w");
    fprintf(file_html,  "<html lang=\"en\">\n"
                        "<head>\n"
                        "  <meta charset=\"UTF-8\">\n"
                        "  <title>Dump HTML</title>\n"
                        "</head>\n"
                        "<body>\n");
    fclose(file_html);

}   

void end_dump(){
    FILE *file_html = fopen(dump_file_position,"a");
    fprintf(file_html,  "</body>\n"
                        "</html>\n");
                        
    fclose(file_html);
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
