#include <libc.h>
#include <error.h>
#include <callback.h>
#include <mm.h>
#include "shell.h"

extern int info_main(int argc, const char *argv[]);

#define PROMPT_SIZE 30

/* Default colors */
#define SHELL_CLR_BG VID_CLR_BLACK
#define SHELL_CLR_FG VID_CLR_LIGHT_GRAY
#define SHELL_HEAD_CLR_BG VID_CLR_DARK_GRAY
#define SHELL_HEAD_CLR_FG VID_CLR_LIGHT_RED
#define SHELL_FOOT_CLR_BG VID_CLR_DARK_GRAY
#define SHELL_FOOT_CLR_FG VID_CLR_LIGHT_RED
#define SHELL_PROMPT_BG SHELL_CLR_BG
#define SHELL_PROMPT_FG VID_CLR_WHITE

struct shell_t {
    char *prompt;
    char *cmd_buf;
    struct frame_t frame;
};

static struct shell_t shell;

static int set_prompt(char *prmpt)
{
    if (strlen(prmpt) > PROMPT_SIZE)
    {
        error = ESIZE
        return -error;
    }

    shell.prompt = prmpt;

    return 0;
}

static void prompt_draw()
{
    color_save();
    set_color(SHELL_PROMPT_BG, SHELL_PROMPT_FG);
    printf("%s", shell.prompt);
    color_load();
}

static void clock_redraw()
{
    cursor_save();
    set_color(SHELL_HEAD_CLR_BG, SHELL_HEAD_CLR_FG);
    goto_xy(71, 0);
    if (hw_time.hour < 10)
        putchar('0');
    printf("%d:", hw_time.hour);
    if (hw_time.min < 10)
        putchar('0');
    printf("%d:", hw_time.min);
    if (hw_time.sec < 10)
        putchar('0');
    printf("%d", hw_time.sec);
    cursor_load();
}