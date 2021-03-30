#include "libc.h

static int _cursor_loc = 0;
static char _color = 0;

static int _cursor_save = 0;
static char _color_save = 0;

static struct frame_t *cur_frame = NULL;

static int cursor_move(int cnt);
static int cursor_move_line(int cnt);
static int _puts(const char *text);
static inline void put_tab();

#define CHAR_TO_MEMVAL(chr) \
        ((chr) | (_color << 8))