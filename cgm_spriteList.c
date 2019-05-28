#include "cgm_spriteList.h"


void spl_init(sprList* spl)
{
    spl->num_values = 0;
    spl->counter = 0;
    spl->head = NULL;
    spl->tail = NULL;
}

int spl_add(sprList* spl, sprite* spr)
{
    sprNode* new_node;

    new_node = (sprNode*)malloc(sizeof(sprNode));
    if(new_node == NULL){
        printf("\nspl_add error: Unable to initialize new sprite");
        return -1;
    }

    new_node->spr = spr;
    new_node->next = NULL;

    /*check for empty list*/
    if(spl->head == NULL){
        spl->head = spl->tail = new_node;
    }
    else if(spl->head == spl->tail){ /*one element*/
        spl->head->next = new_node;
        spl->tail = new_node;
    }else{ /*2 or more elements*/
        spl->tail->next = new_node;
        spl->tail = new_node;
    }

    new_node->remove = 0;
    new_node->index = spl->counter;

    spl->num_values += 1;
    spl->counter += 1;

    printf("\nNode: %d added", new_node->index);

    return new_node->index;
}

int spl_remove(sprList* spl, int i)
{
    sprNode* cur;
    sprNode* prev;

    cur = spl->head;
    prev = spl->head;

    while(cur->index != i && cur != NULL){
        prev = cur;
        cur = cur->next;
    }
    if(cur == NULL){
        /*end of list, no node found*/
        return -1;
    }

    /*one elem list*/
    if(spl->head == spl->tail){
        spr_close(spl->head->spr);
        free(spl->head);
        spl->head = spl->tail = NULL;
    }
    else if(cur == spl->head){/*front of list*/
        spl->head = spl->head->next;
        spr_close(cur->spr);
        free(cur);
        cur = NULL;
    }
    else if(cur == spl->tail){ /*end of list*/
        prev->next = NULL;
        spr_close(cur->spr);
        free(cur);
        cur = NULL;
        spl->tail = prev;
    }
    else{ /*middle of list*/
        prev->next = cur->next;
        spr_close(cur->spr);
        free(cur);
        cur = NULL;
    }

    spl->num_values -= 1;

    return i;
}

void spl_delete(sprList* spl)
{
    sprNode* cur = spl->head;
    sprNode* del = spl->head;

    while(cur != NULL){
        del = cur;
        cur = cur->next;
        spr_close(del->spr);
        free(del);
        del = NULL;
    }
}

int spl_setRemove(sprList* spl, int i)
{
    sprNode* cur = spl->head;

    while(cur != NULL && cur->index != i){
        cur = cur->next;
    }
    if(cur == NULL){
        return -1;
    }
    cur->remove = 1;

    return cur->index;
}

void spl_update(sprList* spl)
{
    sprNode* cur = spl->head;
    sprNode* del = spl->head;
    /*loop through list checking removal of sprites*/
    while(cur != NULL){
        del = cur;
        cur = cur->next;
        if(del->remove){
            spl_remove(spl, del->index);
        }
    }

    /*reloop updating position*/
    cur = spl->head;
    del = spl->head;

    while(cur != NULL){
        spr_update(cur->spr);
        cur = cur->next;
    }
}

void spl_draw(sprList* spl)
{
    sprNode* cur = spl->head;

    while(cur != NULL){
        spr_show(cur->spr);
        cur = cur->next;
    }
}
/*sprite list functions*/

