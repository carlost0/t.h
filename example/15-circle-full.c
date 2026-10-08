#define TURTLE_IMPLEMENTATION
#include "../t.h"

int main(void) {
    t_init();

    t_target_fps(60);
    t_pendown();

    f32 radius = 10;

    while (!t_should_close()) {
        t_rotate(1);
        t_push();
        t_step(radius);
        t_draw();
        t_pop();

        t_update();
    }

    t_destroy();
    return 0;
}
