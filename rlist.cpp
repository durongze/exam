#include <stdio.h>
#include <unistd.h>

typedef struct Node
{
    int data;
    struct Node* next;
}NODE;

typedef struct List
{
    NODE* head;
}LIST;

NODE* swap_node(NODE* prev, NODE* cur)
{
    if (NULL == prev)
    {
        return NULL;
    }
    else if (NULL == cur)
    {
        return prev;
    }
    else if (NULL == cur->next)
    {
        cur->next = prev;
        return cur;
    }
    else
    {
        NODE* currr = cur;
        swap_node( cur, cur->next);
        currr->next = prev;
        return currr;
    }
}

void rlist(LIST** list)
{
    if ((*list)->head)
    {
        swap_node((*list)->head, (*list)->head->next);
    }
}

void rrlist(LIST** list)
{
    if (NULL == *list)
    {
        return ;
    }
    else if (NULL == (*list)->head)
    {
        return ;
    }
    else
    {
        NODE* prev = (*list)->head;
        NODE* curr = prev->next;
        NODE* next ;
        (*list)->head->next = NULL;
        while(curr)
        {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        (*list)->head = prev;
    }
}

void append(LIST* list, int data)
{
    NODE* node = list->head;
    if (node == NULL)
    {
        list->head = new NODE;
        list->head->data = data;
        return ;
    }
    while(node->next) 
    {
        node = node->next;
    }
    node->next = new NODE;
    node->next->data = data; 
}

void print(LIST* l)
{
    NODE* node = l->head;
    while(node)
    {
        printf(" %d\n", node->data);
        node = node->next;
    }
}

int main(int argc, char** argv)
{
    LIST l;
    for (int idx = 0; idx < 10; ++idx)
    {
        append(&l, idx);
    }
    LIST * ll = &l;
    print(&l);
    rrlist(&ll);

    sleep(10);
    print(ll);

    return 0;
}
