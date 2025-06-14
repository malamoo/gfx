#include <stdint.h>

#include "cocoa.h"
#include "ray.h"

static void render(void);

int main(void)
{
    while (running()) {
        processinput();
        render();
    }

    return 0;
}

static void render(void)
{
    RGB *img = raytraceimage();
    displayimage(img, WIDTH, HEIGHT);
}
