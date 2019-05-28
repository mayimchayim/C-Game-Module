#ifndef CGM_SPRITELIST_H
#define CGM_SPRITELIST_H

#include "cgm_sprite.h"

#include <stdio.h>
#include <stdlib.h>

/*linked list structure of sprites with indexing and updating and removal*/

typedef struct sprNodeObj sprNode;

struct sprNodeObj{
    int index;
    int remove;
    sprite* spr;
    sprNode* next;
};

typedef struct sprListObj{
    int num_values;
    int counter;
    sprNode* head;
    sprNode* tail;
}sprList;

void spl_init(sprList* spl);

int spl_add(sprList* spl, sprite* spr);

int spl_remove(sprList* spl, int i);

void spl_delete(sprList* spl);

int spl_setRemove(sprList* spl, int i);

void spl_update(sprList* spl);

void spl_draw(sprList* spl);
/*sprite list functions*/


#endif // CGM_SPRITELIST_H
