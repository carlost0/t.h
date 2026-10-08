#define TURTLE_IMPLEMENTATION
#include "../t.h"

void branch(f32 len, f32 factor, f32 angle1, f32 angle2) {
    if (len < 1) {
        return;
    }

    t_step(len);

    t_draw();

    t_push();
    t_rotate(angle1);
    branch(len * factor, factor, angle1, angle2);
    t_pop();

    t_push();
    t_rotate(angle2);
    branch(len * factor, factor, angle1, angle2);
    t_pop();
}

int main(void) {
    t_init();

    t_target_fps(30);

    f32 factor = 0.7f;
    u32 len = screen_h * factor / 2;
    f32 angle1 = 60;
    f32 angle2 = -60;


    t_pendown();

    while (!t_should_close()) {
        t_clear();

        t_color(hex(0xff0000));
        t_pos(screen_w * 0.25, screen_h - 1);
        branch(len, factor, angle1--, angle2--);

        t_color(hex(0x00ff00));
        t_pos(screen_w * 0.5, screen_h - 1);
        branch(len, factor, angle1, angle2);

        t_color(hex(0x0000ff));
        t_pos(screen_w * 0.7, screen_h - 1);
        branch(len, factor, angle1, angle2);

        t_update();
    }

    t_destroy();
}
