#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "ray.h"
#include "vec.h"
#include "eprintf.h"

#define NELEMS(arr) (sizeof(arr) / sizeof(arr)[0])

enum {
	NXINIT = 400,
	FOVINIT = 60,
	SURFINIT = 1,
	SURFGROW = 2
};

enum {
	SPHERE
};

typedef struct Sphere Sphere;
typedef struct Surface Surface;
typedef struct Surffn Surffn;
typedef struct Hit Hit;
typedef struct Ray Ray;
typedef struct Light Light;
typedef struct Frame Frame;
typedef Vector3 Color;

struct Sphere {
	Vector3 c;
	double r;
};

struct Surffn {
	int (*ishit)(Hit *, Surface *, Ray *, double, double);
};

struct Surface {
	void *ptr;
	Surffn *fn;
};

struct Ray {
	Vector3 o;
	Vector3 d;
};

struct Hit {
	Surface *surf;
	double t;
	Vector3 n;
};

struct Light {
	Vector3 pos;
	Vector3 intensity;
};

struct Frame {
	Vector3 e;
	Vector3 u;
	Vector3 v;
	Vector3 w;
};

static int ishit_sphere(Hit *hit, Surface *surf, Ray *ray, double t0, double t1);

static double inf = 1.0 / 0.0;
static double pi = 3.14159265358979323;
static Vector3 updir = {0.0, 1.0, 0.0};
static Color bgcolor = {0.0, 0.0, 0.0};
static double aspectratio = 16.0 / 9.0;
static double viewheight = 2.0;
static struct Scene {
	int nsurf;
	int max;
	Surface *surf;
} scene;
static Surffn surffn[] = {
	{ ishit_sphere }
};
static struct View {
	double fov;
	double r;
	double l;
	double t;
	double b;
	double d;
} view;
static Frame camera;
static Image image;
static Light ptlight = {
	{0.3, 0.6, 0.5},
	{1.0, 0.49, 0.0}
};

static int addsurface(Surface newsurf)
{
	Surface *sfp;

	if (scene.surf == NULL) {
		scene.surf = malloc(SURFINIT * sizeof(Surface));
		if (scene.surf == NULL)
			return -1;
		scene.max = SURFINIT;
		scene.nsurf = 0;
	} else if (scene.nsurf >= scene.max) {
		sfp = realloc(scene.surf, (SURFGROW*scene.max) * sizeof(Surface));
		if (sfp == NULL)
			return -1;
		scene.max *= SURFGROW;
		scene.surf = sfp;
	}
	scene.surf[scene.nsurf] = newsurf;
	return scene.nsurf++;
}

static void orientcamera(Vector3 pos, Vector3 dir)
{
	camera.e = pos;
	camera.w = unitv3(flipv3(dir));
	camera.u = unitv3(crossv3(updir, camera.w));
	camera.v = unitv3(crossv3(camera.w, camera.u));
}

static Vector3 evalray(Ray *r, double t)
{
	return addv3(r->o, mulv3(r->d, t));
}

static int quadroot(double r[2], double a, double b, double c)
{
	double d, tmp;

	d = b*b - 4*a*c;
	if (d < 0)
		return 0;
	r[0] = (-b - sqrt(d)) / (2*a);
	r[1] = (-b + sqrt(d)) / (2*a);
	if (r[0] < r[1]) {
		return 2;
	} else if (r[0] > r[1]) {
		tmp = r[0];
		r[0] = r[1];
		r[1] = tmp;
		return 2;
	} else {
		return 1;
	}
}

static int ishit_sphere(Hit *hit, Surface *surf, Ray *ray, double t0, double t1)
{
	Sphere *sp;
	double a, b, c, root[2], t;
	int i;

	sp = (Sphere *)surf->ptr;
	a = dotv3(ray->d, ray->d);
	b = 2*dotv3(ray->d, subv3(ray->o, sp->c));
	c = dotv3(subv3(ray->o, sp->c), subv3(ray->o, sp->c)) - sp->r*sp->r;
	if (quadroot(root, a, b, c) < 1)
		return 0;
	for (i = 0; i < NELEMS(root); i++) {
		t = root[i];
		if (t < t0 || t > t1)
			continue;
		hit->surf = surf;
		hit->t = t;
		hit->n = divv3(subv3(evalray(ray, t), sp->c), sp->r);
		return 1;
	}
	return 0;
}

static Color shaderay(Ray *ray, double t0, double t1)
{
	Surface *sp;
	Hit h;
	Vector3 l;
	double r;
	int i;

	h.t = inf;
	for (i = 0; i < scene.nsurf; i++) {
		sp = &scene.surf[i];
		if (sp->fn->ishit(&h, sp, ray, t0, t1))
			t1 = h.t;
	}
	if (h.t < inf) {
		l = subv3(ptlight.pos, evalray(ray, h.t));
		r = lenv3(l);
		l = divv3(l, r);
		return mulv3(ptlight.intensity, fmax(0.0, dotv3(h.n, l))/(r*r));
	} else {
		return bgcolor;
	}
}

static void writergb(Color c, int i, int j)
{
	int p;

	p = j*image.nx + i;
	image.pix[p][0] = fmax(fmin(c.x, 1.0), 0.0) * 255;
	image.pix[p][1] = fmax(fmin(c.y, 1.0), 0.0) * 255;
	image.pix[p][2] = fmax(fmin(c.z, 1.0), 0.0) * 255;
}

static Ray perspectiveray(int i, int j)
{
	Ray r;
	double u, v;

	u = view.l + (view.r - view.l) * (i+0.5) / image.nx;
	v = view.b + (view.t - view.b) * (j+0.5) / image.ny;
	r.o = camera.e;
	r.d = addv3(addv3(mulv3(camera.w, -view.d), mulv3(camera.u, u)), mulv3(camera.v, v));
	return r;
}

static Ray orthoray(int i, int j)
{
	Ray r;
	double u, v;

	u = view.l + (view.r - view.l) * (i+0.5) / image.nx;
	v = view.b + (view.t - view.b) * (j+0.5) / image.ny;
	r.o = addv3(addv3(camera.e, mulv3(camera.u, u)), mulv3(camera.v, v));
	r.d = mulv3(camera.w, -view.d);
	return r;
}

static double radians(double deg)
{
	return deg * (pi/180.0);
}

static Sphere *newsphere(Vector3 c, double r)
{
	Sphere *sp;

	sp = malloc(sizeof(Sphere));
	sp->c = c;
	sp->r = r;
	return sp;
}

static void init(void)
{
	Surface s;

	image.nx = NXINIT;
	image.ny = image.nx / aspectratio;
	image.pix = malloc(image.nx * image.ny * sizeof(RGB));
	view.t = viewheight / 2.0;
	view.b = -view.t;
	view.r = (viewheight * (double)image.nx/image.ny) / 2.0;
	view.l = -view.r;
	view.fov = FOVINIT;
	view.d = view.t / tan(radians(view.fov)/2.0);
	orientcamera(makev3(0.0, 0.0, 0.0), makev3(0.0, 0.0, -1.0));
	s.ptr = newsphere(makev3(0.0, 0.0, -1.0), 0.25);
	s.fn = &surffn[SPHERE];
	addsurface(s);
}

Image *raytrace(void)
{
	int i, j;
	Ray r;

	if (image.pix == NULL)
		init();

	for (j = 0; j < image.ny; j++) {
		for (i = 0; i < image.nx; i++) {
			r = perspectiveray(i, j);
			writergb(shaderay(&r, 0.0, inf), i ,j);
		}
	}

	return &image;
}
