/* Copyright 2015 Bernd Lehmann (der-b@der-b.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <utils/list/_list.h>
#include <utils/overflow.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/*
 * list_dlinked_pushfront()
 */
void *list_dlinked_pushfront(List * list, void *data)
{
    struct list_dlinked *ldl;
    struct list_dlinked_item *i;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif
    ldl = (struct list_dlinked *) list->data;

    i = calloc(1, sizeof(*i));
    if (!i) {
        goto err;
    }

    i->data = malloc(L_DATASIZE(list));
    if (!i->data) {
        goto err_i;
    }

    if (data) {
        memcpy(i->data, data, L_DATASIZE(list));
    } else {
        memset(i->data, 0, L_DATASIZE(list));
    }

    if (!ldl->first) {
#if _LIST_INVALID_STRUCT_POSSIBLE
        if (ldl->last) {
            goto err_i_data;
        }
#endif
        ldl->first = i;
        ldl->last = i;
    } else {
        i->next = ldl->first;
        ldl->first->prev = i;
        ldl->first = i;
    }

    if (__INC(ldl->count)) {
        goto err_i_data;
    }

    return i->data;
err_i_data:
    free(i->data);
err_i:
    free(i);
err:
    return NULL;
}


/*
 * list_dlinked_pushback()
 */
void *list_dlinked_pushback(List * list, void *data)
{
    struct list_dlinked *ldl;
    struct list_dlinked_item *i;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif
    ldl = (struct list_dlinked *) list->data;

    i = calloc(1, sizeof(*i));
    if (!i) {
        goto err;
    }

    i->data = malloc(L_DATASIZE(list));
    if (!i->data) {
        goto err_i;
    }

    if (data) {
        memcpy(i->data, data, L_DATASIZE(list));
    } else {
        memset(i->data, 0, L_DATASIZE(list));
    }

    if (!ldl->last) {
#if _LIST_INVALID_STRUCT_POSSIBLE
        if (ldl->first) {
            goto err_i_data;
        }
#endif
        ldl->first = i;
        ldl->last = i;
    } else {
        i->prev = ldl->last;
        ldl->last->next = i;
        ldl->last = i;
    }

    if (__INC(ldl->count)) {
        goto err_i_data;
    }

    return i->data;
err_i_data:
    free(i->data);
err_i:
    free(i);
err:
    return NULL;
}


/*
 * list_dlinked_popfront()
 */
LRet list_dlinked_popfront(List * list, void *data)
{
    struct list_dlinked *ldl;
    struct list_dlinked_item *tmp;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    ldl = (struct list_dlinked *) list->data;

    if (!ldl->first) {
        return LIST_EMPTY;
    }

    if(__DEC(ldl->count)) {
        goto err;
    }

    tmp = ldl->first;

    ldl->first = tmp->next;

    if (ldl->first) {
        ldl->first->prev = NULL;
    } else {
        ldl->last = NULL;
    }


    if (data) {
        memcpy(data, tmp->data, L_DATASIZE(list));
    }

    free(tmp->data);
    free(tmp);

    return LIST_OK;
err:
    return LIST_ERR;
}


/*
 * list_dlinked_popback()
 */
LRet list_dlinked_popback(List * list, void *data)
{
    struct list_dlinked *ldl;
    struct list_dlinked_item *tmp;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    ldl = (struct list_dlinked *) list->data;

    if (!ldl->last) {
        return LIST_EMPTY;
    }

    if (__DEC(ldl->count)) {
        goto err;
    }
    tmp = ldl->last;

    if (!tmp) {
        goto err;
    }

    ldl->last = tmp->prev;

    if (ldl->last) {
        ldl->last->next = NULL;
    }

    if (data) {
        memcpy(data, tmp->data, L_DATASIZE(list));
    }
    free(tmp->data);
    free(tmp);

    return LIST_OK;
err:
    return LIST_ERR;

}


/*
 * list_dlinked_size()
 */
LIndex list_dlinked_size(List * list)
{
    struct list_dlinked *ldl;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    ldl = (struct list_dlinked *) list->data;

    return ldl->count;
#if _LIST_INVALID_STRUCT_POSSIBLE || _LIST_ASUME_NOT_USING_MACROS
err:
    return LIST_ERR;
#endif
}


/*
 * list_dlinked_insert()
 */
void *list_dlinked_insert(List * list, LIndex pos, void *data)
{
    struct list_dlinked *ldl;
    struct list_dlinked_item *item;
    struct list_dlinked_item *new_item;
    int i;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    ldl = (struct list_dlinked *) list->data;

    if (0 > pos || ldl->count < pos) {
        goto err;
    }

    if (0 == pos) {
        return list_dlinked_pushfront(list, data);
    } else if (ldl->count == pos) {
        return list_dlinked_pushback(list, data);
    } else {
        if (__INC(ldl->count)) {
            goto err;
        }

        for (i = 0, item = ldl->first; i < pos; i++) {
            item = item->next;
        }

        new_item = calloc(1, sizeof(*new_item));
        if (!new_item) {
            goto err_dec;
        }

        new_item->data = malloc(L_DATASIZE(list));
        if (!new_item->data) {
            goto err_new_item;
        }
        memcpy(new_item->data, data, L_DATASIZE(list));

        new_item->next = item;
        new_item->prev = item->prev;
        item->prev->next = new_item;
        item->prev = new_item;
        return new_item->data;
    }
err_new_item:
    free(new_item);
err_dec:
    // should work, since increase was succsessful
    --ldl->count;
err:
    return NULL;

}


/*
 * list_dlinked_remove()
 */
LRet list_dlinked_remove(List * list, LIndex pos, void *data)
{
    struct list_dlinked *ldl;
    struct list_dlinked_item *item;
    int i;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    ldl = (struct list_dlinked *) list->data;

    if (0 > pos || ldl->count < pos) {
        goto err;
    }

    if (0 == pos) {
        return list_dlinked_popfront(list, data);
    } else if (0 < ldl->count && ldl->count - 1 == pos) {
        return list_dlinked_popback(list, data);
    } else {
        if (__DEC(ldl->count)) {
            goto err;
        }

        for (i = 0, item = ldl->first; i < pos; i++) {
            item = item->next;
        }

        item->next->prev = item->prev;
        item->prev->next = item->next;
        if (data) {
            memcpy(data, item->data, L_DATASIZE(list));
        }
        free(item->data);
        free(item);
        return LIST_OK;
    }

err:
    return LIST_ERR;
}


/*
 * list_dlinked_removelast()
 */
LRet list_dlinked_removelast(List * list, void *del, void *data)
{
    struct list_dlinked *ldl;
    struct list_dlinked_item *item;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    if (!list->equal) {
        goto err;
    }

    ldl = (struct list_dlinked *) list->data;
    item = ldl->last;

    while (item && !list->equal(item->data, del)) {
        item = item->prev;
    }

    if (item) {
        if (__DEC(ldl->count)) {
            goto err;
        }

        if (item->prev) {
            item->prev->next = item->next;
        } else {
            ldl->first = item->next;
        }

        if (item->next) {
            item->next->prev = item->prev;
        } else {
            ldl->last = item->prev;
        }

        if (data) {
            memcpy(data, item->data, L_DATASIZE(list));
        }
        free(item->data);
        free(item);
    } else {
        return LIST_ELEMENT_NOT_FOUND;
    }

    return LIST_OK;
err:
    return LIST_ERR;
}


/*
 * list_dlinked_clear()
 */
LRet list_dlinked_clear(List * list)
{
    struct list_dlinked *ldl;
    struct list_dlinked_item *item, *tmp;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    ldl = (struct list_dlinked *) list->data;
    item = ldl->last;

    while (item) {
        tmp = item;
        item = item->prev;
        free(tmp->data);
        free(tmp);
    }

    ldl->first = NULL;
    ldl->last = NULL;
    ldl->count = 0;

    return LIST_OK;
err:
    return LIST_ERR;
}


/*
 * list_dlinked_get()
 */
void *list_dlinked_get(List * list, LIndex pos)
{
    struct list_dlinked *ldl;
    struct list_dlinked_item *item;
    LIndex i;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    ldl = (struct list_dlinked *) list->data;

    if (0 > pos || ldl->count < pos) {
        goto err;
    }

    if (0 == pos) {
        return ldl->first->data;
    } else if (0 < ldl->count && ldl->count - 1 == pos) {
        return ldl->last->data;
    } else {
        for (i = 0, item = ldl->first; i < pos; i++) {
            item = item->next;
        }

        return item->data;
    }

err:
    return NULL;
}


/*
 * list_dlinked_last()
 */
void *list_dlinked_last(List * list)
{
    struct list_dlinked *ldl;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    ldl = (struct list_dlinked *) list->data;

    if (!ldl->last) {
        goto err;
    }

    return ldl->last->data;
err:
    return NULL;
}


/*
 * list_dlinked_first()
 */
void *list_dlinked_first(List * list)
{
    struct list_dlinked *ldl;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    ldl = (struct list_dlinked *) list->data;

    if (!ldl->first) {
        goto err;
    }

    return ldl->first->data;
err:
    return NULL;
}


/*
 * list_dlinked_iter_next()
 */
void *list_dlinked_iter_next(LIter * iter)
{
    struct list_dlinked_item *item;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!iter) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != iter->type) {
        goto err;
    }
#endif

    if (!iter->data) {
        goto err;
    }

    item = (struct list_dlinked_item *) iter->data;

    iter->data = item->next;
    item = item->next;

    if (!item) {
        goto err;
    }

    return item->data;
err:
    return NULL;
}


/*
 * list_dlinked_iter_free()
 */
LRet list_dlinked_iter_free(LIter * iter)
{
    // nothing to do here
    return LIST_OK;
}

/*
 * list_dlinked_iter()
 */
void *list_dlinked_iter(List * list, LIter * iter)
{
    struct list_dlinked *ldl;

    if (!iter) {
        goto err;
    }
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    ldl = (struct list_dlinked *) list->data;

    memset(iter, 0, sizeof(LIter));

    if (!ldl->first) {
        goto err;
    }

    iter->type = LIST_TYPE_DLINKED;
    iter->data = ldl->first;
    iter->next = list_dlinked_iter_next;
    iter->lfree = list_dlinked_iter_free;

    return ldl->first->data;
err:
    return NULL;
}


/* 
 * list_dlinked_destroy()
 */
LRet list_dlinked_destroy(List * list)
{
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    if (list_dlinked_clear(list)) {
        goto err;
    }

    free(list->data);

    return LIST_OK;
err:
    return LIST_ERR;
}


/* 
 * list_dlinked_free()
 */
LRet list_dlinked_free(List * list)
{
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_DLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    if (list_dlinked_destroy(list)) {
        goto err;
    }

    free(list);

    return LIST_OK;
err:
    return LIST_ERR;
}


/*
 * list_dlinked_init()
 */
LRet list_dlinked_init(List *list, LOpts *opts)
{
    struct list_dlinked *ldl;

    if (!list) {
        goto err;
    }

    if (!opts) {
        goto err;
    }

    if (0 == opts->datasize) {
        goto err;
    }

    /* new dlinked list structure */
    ldl = calloc(1, sizeof(*ldl));
    if (!ldl) {
        goto err;
    }

    memcpy(&list->opts, opts, sizeof(LOpts));
    list->data = ldl;

    list->type = LIST_TYPE_DLINKED;

    list->lfree = list_dlinked_free;
    list->ldestroy = list_dlinked_destroy;
    list->size = list_dlinked_size;
    list->insert = list_dlinked_insert;
    list->remove = list_dlinked_remove;
    list->get = list_dlinked_get;
    list->last = list_dlinked_last;
    list->pushback = list_dlinked_pushback;
    list->popback = list_dlinked_popback;
    list->first = list_dlinked_first;
    list->pushfront = list_dlinked_pushfront;
    list->popfront = list_dlinked_popfront;
    list->iter = list_dlinked_iter;
    list->removelast = list_dlinked_removelast;
    list->clear = list_dlinked_clear;

    return LIST_OK;
err:
    return LIST_ERR;
}


/*
 * list_dlinked_new()
 */
List *list_dlinked_new(LOpts * opts)
{
    List *list;

    /* new list structure */
    list = calloc(1, sizeof(List));
    if (!list) {
        goto err;
    }

    if (list_dlinked_init(list, opts)) {
        goto err_list;
    }

    return list;
err_list:
    free(list);
err:
    return NULL;
}
