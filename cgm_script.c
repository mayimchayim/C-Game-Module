#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cgm_script.h"


script* _scr;

int cgm_scrInit()
{
    _scr = (script*)malloc(sizeof(script));
    if(_scr == NULL){
        printf("\nError allocating script Object");
        return -1;
    }
    /*initialize const linked list*/
    _scr->curLine = 0;
    _scr->curLineChar = 0;
    _scr->numLines = 0;

    /*set default variable values*/
    _scr->var.x = 0;
    _scr->var.y = 0;

    _scr->var.col.bgra[0] = 0;
    _scr->var.col.bgra[1] = 0;
    _scr->var.col.bgra[2] = 0;
    _scr->var.col.bgra[3] = 0;

    /*initialize constant list*/
    _scr->cList = (constList*)malloc(sizeof(constList));
    if(_scr->cList == NULL){
        printf("\nError allocating script constant list");
        return -2;
    }

    _scr->cList->counter = 0;
    _scr->cList->head = NULL;
    _scr->cList->tail = NULL;

    return 1;
}

/*command script interpreter for game engine*/
void cgm_scrLoad(char* fname)
{
	char cmd[CMD_MAX_PARAM_SIZE];
	char param[CMD_MAX_PARAM_SIZE];
	char str_const[CMD_MAX_PARAM_SIZE];
	int val;
	int word_len;
    FILE* scr_file;
    int count;
    int i, j;

    _scr = (script*)malloc(sizeof(script));
    if(_scr == NULL){
        printf("\nScript object error");
        return;
    }



    scr_file = fopen(fname, "r");
    if(scr_file == NULL){
        printf("\nScript Loading failed");
        return;
    }

    /*count lines in script*/
    while(!feof(scr_file)){
        if(fgetc(scr_file) == '\n'){
            _scr->numLines++;
        }
    }
    _scr->numLines++;

    /*close file*/
    fclose(scr_file);
    scr_file = NULL;

    _scr->scripts = (char**)malloc(sizeof(char)*_scr->numLines);
    if(_scr->scripts == NULL){
        printf("\nError allocating script array");
        return;
    }

    /*reopen file, adding lines of script to array*/
    scr_file = fopen(fname, "r");
    if(!scr_file){
        printf("\nError adding script to array (reopen error)");
    }

    /*add each line, allocating each element along the way*/
    for(count = 0; count < _scr->numLines; count++){
        _scr->scripts[count] = (char*)malloc(sizeof(char) * (CMD_MAX_LINE_SIZE + 1));
        /*error check*/
        if(_scr->scripts[count] == NULL){
            printf("\nError allocating script line: %d", count);
        }
        fgets(_scr->scripts[count], CMD_MAX_LINE_SIZE, scr_file);
    }

	/*loop through collecting constants*/
	for(_scr->curLine = 0; _scr->curLine < _scr->numLines; _scr->curLine++){
        /*reset position in current line to 0*/
        _scr->curLineChar = 0;

        cgm_scrGetCommand(cmd);

		if(strcmp(cmd, CMD_DEFCONST) == 0){
			/*collect constant string name and numeric value*/
			cgm_scrGetStringParam(param);
			val = cgm_scrGetNumberParam();

			/*add to list*/
			cgm_scrAddConstant(param,val);
		}
	}

	/*reloop replacing constant string with number value*/
    for(_scr->curLine = 0; _scr->curLine < _scr->numLines; _scr->curLine++){
        /*reset position in current line after command and before first param*/
        _scr->curLineChar = 0;



    }

    fclose(scr_file);
}


void cgm_scrUnload()
{
    int i;

    if(!_scr->scripts){
        return;
    }
    for(i = 0; i < _scr->numLines; i++){
        free(_scr->scripts[i]);
        _scr->scripts[i] = NULL;
    }
    free(_scr->scripts);
    _scr->scripts = NULL;
}

/****ConstList functions*******/
int cgm_scrAddConstant(char* con, int val)
{
    constNode* new_node;
    int str_size;
    int i;

    new_node = (constNode*)malloc(sizeof(constNode));
    if(new_node == NULL){
        printf("\nError allocating new Constant node to list");
        return -1;
    }

    new_node->next = NULL;

    str_size = strlen(con); /*account for null terminator (\0)*/

    for(i = 0; i < str_size; i++){
        new_node->name[i] = con[i];
    }
    new_node->name[i] = '\0';
    new_node->val = val;

    /*add to command constant list*/
    if(_scr->cList->head == NULL){/*empty list*/
        _scr->cList->head = _scr->cList->tail = new_node;
    }
    else if(_scr->cList->head == _scr->cList->tail){/*single node in list*/
        _scr->cList->head->next = new_node;
        _scr->cList->tail = new_node;
    }
    else{/*2 or more nodes*/
        _scr->cList->tail->next = new_node;
        _scr->cList->tail = new_node;
    }

    /*increment node counter*/
    _scr->cList->counter += 1;

    return 1;
}


void cgm_scrDeleteConstantList(constList* cl)
{
    constNode* cur = cl->head;
    constNode* del = cl->head;

    while(cur != NULL){
        del = cur;
        cur = cur->next;

        /*delete node*/

        free(del);
        del = NULL;
    }
}

/*script parsing functions*/
void cgm_scrGetCommand(char* cmd)
{
    char c;
    int com_size = 0;

    /*loop from current character in line to space*/
    while(_scr->curLineChar < (int)strlen(_scr->scripts[_scr->curLine])){
        c = _scr->scripts[_scr->curLine][_scr->curLineChar];
        if(c == ' ' || c == '\n'){
            break;
        }
        cmd[com_size] = c;
        com_size++;
        _scr->curLineChar++;
    }

    /*append null terminator to command string*/
    cmd[com_size] = '\0';
    /*convert command string to uppercase*/
    strupr(cmd);

    /*increment character counter past space*/
    _scr->curLineChar++;

}

void cgm_scrGetStringParam(char* str)
{
    char c;
    int str_size = 0;

    /*skip first double quote(") sorrounding string*/
    _scr->curLineChar++;
    while(_scr->curLineChar < (int)strlen(_scr->scripts[_scr->curLine])){
        c = _scr->scripts[_scr->curLine][_scr->curLineChar];
        if(c == '"' || c == '\n'){
            break;
        }

        str[str_size] = c;

        str_size++;
        _scr->curLineChar++;
    }

    str[str_size] = '\0';
    /*move past quotes and space*/
    _scr->curLineChar += 2;
}

int cgm_scrGetNumberParam()
{
    char num_str[CMD_MAX_PARAM_SIZE];
    char c;
    int num_val;
    int param_size = 0;

    while(_scr->curLineChar < strlen(_scr->scripts[_scr->curLine])){
        c = _scr->scripts[_scr->curLine][_scr->curLineChar];
        if(c == ' ' || c == '\n'){
            break;
        }
        num_str[param_size] = c;
        param_size++;
        _scr->curLineChar++;
    }
    _scr->curLineChar++;

    num_str[param_size] = '\0';

    printf("\nStr Val: %s", num_str);

    num_val = atoi(num_str);

    printf("\nNum Val: %d", num_val);

    return num_val;
}

void cgm_scrGetToken(char* tok)
{

}

unsigned char cgm_scrNumberToChar(int num)
{

}

void cgm_scrRun()/*main script runner*/
{
    char cmd[CMD_MAX_PARAM_SIZE];
    char param[CMD_MAX_PARAM_SIZE];
    int num;

    for(_scr->curLine = 0; _scr->curLine < _scr->numLines; _scr->curLine++){
        /*reset position in current line to 0*/
        _scr->curLineChar = 0;

        cgm_scrGetCommand(cmd);

        if(strcmp(cmd, CMD_SETPOSITION) == 0){
            printf("\nCmd:SetPosition");
            /*set x and y position*/
            _scr->var.x = cgm_scrGetNumberParam();
            _scr->var.y = cgm_scrGetNumberParam();
        }
        else if(strcmp(cmd, CMD_SETCOLOR) == 0){

            _scr->var.col.bgra[2] = cgm_scrGetNumberParam();
            _scr->var.col.bgra[1] = cgm_scrGetNumberParam();
            _scr->var.col.bgra[0] = cgm_scrGetNumberParam();

            printf("\nCmd:SetColor: %0xd", _scr->var.col.value);
            printf("\nBlue: %d", _scr->var.col.bgra[0]);
            /*set alpha*/

        }
        else if(strcmp(cmd, CMD_FILL) == 0){
            printf("\nCmd:Fill");
            eng_fillScreen(_scr->var.col.value);
        }
        else if(strcmp(cmd, CMD_TEXTSTRING) == 0){
            cgm_scrGetStringParam(param);
            printf("\n%s", param);
        }
        else{
            printf("\nInvalid Command");
            /*leave script*/
            break;
        }
    }
}
