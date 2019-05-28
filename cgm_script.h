#ifndef CGM_SCRIPT_H
#define CGM_SCRIPT_H

#include "cgm_engine.h"

/*script max values*/
#define CMD_MAX_LINE_SIZE       1024
#define CMD_MAX_PARAM_SIZE      64
#define CMD_MAX_CONST_SIZE      64

/*command list*/
#define CMD_DEFCONST	"DEFCONST"
#define CMD_SETCOLOR    "SETCOLOR" /*rgb parameters for draw commands*/
#define CMD_SETPOSITION "SETPOSITION" /*x and y number parameters for drawing start position*/
/*draw commands*/
#define CMD_TEXTSTRING  "TEXTSTRING" /*string parameter*/
#define CMD_FILL        "FILL" /*fill whole screen background with previously set color(no params)*/

/*END command list*/
typedef struct constNodeObj constNode;

typedef struct constNodeObj{
    char name[CMD_MAX_CONST_SIZE];
    int val;

    constNode* next;
}constNode;

typedef struct constListObj{
    constNode* head;
    constNode* tail;

    int counter;
}constList;

typedef struct scriptVarsObj{
    /*pixel plotting*/
    int x, y;
    color col;
}scriptVars;

typedef struct scriptObj{
    int curLine;
    int curLineChar;

    int numLines;

    scriptVars var;
    constList* cList;

    char** scripts;
}script;



/*command script interpreter for game engine*/
int cgm_scrInit();
void cgm_scrLoad(char* fname);
void cgm_scrUnload();

/*constList functions*/
int cgm_scrAddConstant(char* con, int val);
int cgm_scrRemoveConstant(char* con);

void cgm_scrDeleteConstantList();


/*script parsing functions*/
void cgm_scrGetCommand(char* cmd);
void cgm_scrGetStringParam(char* str);
int cgm_scrGetNumberParam();
/*generic token retrieval (any space separated statement)*/
void cgm_scrGetToken(char* tok);

/*helper functions*/
unsigned char cgm_scrNumberToChar(int num);

void cgm_scrRun();/*main script runner*/



extern script* _scr;

#endif // CGM_SCRIPT_H
