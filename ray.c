#include <math.h>
#include <stdint.h>

#include "ray.h"
#include "vec.h"

typedef struct Sphere Sphere;
struct Sphere {
	Vec3 center;
	double radius;
};

typedef struct Light Light;
struct Light {
	Vec3 position;
	Vec3 color;
};

static RGB rgb(Vec3 color);

static const Vec3 background = {0.0, 0.0, 0.0};
static const double left = -WIDTH / 2.0;
static const double bottom = -HEIGHT / 2.0;

static RGB image[HEIGHT][WIDTH];
static Vec3 eye = {0.0, 0.0, 0.0};
static Sphere sphere = {
	{0.0, 0.0, -2.0},
	0.5
};
static Light light = {
	{0.0, 1.0, -0.25},
	{1.0, 1.0, 1.0}
};

RGB *raytraceimage(void)
{
	int j, i;
	Vec3 d, e, x, l, n;
	double a, b, c, disc, root, r;
	Vec3 color;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			d = vmake3(left + (j+0.5), bottom + (i+0.5), -DEPTH);
			a = vdot3(d, d);
			e = vsub3(eye, sphere.center);
			b = 2 * vdot3(d, e);
			c = vdot3(e, e) - (sphere.radius*sphere.radius);
			disc = b*b - 4*a*c;
			if (disc < 0) {
				image[i][j] = rgb(background);
			} else {
				root = (-b - sqrt(disc)) / (2*a);
				x = vadd3(eye, vmul3(root, d));
				l = vsub3(light.position, x);
				r = vlen3(l);
				l = vdiv3(r, l);
				n = vdiv3(sphere.radius, vsub3(x, sphere.center));
				color = vmul3(fmax(0, vdot3(n, l)) / (r*r), light.color);
				image[i][j] = rgb(color);
			}
		}
	}

	return &image[0][0];
}

static RGB rgb(Vec3 color)
{
	RGB c;

	c.r = fmax(fmin(color.x, 1.0), 0.0) * 255;
	c.g = fmax(fmin(color.y, 1.0), 0.0) * 255;
	c.b = fmax(fmin(color.z, 1.0), 0.0) * 255;
	return c;
}
