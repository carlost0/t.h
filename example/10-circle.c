#define TURTLE_IMPLEMENTATION
#include "../t.h"

int main(void) {
    t_init();

    t_target_fps(30);
    t_pendown();

    while (!t_should_close()) {
        t_step(1);
        t_rotate(10);
        t_draw();
        t_update();    
    }

    t_destroy();
    return 0;
}
