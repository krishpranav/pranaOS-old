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

void clear_screen()
{
    int i;
    int max_offset = MAX_CRS_X * MAX_CRS_Y;

    for (i = 0; i < max_offset; i++)
    {
        short mem_val = CHAR_TO_MEMVAL(' ');
        ((short *)VIDEO_MEMORY)[i] = mem_val;
    }
}

void set_color(unsigned char backgrnd, unsigned char forgrnd)
{
    _color = (forgrnd | (backgrnd << 4));
}

void goto_xy(unsigned x, unsigned y)
{
    _cursor_loc = MAX_CRS_X * y + x;
}

void cursor_save()
{
    _cursor_save = _cursor_loc;
    _color_save = _color;
}
