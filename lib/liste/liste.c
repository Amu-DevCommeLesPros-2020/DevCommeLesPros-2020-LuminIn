#include "liste.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

node* l_make_node(void* data)
{
    node *n = malloc(sizeof(node));
    n->data = data;
    n->next = NULL;
    n->previous = NULL;

    return n;
}

void l_free_node(node* n)
{
    if(n)
    {
        free(n->data);
        n->data = NULL;
    }
    free(n);
    n = NULL;

    return;
}

void l_free_list(node* n)
{
    for(node *next = n; next;)
    {
        n = next;
        next = n->next;
        l_free_node(n);
    }

    return;
}

int l_length(node const* n)
{
    int i = n ? 1 : 0;
    for(node const* p = n; p && p->next; p = p->next) ++i;
    for(node const* p = n; p && p->previous; p = p->previous) ++i;

    return i;
}

node* l_head(node* n)
{
    for(; n && n->previous; n = n->previous) {}

    return n;
}

node* l_tail(node* n)
{
    for(; n && n->next; n = n->next) {}

    return n;
}

node* l_skip(node* n, int i)
{
    if(i < 0)
    {
        for(; i && n && n->previous; n = n->previous)
        {
             ++i;
        }
    }
    else if(i > 0)
    {
        for(; i && n && n->next; n = n->next)
        {
             --i;
        }
    }

    return i ? NULL : n;
}

node* l_append(node** p, node* tail)
{
    assert(p);
    if(!*p)
    {
        *p = tail;
    }
    else
    {
        node *last = l_tail(*p);
        last->next = tail;
        tail->previous = last;
    }

    return tail;
}

node* l_prepend(node** p, node* head)
{
    assert(p);
    if(!*p)
    {
        *p = head;
    }
    else
    {
        node *first = l_head(*p);
        first->previous = head;
        head->next = first;
    }

    return head;
}

node* l_insert(node** p, node* body)
{
    assert(p);
    if(!*p)
    {
        *p = body;
    }
    else
    {
        body->previous = (*p);
        body->next = (*p)->next;

        (*p)->next = body;
        if(body->next)
        {
            body->next->previous = body;
        }
    }

    return body;
}

node* l_remove(node** n)
{
    node *c = *n;
    node *next = c ? c->next : NULL;

    if(c && c->previous)
    {
        c->previous->next = next;
    }

    if(c && c->next)
    {
        c->next->previous = c->previous;
    }

    l_free_node(c);
    
    return next;
}
