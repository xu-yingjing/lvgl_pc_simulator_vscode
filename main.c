#include <unistd.h>
#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

static lv_disp_t *hal_init(lv_coord_t w, lv_coord_t h)
{
    lv_disp_t *disp = lv_sdl_window_create(w, h);
    lv_indev_t *mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, lv_group_get_default());
    lv_indev_set_disp(mouse, disp);

    LV_IMG_DECLARE(mouse_cursor_icon);
    lv_obj_t *cursor_obj;
    cursor_obj = lv_img_create(lv_scr_act());
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);
    lv_indev_set_cursor(mouse, cursor_obj);

    lv_indev_t *mousewheel = lv_sdl_mousewheel_create();
    lv_indev_set_disp(mousewheel, disp);
    lv_indev_set_group(mousewheel, lv_group_get_default());

    lv_indev_t *keyboard = lv_sdl_keyboard_create();
    lv_indev_set_disp(keyboard, disp);
    lv_indev_set_group(keyboard, lv_group_get_default());

    return disp;
}

int main(int argc, char **argv)
{
    lv_init();
    lv_group_set_default(lv_group_create());
    hal_init(240, 320);

    lv_demo_widgets();

    while (1)
    {
        lv_timer_handler();
        usleep(5 * 1000);
    }
}
