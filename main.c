#include <stdint.h>

#include "cocoa.h"
#include "ray.h"

static void render(void)
{
    Image *img = raytrace();
    displayimage(img->pix, img->nx, img->ny);
}

int main(void)
{
    while (running()) {
        processinput();
        render();
    }

    return 0;
}
