#include <stdint.h>

#include "cocoa.h"
#include "ray.h"

static void render(void)
{
    RGB *img = raytrace();
    displayimage(img, image.nx, image.ny);
}

int main(void)
{
    while (running()) {
        processinput();
        render();
    }

    return 0;
}
