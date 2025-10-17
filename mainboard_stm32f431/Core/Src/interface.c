#include "interface.h"

#include <stdio.h>

#include "stdbool.h"
#include "ssd1306.h"
#include "control.h"

uint8_t pages = 0;
char chr[256];
bool ChangeDataFlag = false;

uint8_t DisKp = 10;
uint8_t DisKi = 10;
uint8_t DisKd = 10;

// 读取顺时针状态
bool read_clockwise(void) {
    if (last_count - count > 0) {
        last_count = count;
        return true;
    }
    return false;
}

// 读取逆时针状态
bool read_counterclockwise(void) {
    if (last_count - count < 0) {
        last_count = count;
        return true;
    }
    return false;
}

// 短按确定键
bool read_short_confirm(void) {
    if (key.key_short) {
        key.key_short = false;
        return true;
    }
    return false;
}

// 长按确定键
bool read_long_confirm(void) {
    if (key.key_long) {
        key.key_long = false;
        return true;
    }
    return false;
}

// 总按键调参
void edit_all_option(const O_SELECT *select_opt, const uint8_t select_opt_num) {
    static uint8_t main_options = 0;
    uint8_t last_option;

    // 换页
    if (!ChangeDataFlag && read_clockwise()) {
        last_option = main_options;
        if (main_options-- == 0)
            main_options = select_opt_num - 1;

        screen_show_string(0, last_option + 1, "  ");
    }
    if (!ChangeDataFlag && read_counterclockwise()) {
        last_option = main_options;
        if (++main_options >= select_opt_num)
            main_options = 0;

        screen_show_string(0, last_option + 1, "  ");
    }

    // 显示级联主菜单与选项指针
    for (uint8_t i = 0; i < select_opt_num; i++) {
        switch (select_opt[i].type) {
            case O_BOOL:
                if (main_options == i && ChangeDataFlag) {
                    screen_set_black();
                    if (read_clockwise())
                        (*(bool *) select_opt[i].value) = false;
                    if (read_counterclockwise())
                        (*(bool *) select_opt[i].value) = true;
                }

                screen_show_string(80, i + 1, *(bool *)select_opt[i].value == true ? "true " : "false");

                break;
            case O_UINT8:
                if (main_options == i && ChangeDataFlag) {
                    screen_set_black();
                    if (read_clockwise())
                        (*(uint8_t *) select_opt[i].value) -= select_opt[i].step;
                    if (read_counterclockwise())
                        (*(uint8_t *) select_opt[i].value) += select_opt[i].step;
                }

                screen_show_uint8(80, i + 1, *(uint8_t *) select_opt[i].value, 4);

                break;
            case O_UINT16:
                if (main_options == i && ChangeDataFlag) {
                    screen_set_black();
                    if (read_clockwise())
                        (*(uint16_t *) select_opt[i].value) -= select_opt[i].step;
                    if (read_counterclockwise())
                        (*(uint16_t *) select_opt[i].value) += select_opt[i].step;
                }

                screen_show_uint16(80, i + 1, *(uint16_t *) select_opt[i].value, 6);

                break;
            case O_UINT32:
                if (main_options == i && ChangeDataFlag) {
                    screen_set_black();
                    if (read_clockwise())
                        (*(uint32_t *) select_opt[i].value) -= select_opt[i].step;
                    if (read_counterclockwise())
                        (*(uint32_t *) select_opt[i].value) += select_opt[i].step;
                }

                screen_show_uint32(80, i + 1, *(uint32_t *) select_opt[i].value, 10);

                break;
            case O_INT8:
                if (main_options == i && ChangeDataFlag) {
                    screen_set_black();
                    if (read_clockwise())
                        (*(int8_t *) select_opt[i].value) -= select_opt[i].step;
                    if (read_counterclockwise())
                        (*(int8_t *) select_opt[i].value) += select_opt[i].step;
                }

                screen_show_int8(80, i + 1, *(int8_t *) select_opt[i].value, 4);

                break;
            case O_INT16:
                if (main_options == i && ChangeDataFlag) {
                    screen_set_black();

                    if (read_clockwise())
                        (*(int16_t *) select_opt[i].value) -= select_opt[i].step;
                    if (read_counterclockwise())
                        (*(int16_t *) select_opt[i].value) += select_opt[i].step;
                }

                screen_show_int16(80, i + 1, *(int16_t *) select_opt[i].value, 6);

                break;
            case O_INT32:
                if (main_options == i && ChangeDataFlag) {
                    screen_set_black();

                    if (read_clockwise())
                        (*(int32_t *) select_opt[i].value) -= select_opt[i].step;
                    if (read_counterclockwise())
                        (*(int32_t *) select_opt[i].value) += select_opt[i].step;
                }

                screen_show_int32(80, i + 1, *(int32_t *) select_opt[i].value, 10);
                break;
            case O_FLOAT:
                if (main_options == i && ChangeDataFlag) {
                    screen_set_black();
                    if (read_clockwise())
                        (*(float *) select_opt[i].value) -= select_opt[i].step;
                    if (read_counterclockwise())
                        (*(float *) select_opt[i].value) += select_opt[i].step;
                }

                screen_show_float(40, i + 1,
                                  *(float *) select_opt[i].value + (*(float *) select_opt[i].value >= 0
                                      ? 0.0001f
                                      : -0.0001f), 5, 3); // + 0.0...01 to round
                break;

            default:
                break;
        }
        screen_set_white();
    }

    for (uint8_t i = 0; i < select_opt_num; i++) {
        if (main_options == i)
            screen_show_string(0, i + 1, "->");

        screen_show_string(16, i + 1, select_opt[i].name);
    }

    if (read_short_confirm() && select_opt[main_options].step != 0)
        ChangeDataFlag = ChangeDataFlag ? false : true; // 切换数据变化标志

    if (read_long_confirm()) {
        screen_clear();
        ChangeDataFlag = false;
        current_func = display_main;
        screen_show_string(36, 0, "[Main]");
    }
}

void edit_all_menu(const M_SELECT *select_opt, const uint8_t select_opt_num) {
    static uint8_t main_options = 0;
    uint8_t last_option;

    // 换页
    if (read_clockwise()) {
        last_option = main_options;
        if (++main_options >= select_opt_num)
            main_options = 0;

        screen_show_string(0, last_option + 1, "  ");
    }
    if (read_counterclockwise()) {
        last_option = main_options;
        if (main_options-- == 0)
            main_options = select_opt_num - 1;

        screen_show_string(0, last_option + 1, "  ");
    }

    // 显示级联主菜单与选项指针
    for (uint8_t i = 0; i < select_opt_num; i++) {
        if (main_options == i)
            screen_show_string(0, i + 1, "->");
        screen_show_string(16, i + 1, select_opt[i].name);
    }

    if (read_short_confirm()) {
        screen_clear();
        sprintf(chr, "[%s]", select_opt[main_options].name);
        screen_show_string(16, 0, chr);
        current_func = select_opt[main_options].func;
    }
}

void display_pid(void) {
    static O_SELECT select_opt[] =
    {
        {"DisYP", &DisKp, 1, O_UINT8},
        {"DisYI", &DisKi, 1, O_UINT8},
        {"DisYD", &DisKd, 1, O_UINT8},
    };
    edit_all_option(select_opt, sizeof(select_opt) / sizeof(O_SELECT));
}

void get_uart(void) {
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
        {"pid", display_pid},
        {"get uart", get_uart},
    };

    edit_all_menu(select_opt, sizeof(select_opt) / sizeof(M_SELECT));
}

void (*current_func)(void) = display_main;

void interface_main(void) {
    current_func();
    ssd1306_UpdateScreen();
}
