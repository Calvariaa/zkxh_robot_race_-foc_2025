#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "stm32g4xx_hal.h"

#define screen_show_string(_x, _y, _chr) do { \
ssd1306_SetCursor((_x), 10 * (_y)); \
ssd1306_WriteString((_chr), Font_7x10); \
} while (0);

#define screen_show_int(_x, _y, _num, _) do { \
static char _chr[128]; \
sprintf(_chr, "%ld ", ((int32_t)_num)); \
ssd1306_SetCursor((_x), 10 * (_y)); \
ssd1306_WriteString((_chr), Font_7x10); \
} while (0);


#define screen_show_float(_x, _y, _num, _int, _float) do { \
ssd1306_SetCursor((_x), 10 * (_y)); \
ssd1306_WriteFloat((_num), _int, _float, Font_7x10); \
} while (0);

#define screen_clear ssd1306_Clear

#define screen_show_uint8 screen_show_int
#define screen_show_uint16 screen_show_int
#define screen_show_uint32 screen_show_int
#define screen_show_int8 screen_show_int
#define screen_show_int16 screen_show_int
#define screen_show_int32 screen_show_int

#define screen_set_white() ssd1306_SetColor(White)
#define screen_set_black() ssd1306_SetColor(Black)

typedef struct MENU {
  char *name;

  void (*func)();
} M_SELECT;

// 数据名称，数据指针，数据更新步长，数据类型
typedef struct OPTION {
  char *name;
  void *value;
  float step;

  enum type_e {
    O_BOOL,
    O_UINT8,
    O_UINT16,
    O_UINT32,
    O_INT8,
    O_INT16,
    O_INT32,
    O_FLOAT,
    O_TYPE_NUM
  } type;
} O_SELECT;

extern void (*current_func)();

void interface_main();

void display_main();

#endif
