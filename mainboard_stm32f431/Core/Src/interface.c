#include "interface.h"

#include <stdio.h>

#include "stdbool.h"
#include "ssd1306.h"

uint8_t pages = 0;

char chr[256];

int test = 0;
bool key_short_press(void) {
  return test;
}

bool key_long_press(void) {
  return test;
}

// 总按键调参
void edit_all_option(O_SELECT *select_opt, const uint8_t select_opt_num) {
  static uint8_t main_options = 0;
  uint8_t last_option;
  uint8_t i;

  // 换页
  if (key_short_press()) {
    last_option = main_options;
    if (main_options-- == 0)
      main_options = select_opt_num - 1;

    screen_show_string(0, (last_option + 1), "  ");
  }
  if (key_short_press()) {
    last_option = main_options;
    if (++main_options >= select_opt_num)
      main_options = 0;

    screen_show_string(0, (last_option + 1), "  ");
  }

  // 显示级联主菜单与选项指针
  for (i = 0; i < select_opt_num; i++) {
    if (main_options == i)
      screen_show_string(0, (i + 1), "->");

    screen_show_string(16, (i + 1), select_opt[i].name);
    switch (select_opt[i].type) {
      case O_BOOL:
        if (main_options == i) {
          if (key_short_press())
            (*(bool *) select_opt[i].value) = 0;
          if (key_short_press())
            (*(bool *) select_opt[i].value) = 1;
        }

        screen_show_string(70, (i + 1), *(bool *)select_opt[i].value == 1 ? "1 " : "0 ");

        break;
      case O_UINT8:
        if (main_options == i) {
          if (key_short_press() || key_long_press())
            (*(uint8_t *) select_opt[i].value) -= select_opt[i].step;
          if (key_short_press() || key_long_press())
            (*(uint8_t *) select_opt[i].value) += select_opt[i].step;
        }

        screen_show_uint8(70, (i + 1), *(uint8_t *)select_opt[i].value, 4);

        break;
      case O_UINT16:
        if (main_options == i) {
          if (key_short_press() || key_long_press())
            (*(uint16_t *) select_opt[i].value) -= select_opt[i].step;
          if (key_short_press() || key_long_press())
            (*(uint16_t *) select_opt[i].value) += select_opt[i].step;
        }

        screen_show_uint16(70, (i + 1), *(uint16_t *)select_opt[i].value, 6);

        break;
      case O_UINT32:
        if (main_options == i) {
          if (key_short_press() || key_long_press())
            (*(uint32_t *) select_opt[i].value) -= select_opt[i].step;
          if (key_short_press() || key_long_press())
            (*(uint32_t *) select_opt[i].value) += select_opt[i].step;
        }

        screen_show_uint32(70, (i + 1), *(uint32_t *)select_opt[i].value, 10);

        break;
      case O_INT8:
        if (main_options == i) {
          if (key_short_press() || key_long_press())
            (*(int8_t *) select_opt[i].value) -= select_opt[i].step;
          if (key_short_press() || key_long_press())
            (*(int8_t *) select_opt[i].value) += select_opt[i].step;
        }

        screen_show_int8(70, (i + 1), *(int8_t *)select_opt[i].value, 4);

        break;
      case O_INT16:
        if (main_options == i) {
          if (key_short_press() || key_long_press())
            (*(int16_t *) select_opt[i].value) -= select_opt[i].step;
          if (key_short_press() || key_long_press())
            (*(int16_t *) select_opt[i].value) += select_opt[i].step;
        }

        screen_show_int16(70, (i + 1), *(int16_t *)select_opt[i].value, 6);

        break;
      case O_INT32:
        if (main_options == i) {
          if (key_short_press() || key_long_press())
            (*(int32_t *) select_opt[i].value) -= select_opt[i].step;
          if (key_short_press() || key_long_press())
            (*(int32_t *) select_opt[i].value) += select_opt[i].step;
        }

        screen_show_int32(70, (i + 1), *(int32_t *)select_opt[i].value, 10);

        break;
      case O_FLOAT:
        if (main_options == i) {
          if (key_short_press() || key_long_press())
            (*(float *) select_opt[i].value) -= select_opt[i].step;
          if (key_short_press() || key_long_press())
            (*(float *) select_opt[i].value) += select_opt[i].step;
        }

        screen_show_float(70, (i + 1), *(float *)select_opt[i].value + 0.00001, 5, 3); // + 0.0...01 to round sb float^^

        break;

      default:
        break;
    }
  }

  if (key_long_press()) {
    screen_clear();
    current_func = display_main;
  }
}


void get_uart() {
  // screen_show_uint32(0, 2, timer_1s, 10);
  // screen_show_uint16(0, 3, (uint16_t)uart_timer_update, 6);
  // screen_show_uint16(0, 4, (uint16_t)uart_get_count, 6);
  //
  // screen_show_int32(0, 6, icar_param.duty_x, 10);
  // screen_show_int32(0, 7, icar_param.duty_y, 10);
  //
  // if (key_long_press())
  // {
  //     screen_clear();
  //     current_func = display_main;
  // }
}

// 目录
uint32_t interface_update_timer_1s = 0;

void display_main(void) {
  screen_show_string(36, 0, "[Main]");
  const M_SELECT select_opt[] =
  {
    {"config", display_main},
    {"task", display_main},
    {"ccd", display_main},
    {"stepper", display_main},
    {"get uart", get_uart},

    // {"Edit Speed", display_edit_speed},
    // {"Edit Motor", display_edit_motor},
  };
  const uint8_t select_opt_num = sizeof(select_opt) / sizeof(M_SELECT);

  static uint8_t main_options = 0;
  uint8_t last_option;
  uint8_t i;

  // 换页
  if (key_short_press()) {
    last_option = main_options;
    if (main_options-- == 0)
      main_options = select_opt_num - 1;

    screen_show_string(0, (last_option + 1), "  ");
  }
  if (key_short_press()) {
    last_option = main_options;
    if (++main_options >= select_opt_num)
      main_options = 0;

    screen_show_string(0, (last_option + 1), "  ");
  }

  // 显示级联主菜单与选项指针
  for (i = 0; i < select_opt_num; i++) {
    if (main_options == i)
      screen_show_string(0, (i + 1), "->");
    screen_show_string(16, (i + 1), select_opt[i].name);
  }

  if (key_short_press()) {
    screen_clear();
    sprintf(chr, "[%s]", select_opt[main_options].name);
    screen_show_string(36, 0, chr);
    current_func = select_opt[main_options].func;
  }
}

void (*current_func)(void) = display_main;

void interface_main(void) {
  current_func();

  ssd1306_UpdateScreen();
}
