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
#include <utils/list.h>
#include <utils/list/list_stlinked.h>
#include <utils/overflow.h>
#include <string.h>
#include <assert.h>

/*
 * list_stlinked_last()
 */
void *list_stlinked_last(List * list)
{
    struct list_stlinked *lstl;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    lstl = (struct list_stlinked *) list->data;

    if (!lstl->last) {
        goto err;
    }

    return lstl->last->data;
err:
    return NULL;
}


/*
 * list_stlinked_first()
 */
void *list_stlinked_first(List * list)
{
    struct list_stlinked *lstl;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    lstl = (struct list_stlinked *) list->data;

    if (!lstl->first) {
        goto err;
    }

    return lstl->first->data;
err:
    return NULL;
}


/*
 * list_stlinked_pushfront()
 */
void *list_stlinked_pushfront(List * list, void *data)
{
    struct list_stlinked *lstl;
    struct list_stlinked_item *item;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    lstl = (struct list_stlinked *) list->data;

    if (__INC(lstl->count)) {
        goto err;
    }

    item = calloc(1, sizeof(*item));
    if (!item) {
        goto err_count;
    }

    item->data = malloc(L_DATASIZE(list));
    if (!item->data) {
        goto err_item;
    }

    if (data) {
        memcpy(item->data, data, L_DATASIZE(list));
    } else {
        memset(item->data, 0, L_DATASIZE(list));
    }

    if (NULL == lstl->last) {
#if _LIST_INVALID_STRUCT_POSSIBLE
        if (lstl->first) {
            goto err_item_data;
        }
#endif

        lstl->first = item;
        lstl->last = item;
    } else {
        lstl->first->prev = item;
        lstl->first = item;
    }


    return item->data;
#if _LIST_INVALID_STRUCT_POSSIBLE
err_item_data:
    free(item->data);
#endif
err_item:
    free(item);
err_count:
    // should always work, since increas was possible
    --lstl->count;
err:
    return NULL;
}


/*
 * list_stlinked_pushback()
 */
void *list_stlinked_pushback(List * list, void *data)
{
    struct list_stlinked *lstl;
    struct list_stlinked_item *item;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    lstl = (struct list_stlinked *) list->data;

    if (__INC(lstl->count)) {
        goto err;
    }

    item = calloc(1, sizeof(*item));
    if (!item) {
        goto err_count;
    }

    item->data = malloc(L_DATASIZE(list));
    if (!item->data) {
        goto err_item;
    }

    if (data) {
        memcpy(item->data, data, L_DATASIZE(list));
    } else {
        memset(item->data, 0, L_DATASIZE(list));
    }

    if (NULL == lstl->last) {
#if _LIST_INVALID_STRUCT_POSSIBLE
        if (lstl->first) {
            goto err_item_data;
        }
#endif

        lstl->first = item;
        lstl->last = item;
    } else {
        item->prev = lstl->last;
        lstl->last = item;
    }


    return item->data;
#if _LIST_INVALID_STRUCT_POSSIBLE
err_item_data:
    free(item->data);
#endif
err_item:
    free(item);
err_count:
    // should work since inc was possible
    --lstl->count;
err:
    return NULL;
}


/*
 * list_stlinked_size(LIst *llist)
 */
LIndex list_stlinked_size(List * list)
{
    struct list_stlinked *lstl;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err:
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err:
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err:
    }
#endif

    lstl = (struct list_stlinked *) list->data;
    return lstl->count;
#if _LIST_ASUME_NOT_USING_MACROS || _LIST_INVALID_STRUCT_POSSIBLE
err:
    return LIST_ERR;
#endif
}


/*
 * list_stlinked_insert()
 */
void *list_stlinked_insert(List * list, LIndex pos, void *data)
{
    struct list_stlinked *lst;
    struct list_stlinked_item *item;
    struct list_stlinked_item *new_item;
    LIndex i;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    lst = (struct list_stlinked *) list->data;

    if (0 > pos || lst->count <= pos) {
        goto err;
    }

    if (0 == pos) {
        return list_stlinked_pushfront(list, data);
    } else if (lst->count == pos) {
        return list_stlinked_pushback(list, data);
    } else {

        for (i = lst->count - 1, item = lst->last;
                item->prev && pos < i; --i, item = item->prev);


#if _LIST_INVALID_STRUCT_POSSIBLE
        if (!item) {
            goto err;
        }
#endif

        new_item = calloc(1, sizeof(*new_item));
        if (!new_item) {
            goto err;
        }

        new_item->data = malloc(L_DATASIZE(list));
        if (!new_item->data) {
            goto err_new_item;
        }

        if (__INC(lst->count)) {
            goto err_new_item_data;
        }

        if (data) {
            memcpy(new_item->data, data, L_DATASIZE(list));
        } else {
            memset(new_item->data, 0, L_DATASIZE(list));
        }

        new_item->prev = item->prev;
        item->prev = new_item;
        return new_item->data;
    }

err_new_item_data:
    free(new_item->data);
err_new_item:
    free(new_item);
err:
    return NULL;

}


/*
 * list_stlinked_get()
 */
void *list_stlinked_get(List * list, LIndex pos)
{
    struct list_stlinked *lstlinked;
    struct list_stlinked_item *item;
    LIndex i;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    lstlinked = (struct list_stlinked *) list->data;

    if (0 >= lstlinked->count || lstlinked->count <= pos) {
        goto err;
    }

    for (i = lstlinked->count - 1, item = lstlinked->last;
            item->prev && pos < i; --i, item = item->prev);

    return item->data;
err:
    return NULL;
}


/*
 * list_stlinked_iter_next()
 */
void *list_stlinked_iter_next(LIter * iter)
{
    struct list_stlinked_iter *it;
#if _LIST_ASUME_NOT_USING_MACROS
    if (!iter) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != iter->type) {
        goto err;
    }
#endif

    if (!iter->data) {
        goto err;
    }

    it = (struct list_stlinked_iter *) iter->data;

    if (__INC(it->i)) {
        goto err;
    }

    return list_stlinked_get(it->list, it->i);
err:
    return NULL;
}


/*
 * list_stlinked_iter_free()
 */
LRet list_stlinked_iter_free(LIter * iter)
{
#if _LIST_ASUME_NOT_USING_MACROS
    if (!iter) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != iter->type) {
        goto err;
    }
#endif

    free(iter->data);
    iter->data = NULL;

    return LIST_OK;
#if _LIST_ASUME_NOT_USING_MACROS
err:
    return LIST_ERR;
#endif
}


/*
 * list_stlinked_iter()
 */
void *list_stlinked_iter(List * list, LIter * iter)
{

    if (!iter) {
        goto err;
    }

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    memset(iter, 0, sizeof(LIter));

    iter->data = malloc(sizeof(struct list_stlinked_iter));
    if (!iter->data) {
        goto err;
    }

    ((struct list_stlinked_iter *) iter->data)->i = 0;
    ((struct list_stlinked_iter *) iter->data)->list = list;

    iter->type = LIST_TYPE_STLINKED;
    iter->next = list_stlinked_iter_next;
    iter->lfree = list_stlinked_iter_free;

    return list_stlinked_get(list, 0);
err:
    return NULL;
}


/*
 * list_stlinked_popfront()
 */
LRet list_stlinked_popfront(List * list, void *data)
{
    struct list_stlinked *lstl;
    struct list_stlinked_item *item;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    lstl = (struct list_stlinked *) list->data;

    if (!lstl->last) {
        return LIST_EMPTY;
    }

    for (item = lstl->last; item->prev && item->prev->prev;
            item = item->prev);

    if (__DEC(lstl->count)) {
        goto err;
    }

    if (data) {
        memcpy(data, item->prev->data, L_DATASIZE(list));
    }
    free(item->prev->data);
    free(item->prev);
    item->prev = NULL;
    lstl->first = item;

    return LIST_OK;
err:
    return LIST_ERR;
}


/*
 * list_stlinked_popback()
 */
LRet list_stlinked_popback(List * list, void *data)
{
    struct list_stlinked *lstl;
    struct list_stlinked_item *item;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    lstl = (struct list_stlinked *) list->data;

    item = lstl->last;

    if (!item) {
        return LIST_EMPTY;
    }

    lstl->last = item->prev;

    if (__DEC(lstl->count)) {
        return LIST_EMPTY;
    }

    if (data) {
        memcpy(data, item->data, L_DATASIZE(list));
    }
    free(item->data);
    free(item);

    return LIST_OK;
err:
    return LIST_ERR;
}


/*
 * list_stlinked_remove()
 */
LRet list_stlinked_remove(List * list, LIndex pos, void *data)
{
    struct list_stlinked *lstl;
    struct list_stlinked_item *item, *tmp;
    LIndex i;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    lstl = (struct list_stlinked *) list->data;

    if (0 > pos || lstl->count <= pos) {
        goto err;
    }

    if (0 == pos) {
        return list_stlinked_popfront(list, data);
    } else if (lstl->count - 1 == pos) {
        return list_stlinked_popback(list, data);
    } else {
        for (i = lstl->count - 1, item = lstl->last;
                lstl && pos + 1 < i; item = item->prev, i--);

        tmp = item->prev;
        item->prev = item->prev->prev;

        if (__DEC(lstl->count)) {
            goto err;
        }

        if (data) {
            memcpy(data, tmp->data, L_DATASIZE(list));
        }
        free(tmp->data);
        free(tmp);
    }

    return LIST_OK;
err:
    return LIST_ERR;
}


/*
 * list_stlinked_removelast()
 */
LRet list_stlinked_removelast(List * list, void *del, void *data)
{
    struct list_stlinked *lstl;
    struct list_stlinked_item *item, *tmp;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
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

    lstl = (struct list_stlinked *) list->data;
    item = lstl->last;
    tmp = NULL;

    while (item && !list->equal(item->data, del)) {
        tmp = item;
        item = item->prev;
    }

    if (item) {
        if (__DEC(lstl->count)) {
            goto err;
        }
        if (tmp) {
            tmp->prev = item->prev;
        } else {
            lstl->last = item->prev;
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
 * list_stlinked_clear()
 */
LRet list_stlinked_clear(List * list)
{
    struct list_stlinked *lstl;
    struct list_stlinked_item *item, *tmp;

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif

    lstl = (struct list_stlinked *) list->data;

    item = lstl->last;
    while (item) {
        tmp = item;
        item = item->prev;
        free(tmp->data);
        free(tmp);
    }

    lstl->first = NULL;
    lstl->last = NULL;
    lstl->count = 0;

    return LIST_OK;
#if _LIST_ASUME_NOT_USING_MACROS || _LIST_INVALID_STRUCT_POSSIBLE
err:
    return LIST_ERR:
#endif
}


/*
 * list_stlinked_free()
 */
LRet list_stlinked_free(List * list)
{

#if _LIST_ASUME_NOT_USING_MACROS
    if (!list) {
        goto err;
    }

    if (LIST_TYPE_STLINKED != list->type) {
        goto err;
    }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
    if (!list->data) {
        goto err;
    }
#endif
    if (list_stlinked_clear(list)) {
        goto err;
    }

    free(list->data);
    free(list);

    return LIST_OK;
err:
    return LIST_ERR;
}


/*
 * list_stlinked_new();
 */
List *list_stlinked_new(LOpts * opts)
{
    List *list;
    struct list_stlinked *lstl;

    if (!opts) {
        goto err;
    }

    if (0 >= opts->datasize) {
        goto err;
    }

    lstl = calloc(1, sizeof(*lstl));
    if (!lstl) {
        goto err;
    }

    list = calloc(1, sizeof(*list));
    if (!list) {
        goto err_lstl;
    }

    memcpy(&list->opts, opts, sizeof(LOpts));

    list->type = LIST_TYPE_STLINKED;
    list->data = lstl;

    list->lfree = list_stlinked_free;
    list->size = list_stlinked_size;
    list->insert = list_stlinked_insert;
    list->remove = list_stlinked_remove;
    list->get = list_stlinked_get;
    list->last = list_stlinked_last;
    list->pushback = list_stlinked_pushback;
    list->popback = list_stlinked_popback;
    list->first = list_stlinked_first;
    list->pushfront = list_stlinked_pushfront;
    list->popfront = list_stlinked_popfront;
    list->iter = list_stlinked_iter;
    list->removelast = list_stlinked_removelast;
    list->clear = list_stlinked_clear;

    return list;
err_lstl:
    free(lstl);
err:
    return NULL;
}
