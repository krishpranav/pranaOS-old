/*
Note: To remain compatible with old software, the motherboard emulates USB
keyboads and mice and PS/2 devices
*/

//includes
#include <libc.h>
#include <shell.h>
#include <x86/cpu.h>
#include <x86/i8259.h>

static int kbrd_enable();
static int kbrd_disable();

/*
 * Alphanumeric Keys
 */

#define KEY_SPACE              ' '
#define KEY_0                  '0'
#define KEY_1                  '1'
#define KEY_2                  '2'
#define KEY_3                  '3'
#define KEY_4                  '4'
#define KEY_5                  '5'
#define KEY_6                  '6'
#define KEY_7                  '7'
#define KEY_8                  '8'
#define KEY_9                  '9'

#define KEY_A                  'a'
#define KEY_B                  'b'
#define KEY_C                  'c'
#define KEY_D                  'd'
#define KEY_E                  'e'
#define KEY_F                  'f'
#define KEY_G                  'g'
#define KEY_H                  'h'
#define KEY_I                  'i'
#define KEY_J                  'j'
#define KEY_K                  'k'
#define KEY_L                  'l'
#define KEY_M                  'm'
#define KEY_N                  'n'
#define KEY_O                  'o'
#define KEY_P                  'p'
#define KEY_Q                  'q'
#define KEY_R                  'r'
#define KEY_S                  's'
#define KEY_T                  't'
#define KEY_U                  'u'
#define KEY_V                  'v'
#define KEY_W                  'w'
#define KEY_X                  'x'
#define KEY_Y                  'y'
#define KEY_Z                  'z'

#define KEY_RETURN             '\r'
#define KEY_ESCAPE             0x1001
#define KEY_BACKSPACE          '\b'

/*
 * Arrow Keys
 */

#define KEY_UP                 0x1100
#define KEY_DOWN               0x1101
#define KEY_LEFT               0x1102
#define KEY_RIGHT              0x1103

/*
 * Function Keys
 */

#define KEY_F1                 0x1201
#define KEY_F2                 0x1202
#define KEY_F3                 0x1203
#define KEY_F4                 0x1204
#define KEY_F5                 0x1205
#define KEY_F6                 0x1206
#define KEY_F7                 0x1207
#define KEY_F8                 0x1208
#define KEY_F9                 0x1209
#define KEY_F10                0x120a
#define KEY_F11                0x120b
#define KEY_F12                0x120b
#define KEY_F13                0x120c
#define KEY_F14                0x120d
#define KEY_F15                0x120e

#define KEY_DOT                '.'
#define KEY_COMMA              ','
#define KEY_COLON              ':'
#define KEY_SEMICOLON          ';'
#define KEY_SLASH              '/'
#define KEY_BACKSLASH          '\\'
#define KEY_PLUS               '+'
#define KEY_MINUS              '-'
#define KEY_ASTERISK           '*'
#define KEY_EXCLAMATION        '!'
#define KEY_QUESTION           '?'
#define KEY_QUOTEDOUBLE        '\"'
#define KEY_QUOTE              '\''
#define KEY_EQUAL              '='
#define KEY_HASH               '#'
#define KEY_PERCENT            '%'
#define KEY_AMPERSAND          '&'
#define KEY_UNDERSCORE         '_'
#define KEY_LEFTPARENTHESIS    '('
#define KEY_RIGHTPARENTHESIS   ')'
#define KEY_LEFTBRACKET        '['
#define KEY_RIGHTBRACKET       ']'
#define KEY_LEFTCURL           '{'
#define KEY_RIGHTCURL          '}'
#define KEY_DOLLAR             '$'
#define KEY_POUND              '£'
#define KEY_EURO               '$'
#define KEY_LESS               '<'
#define KEY_GREATER            '>'
#define KEY_BAR                '|'
#define KEY_GRAVE              '`'
#define KEY_TILDE              '~'
#define KEY_AT                 '@'
#define KEY_CARRET             '^'

/*
 * Numeric Keypad
 */

#define KEY_KP_0               '0'
#define KEY_KP_1               '1'
#define KEY_KP_2               '2'
#define KEY_KP_3               '3'
#define KEY_KP_4               '4'
#define KEY_KP_5               '5'
#define KEY_KP_6               '6'
#define KEY_KP_7               '7'
#define KEY_KP_8               '8'
#define KEY_KP_9               '9'
#define KEY_KP_PLUS            '+'
#define KEY_KP_MINUS           '-'
#define KEY_KP_DECIMAL         '.'
#define KEY_KP_DIVIDE          '/'
#define KEY_KP_ASTERISK        '*'
#define KEY_KP_NUMLOCK         0x300f
#define KEY_KP_ENTER           0x3010

#define KEY_TAB                0x4000
#define KEY_CAPSLOCK           0x4001

/*
 * Modify Keys
 */

#define KEY_LSHIFT             0x4002
#define KEY_LCTRL              0x4003
#define KEY_LALT               0x4004
#define KEY_LWIN               0x4005
#define KEY_RSHIFT             0x4006
#define KEY_RCTRL              0x4007
#define KEY_RALT               0x4008
#define KEY_RWIN               0x4009

#define KEY_INSERT             0x400a
#define KEY_DELETE             0x400b
#define KEY_HOME               0x400c
#define KEY_END                0x400d
#define KEY_PAGEUP             0x400e
#define KEY_PAGEDOWN           0x400f
#define KEY_SCROLLLOCK         0x4010
#define KEY_PAUSE              0x4011
#define KEY_PRINT              0x4012

/*
 * Scan codes of the keyboard.
 * In my tests all emulators returned me Set 1 codes, so let's use it here.
 * (Might be because my host is set like that)
 */
static const short SCAN_CODES_SINGLE[] = {
    [0x1]   = KEY_ESCAPE,
    [0x2]   = KEY_1,
    [0x3]   = KEY_2,
    [0x4]   = KEY_3,
    [0x5]   = KEY_4,
    [0x6]   = KEY_5,
    [0x7]   = KEY_6,
    [0x8]   = KEY_7,
    [0x9]   = KEY_8,
    [0xA]   = KEY_9,
    [0xB]   = KEY_0,
    [0xC]   = KEY_MINUS,
    [0xD]   = KEY_EQUAL,
    [0xE]   = KEY_BACKSPACE,
    [0xF]   = KEY_TAB,
    [0x10]  = KEY_Q,
    [0x11]  = KEY_W,
    [0x12]  = KEY_E,
    [0x13]  = KEY_R,
    [0x14]  = KEY_T,
    [0x15]  = KEY_Y,
    [0x16]  = KEY_U,
    [0x17]  = KEY_I,
    [0x18]  = KEY_O,
    [0x19]  = KEY_P,
    [0x1A]  = KEY_LEFTBRACKET,
    [0x1B]  = KEY_RIGHTBRACKET,
    [0x1C]  = KEY_RETURN,
    [0x1D]  = KEY_LCTRL,
    [0x1E]  = KEY_A,
    [0x1F]  = KEY_S,
    [0x20]  = KEY_D,
    [0x21]  = KEY_F,
    [0x22]  = KEY_G,
    [0x23]  = KEY_H,
    [0x24]  = KEY_J,
    [0x25]  = KEY_K,
    [0x26]  = KEY_L,
    [0x27]  = KEY_SEMICOLON,
    [0x28]  = KEY_QUOTE,
    [0x29]  = KEY_GRAVE,
    [0x2A]  = KEY_LSHIFT,
    [0x2B]  = KEY_BACKSLASH,
    [0x2C]  = KEY_Z,
    [0x2D]  = KEY_X,
    [0x2E]  = KEY_C,
    [0x2F]  = KEY_V,
    [0x30]  = KEY_B,
    [0x31]  = KEY_N,
    [0x32]  = KEY_M,
    [0x33]  = KEY_COMMA,
    [0x34]  = KEY_DOT,
    [0x35]  = KEY_SLASH,
    [0x36]  = KEY_RSHIFT,
    [0x37]  = KEY_KP_ASTERISK,
    [0x38]  = KEY_LALT,
    [0x39]  = KEY_SPACE,
    [0x3A]  = KEY_CAPSLOCK,
    [0x3B]  = KEY_F1,
    [0x3C]  = KEY_F2,
    [0x3D]  = KEY_F3,
    [0x3E]  = KEY_F4,
    [0x3F]  = KEY_F5,
    [0x40]  = KEY_F6,
    [0x41]  = KEY_F7,
    [0x42]  = KEY_F8,
    [0x43]  = KEY_F9,
    [0x44]  = KEY_F10,
    [0x45]  = KEY_KP_NUMLOCK,
    [0x46]  = KEY_SCROLLLOCK,
    [0x47]  = KEY_KP_7,
    [0x48]  = KEY_KP_8,
    [0x49]  = KEY_KP_9,
    [0x4A]  = KEY_KP_MINUS,
    [0x4B]  = KEY_KP_4,
    [0x4C]  = KEY_KP_5,
    [0x4D]  = KEY_KP_6,
    [0x4E]  = KEY_KP_PLUS,
    [0x4F]  = KEY_KP_1,
    [0x50]  = KEY_KP_2,
    [0x51]  = KEY_KP_3,
    [0x52]  = KEY_KP_0,
    [0x53]  = KEY_KP_DECIMAL,
    [0x56]  = KEY_LESS
};

static const short SCAN_CODES_MULTI[] = {
    [0x1C]  = KEY_KP_ENTER,
    [0x1D]  = KEY_RCTRL,
    [0x35]  = KEY_KP_DIVIDE,
    [0x37]  = KEY_PRINT,
    [0x38]  = KEY_RALT,
    [0x47]  = KEY_HOME,
    [0x48]  = KEY_UP,
    [0x49]  = KEY_PAGEUP,
    [0x4B]  = KEY_LEFT,
    [0x4D]  = KEY_RIGHT,
    [0x4F]  = KEY_END,
    [0x50]  = KEY_DOWN,
    [0x51]  = KEY_PAGEDOWN,
    [0x52]  = KEY_INSERT,
    [0x53]  = KEY_DELETE
};

static const short SCAN_CODES_BREAK[] = {
    [0xAA]  = KEY_LSHIFT,
    [0xB6]  = KEY_RSHIFT,
    [0xBA]  = KEY_CAPSLOCK,
    [0xD9]  = KEY_LCTRL
};

static const short SCAN_CODES_SYMBOLS[] = {
    [0x2]   = KEY_EXCLAMATION,
    [0x3]   = KEY_AT,
    [0x4]   = KEY_HASH,
    [0x5]   = KEY_DOLLAR,
    [0x6]   = KEY_PERCENT,
    [0x7]   = KEY_CARRET,
    [0x8]   = KEY_AMPERSAND,
    [0x9]   = KEY_ASTERISK,
    [0xA]   = KEY_LEFTPARENTHESIS,
    [0xB]   = KEY_RIGHTPARENTHESIS,
    [0xC]   = KEY_UNDERSCORE,
    [0xD]   = KEY_PLUS,
    [0x1A]  = KEY_LEFTCURL,
    [0x1B]  = KEY_RIGHTCURL,
    [0x27]  = KEY_COLON,
    [0x28]  = KEY_QUOTEDOUBLE,
    [0x29]  = KEY_TILDE,
    [0x2B]  = KEY_BAR,
    [0x33]  = KEY_LESS,
    [0x34]  = KEY_GREATER,
    [0x35]  = KEY_QUESTION,
    [0x56]  = KEY_GREATER
};

