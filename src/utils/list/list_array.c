/* Copyright 2015 Bernd Lehmann (der-b@der-b.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdlib.h>
#include <string.h>
#include <utils/list/_list.h>
#include <utils/list/list_array.h>
#include <utils/overflow.h>

/*
 * _list_array_realloc()
 */
inline LRet _list_array_realloc(List * list)
{
	struct list_array *larray = (struct list_array *) list->data;
        size_t len, len2;
	void *tmp;

	// ensure minimal size
	if (_LIST_ARRAY_MINSIZE > larray->max_count) {

                if (__MUL(len, _LIST_ARRAY_MINSIZE, L_SIZE(list))) {
                        goto err;
                }

		tmp = realloc(larray->data, len);
		if (!tmp) {
                        goto err;
		}
		larray->data = tmp;
		larray->max_count = _LIST_ARRAY_MINSIZE;

        // increase size
	} else if (larray->count >= larray->max_count) {

                if (   __MUL(len,  larray->max_count, 2)
                    || __MUL(len2, len,               L_SIZE(list))) {
                        goto err;
                }

		tmp = realloc(larray->data, len2);
		if (!tmp) {
			goto err;
		}
		larray->data = tmp;
		larray->max_count = len;

        // decrese size but not smaler then _LIST_ARRAY_MINSIZE
	} else if (larray->count < larray->max_count / 4) {

		if (larray->max_count / 2 <= _LIST_ARRAY_MINSIZE) {

                        if (__MUL(len, _LIST_ARRAY_MINSIZE, L_SIZE(list))) {
                                goto err;
                        }

			tmp = realloc(larray->data, len);
                        if (!tmp) {
                                goto err;
                        }

                        larray->data = tmp;
                        larray->max_count = _LIST_ARRAY_MINSIZE;

		} else if (larray->max_count / 2 > _LIST_ARRAY_MINSIZE) {

                        if (__MUL(len, larray->max_count / 2, L_SIZE(list))) {
                                goto err;
                        }

			tmp = realloc(larray->data, len);
                        if (!tmp) {
                                goto err;
                        }

                        larray->data = tmp;
                        larray->max_count = larray->max_count / 2;
		}
	}

	return LIST_OK;
err:
        return LIST_ERR;
}


/*
 * list_array_free()
 */
LRet list_array_free(List * list)
{
	struct list_array *larray;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
                goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
                goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
                goto err;
	}
#endif

	larray = (struct list_array *) list->data;

	free(larray->data);
	free(larray);
        list->data = NULL;
	free(list);

	return LIST_OK;
#if _LIST_ASSUME_NOT_USING_MACROS || _LIST_INVALID_STRUCT_POSSIBLE
err:
        return LIST_ERR;
#endif
}


/*
 * list_array_length
 */
LIndex list_array_length(List * list)
{
	struct list_array *larray;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
                goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
                goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
                goto err;
	}
#endif

	larray = (struct list_array *) list->data;

	return larray->count;
#if _LIST_ASSUME_NOT_USING_MACROS || _LIST_INVALID_STRUCT_POSSIBLE
err:
        return LIST_ERR;
#endif
}


/*
 * list_array_pushback()
 */
void *list_array_pushback(List * list, void *data)
{
	struct list_array *larray;
        size_t offset;
        uint8_t *ptr;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
                goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
                goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
                goto err;
	}
#endif

	larray = (struct list_array *) list->data;

	if (larray->count >= larray->max_count) {
		if (_list_array_realloc(list)) {
                        goto err;
		}
	}

        if (__MUL(offset, larray->count, L_SIZE(list))) {
                goto err;
        }

        ptr = ((uint8_t *) larray->data) + offset;

	if (__INC(larray->count)) {
                goto err;
        }

	if (data) {
		memcpy(ptr, data, L_SIZE(list));
	} else {
		memset(ptr, 0, L_SIZE(list));
	}

	return ptr;
err:
        return NULL;
}


/*
 * list_array_popback()
 */
LRet list_array_popback(List * list, void *data)
{
	struct list_array *larray;
        size_t offset;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
		goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
		goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
		goto err;
	}
#endif

	larray = (struct list_array *) list->data;

	if (0 >= larray->count) {
		return LIST_EMPTY;
	}

	if (__DEC(larray->count)) {
                goto err;
        }

	if (data) {
                if (__MUL(offset, larray->count, L_SIZE(list))) {
                        goto err_count;
                }
		memcpy(data, (((uint8_t *) larray->data) + offset), L_SIZE(list));
	}


	_list_array_realloc(list);

	return LIST_OK;
err_count:
	__INC(larray->count);
err:
        return LIST_ERR;
}


/*
 * list_array_last()
 */
void *list_array_last(List * list)
{
	struct list_array *larray;
        size_t offset;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
		goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
		goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
		goto err;
	}
#endif

	larray = (struct list_array *) list->data;

	if (0 >= larray->count) {
		goto err;
	}

        if (__MUL(offset, (larray->count - 1), L_SIZE(list))) {
                goto err;
        }

	return ((uint8_t *) larray->data) + offset;
err:
        return NULL;
}


/*
 * list_array_get()
 */
void *list_array_get(List * list, LIndex pos)
{
	struct list_array *larray;
        size_t offset;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
		goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
		goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
		goto err;
	}
#endif

	larray = (struct list_array *) list->data;

	if (0 >= larray->count || larray->count <= pos) {
		goto err;
	}

        if (__MUL(offset, pos, L_SIZE(list))) {
                goto err;
        }

	return ((uint8_t *) larray->data) + offset;
err:
        return NULL;
}


/*
 * list_array_insert()
 */
void *list_array_insert(List * list, LIndex pos, void *data)
{
	struct list_array *larray;
	uint8_t *dptr, *stop;
        size_t offset;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
                goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
                goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
                goto err;
	}
#endif

	larray = (struct list_array *) list->data;

	if (pos > larray->count) {
                goto err;
	}

	if (larray->count >= larray->max_count) {
		if (_list_array_realloc(list)) {
                        goto err;
		}
	}

        // offset for stop
        if (__MUL(offset, pos, L_SIZE(list))) {
                goto err;
        }
	stop = ((uint8_t *) larray->data) + offset;

        // offset for basepointer of dptr
        if (__MUL(offset, larray->count, L_SIZE(list))) {
                goto err;
        }
        
        if (__INC(larray->count)) {
                goto err;
        }
	for (dptr = ((uint8_t *) larray->data) + offset;
	     dptr > stop; 
             dptr -= L_SIZE(list)) {

		memcpy(dptr, dptr - L_SIZE(list), L_SIZE(list));
	}

	if (data) {
		memcpy(dptr, data, L_SIZE(list));
	} else {
		memset(dptr, 0, L_SIZE(list));
	}

	return dptr;
err:
        return NULL;
}


/*
 * list_array_remove()
 */
LRet list_array_remove(List * list, LIndex pos, void *data)
{
	struct list_array *larray;
	uint8_t *dptr, *end;
        size_t offset, offset_end;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
                goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
                goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
                goto err;
	}
#endif

	larray = (struct list_array *) list->data;

	if (pos >= larray->count || !larray->count) {
                goto err;
	}

        if (__MUL(offset, pos, L_SIZE(list))) {
                goto err;
        }

	if (data) {
		memcpy(data, ((uint8_t *) larray->data) + offset, L_SIZE(list));
	}

        if (__DEC(larray->count)) {
                goto err;
        }

        if (__MUL(offset_end, larray->count, L_SIZE(list))) {
                goto err_inc;
        }
	end = ((uint8_t *) larray->data) + offset_end;

	for (dptr = ((uint8_t *) larray->data) + offset;
	     dptr < end; dptr += L_SIZE(list)) {
		memcpy(dptr, dptr + L_SIZE(list), L_SIZE(list));
	}

	_list_array_realloc(list);

	return LIST_OK;
err_inc:
        // since decreasing was possible, 
        // increasing also have to be possible
        ++larray->count;
err:
        return LIST_ERR;
}


/*
 * list_array_removelast()
 */
LRet list_array_removelast(List * list, void *del, void *data)
{
	struct list_array *larray;
	uint8_t *dptr, *end, *pos;
        size_t offset;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
                goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
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

	larray = (struct list_array *) list->data;

        if (__DEC(larray->count)) {
                goto err;
        }

        if (__MUL(offset, larray->count, L_SIZE(list))) {
                goto err_inc;
        }

	end = ((uint8_t *) larray->data) + offset;
	pos = end;
	dptr = NULL;

	while (((uint8_t *) larray->data) < pos) {
		pos -= L_SIZE(list);
		if (list->equal(pos, del)) {
			dptr = pos;
			break;
		}
	}

	if (!dptr) {
                goto err_element_not_found;
	}

	if (data) {
		memcpy(data, dptr, L_SIZE(list));
	}

	for (; dptr < end; dptr += L_SIZE(list)) {
		memcpy(dptr, dptr + L_SIZE(list), L_SIZE(list));
	}


	_list_array_realloc(list);

	return LIST_OK;
err_inc:
        // since decreasing was successful
        // increasing also have to be successful
        ++larray->count;
err:
        return LIST_ERR;
err_element_not_found:
        // since decreasing was successful
        // increasing also have to be successful
        ++larray->count;
        return LIST_ELEMENT_NOT_FOUND;
}


/*
 * list_array_first()
 */
void *list_array_first(List * list)
{
	struct list_array *larray;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
                goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
                goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
                goto err;
	}
#endif

	larray = (struct list_array *) list->data;

	if (0 >= larray->count) {
                goto err;
	}

	return larray->data;
err:
        return NULL;
}


/*
 * list_array_pushfront()
 */
void *list_array_pushfront(List * list, void *data)
{
	return list_array_insert(list, 0, data);
}


/*
 * list_array_popfront()
 */
LRet list_array_popfront(List * list, void *data)
{
	struct list_array *larray;

#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
		goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
		goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
		goto err;
	}
#endif

	larray = (struct list_array *) list->data;

	if (0 >= larray->count) {
		return LIST_EMPTY;
	}

	return list_array_remove(list, 0, data);
err:
        return LIST_ERR;
}


/*
 * list_array_iter_free()
 */
LRet list_array_iter_free(LIter * iter)
{
	free(iter->data);

	return LIST_OK;
}


/*
 * list_array_iter_next()
 */
void *list_array_iter_next(LIter * iter)
{
	struct list_array_iter *ait;
        void *ret;
#if _LIST_ASSUME_NOT_USING_MACROS
	if (!iter) {
                goto err;
	}

	if (LIST_TYPE_ARRAY != iter->type) {
                goto err;
        }
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!iter->data) {
                goto err;
	}
#endif

	ait = (struct list_array_iter *) iter->data;

	ret = list_array_get(ait->list, ait->pos);

        if (__INC(ait->pos)) {
                goto err;
        }

        return ret;

err:
        return NULL;
}


/*
 * list_array_iter()
 */
void *list_array_iter(List * list, LIter * iter)
{
	struct list_array_iter *it;

	if (!iter) {
                goto err;
	}
#if _LIST_ASSUME_NOT_USING_MACROS
	if (!list) {
                goto err;
	}

	if (LIST_TYPE_ARRAY != list->type) {
                goto err;
	}
#endif

#if _LIST_INVALID_STRUCT_POSSIBLE
	if (!list->data) {
                goto err;
	}
#endif


	iter->data = calloc(1, sizeof(struct list_array_iter));
	if (!iter->data) {
                goto err;
	}
	it = (struct list_array_iter *) iter->data;
	it->pos = 1;
	it->list = list;

	iter->type = LIST_TYPE_ARRAY;
	iter->next = list_array_iter_next;
	iter->lfree = list_array_iter_free;

	return list_array_get(list, 0);
err:
        return NULL;
}


/*
 * list_array_new()
 */
List *list_array_new(LOpts * opts)
{
	List *list;
	struct list_array *larray;

	if (!opts) {
                goto err;
	}

	if (0 >= opts->size) {
                goto err;
	}

	list = calloc(1, sizeof(List));
	if (!list) {
                goto err;
	}

	larray = calloc(1, sizeof(struct list_array));
	if (!larray) {
                goto err_list;
	}

	memcpy(&list->opts, opts, sizeof(LOpts));

	list->type = LIST_TYPE_ARRAY;
	list->data = larray;

	list->lfree = list_array_free;
	list->length = list_array_length;
	list->insert = list_array_insert;
	list->remove = list_array_remove;
	list->get = list_array_get;
	list->last = list_array_last;
	list->pushback = list_array_pushback;
	list->popback = list_array_popback;
	list->first = list_array_first;
	list->pushfront = list_array_pushfront;
	list->popfront = list_array_popfront;
	list->iter = list_array_iter;
	list->removelast = list_array_removelast;

	return list;
err_list:
        free(list);
err:
        return NULL;
}
