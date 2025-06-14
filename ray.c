#include <math.h>
#include <stdint.h>

#include "ray.h"
#include "vec.h"

typedef struct Sphere Sphere;
struct Sphere {
	vector center;
	double radius;
};

typedef struct Light Light;
struct Light {
	vector position;
	vector color;
};

static RGB rgb(vector color);

static const vector background = {0.0, 0.0, 0.0};
static const double left = -WIDTH / 2.0;
static const double bottom = -HEIGHT / 2.0;

static RGB image[HEIGHT][WIDTH];
static vector eye = {0.0, 0.0, 0.0};
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
	vector d, e, x, l, n;
	double a, b, c, disc, root, r;
	vector color;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			d = vmake(left + (j+0.5), bottom + (i+0.5), -DEPTH);
			a = vdot(d, d);
			e = vsub(eye, sphere.center);
			b = 2 * vdot(d, e);
			c = vdot(e, e) - (sphere.radius*sphere.radius);
			disc = b*b - 4*a*c;
			if (disc < 0) {
				image[i][j] = rgb(background);
			} else {
				root = (-b - sqrt(disc)) / (2*a);
				x = vadd(eye, vmul(root, d));
				l = vsub(light.position, x);
				r = vlen(l);
				l = vdiv(r, l);
				n = vdiv(sphere.radius, vsub(x, sphere.center));
				color = vmul(fmax(0, vdot(n, l)) / (r*r), light.color);
				image[i][j] = rgb(color);
			}
		}
	}

	return &image[0][0];
}

static RGB rgb(vector color)
{
	RGB c;

	c.r = fmax(fmin(color.x, 1.0), 0.0) * 255;
	c.g = fmax(fmin(color.y, 1.0), 0.0) * 255;
	c.b = fmax(fmin(color.z, 1.0), 0.0) * 255;
	return c;
}
