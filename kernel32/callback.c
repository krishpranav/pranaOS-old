//includes
#include <linklist.h>
#include "callback.h"
#include "mm.h"

static struct callback_t *cb_list = NULL;

int register_callback(enum cb_type type,
                      struct time_t *delay, cb_func_t *callback)
{
    struct callback_t *cb =
            (struct callback_t *) kalloc(sizeof(struct callback_t));
    if (!cb)
        return -1;

    cb->reg_time = get_cur_milis();
    cb->delay = time_to_milis(delay);
    cb->callback = callback;
    cb->type = type;

    /* create a new linklist if none exist */
    if (!cb_list)
    {
        llist_init(cb, ll);
        cb_list = cb;
    }
    else
        llist_add_before(cb_list, cb, ll);

    return 0;
}
