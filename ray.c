#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "ray.h"
#include "vec.h"

enum {
	NXINIT = 400,
	FOVINIT = 60,
	SURFINIT = 1,
	SURFGROW = 2
};

enum Surftype {
	SPHERE
};

typedef Vector3 Color;

typedef struct Sphere Sphere;
struct Sphere {
	Vector3 c;
	double r;
};

typedef struct Surface Surface;
struct Surface {
	int type;
	void *ptr;
};

typedef struct Light Light;
struct Light {
	Vector3 pos;
	Vector3 intensity;
};

typedef struct Ray Ray;
struct Ray {
	Vector3 o;
	Vector3 d;
};

typedef struct Hit Hit;
struct Hit {
	Surface *surf;
	double t;
	Vector3 n;
};

typedef struct Frame Frame;
struct Frame {
	Vector3 e;
	Vector3 u;
	Vector3 v;
	Vector3 w;
};

static Hit intersect_sphere(Ray *r, Surface *s, double t0, double t1);

static const double INF = 1.0 / 0.0;
static const double PI = 3.14159265358979323;
static const double DEG2RAD = PI / 180.0;
static const Vector3 UP = {0.0, 1.0, 0.0};
static const Color BACKGROUND = {0.0, 0.0, 0.0};
static const double ASPECT = 16.0 / 9.0;
static const double VHEIGHT = 2.0;

static struct Surftab {
	int nval;
	int max;
	Surface *surface;
} surftab;
struct Image image;
static struct View {
	double r;
	double l;
	double t;
	double b;
	double fov;
	double focal;
	Frame camera;
} view;
static Light lamp = {{0.3, 0.6, -0.15}, {1.0, 1.0, 1.0}};
static Hit (*intersectfn[])(Ray *, Surface *, double, double) = {
	intersect_sphere
};

static int addsurface(Surface newsurf)
{
	Surface *sfp;

	if (surftab.surface == NULL) {
		surftab.surface = malloc(SURFINIT * sizeof(Surface));
		if (surftab.surface == NULL)
			return -1;
		surftab.max = SURFINIT;
		surftab.nval = 0;
	} else if (surftab.nval >= surftab.max) {
		sfp = realloc(surftab.surface, (SURFGROW*surftab.max) * sizeof(Surface));
		if (sfp == NULL)
			return -1;
		surftab.max *= SURFGROW;
		surftab.surface = sfp;
	}
	surftab.surface[surftab.nval] = newsurf;
	return surftab.nval++;
}

static void orientcamera(Vector3 pos, Vector3 dir)
{
	view.camera.e = pos;
	view.camera.w = unitv3(flipv3(dir));
	view.camera.u = unitv3(crossv3(UP, view.camera.w));
	view.camera.v = unitv3(crossv3(view.camera.w, view.camera.u));
}

static Vector3 evalray(Ray *r, double t)
{
	return addv3(r->o, mulv3(r->d, t));
}

static int quadroot(double root[2], double a, double b, double c)
{
	double d, r0, r1;

	d = b*b - 4*a*c;
	if (d < 0)
		return 0;

	r0 = (-b - sqrt(d)) / (2*a);
	r1 = (-b + sqrt(d)) / (2*a);
	if (r0 < r1) {
		root[0] = r0;
		root[1] = r1;
		return 2;
	} else if (r0 > r1) {
		root[0] = r1;
		root[1] = r0;
		return 2;
	} else {
		root[0] = r0;
		return 1;
	}
}

static Hit intersect_sphere(Ray *r, Surface *s, double t0, double t1)
{
	Sphere *sp;
	double a, b, c, root[2], t;
	Hit hit;

	sp = (Sphere *)s->ptr;
	hit.t = INF;
	a = dotv3(r->d, r->d);
	b = 2*dotv3(r->d, subv3(r->o, sp->c));
	c = dotv3(subv3(r->o, sp->c), subv3(r->o, sp->c)) - sp->r*sp->r;
	if (quadroot(root, a, b, c) < 1)
		return hit;
	t = root[0];
	if (t < t0 || t > t1) {
		t = root[1];
		if (t < t0 || t > t1) {
			return hit;
		}
	}
	hit.surf = s;
	hit.t = t;
	hit.n = divv3(subv3(evalray(r, t), sp->c), sp->r);
	return hit;
}

static Color shaderay(Ray *ray, double t0, double t1)
{
	Surface *sp;
	Hit hit, minhit;
	Vector3 l;
	double r;
	int i;

	minhit.t = INF;
	for (i = 0; i < surftab.nval; i++) {
		sp = &surftab.surface[i];
		hit = intersectfn[sp->type](ray, sp, t0, t1);
		if (hit.t < INF) {
			minhit = hit;
			t1 = hit.t;
		}
	}
	if (minhit.t < INF) {
		l = subv3(lamp.pos, evalray(ray, minhit.t));
		r = lenv3(l);
		l = divv3(l, r);
		return mulv3(lamp.intensity, fmax(0.0, dotv3(minhit.n, l))/(r*r));
	} else {
		return BACKGROUND;
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
	r.o = view.camera.e;
	r.d = addv3(addv3(mulv3(view.camera.w, -view.focal), mulv3(view.camera.u, u)), mulv3(view.camera.v, v));
	return r;
}

static Ray orthoray(int i, int j)
{
	Ray r;
	double u, v;

	u = view.l + (view.r - view.l) * (i+0.5) / image.nx;
	v = view.b + (view.t - view.b) * (j+0.5) / image.ny;
	r.o = addv3(addv3(view.camera.e, mulv3(view.camera.u, u)), mulv3(view.camera.v, v));
	r.d = mulv3(view.camera.w, -view.focal);
	return r;
}

static Surface makesurface(int type, void *ptr)
{
	Surface s;

	s.type = type;
	s.ptr = ptr;
	return s;
}

static void init(void)
{
	Sphere *sp;

	image.nx = NXINIT;
	image.ny = image.nx / ASPECT;
	image.pix = malloc(image.nx * image.ny * sizeof(RGB));
	view.t = VHEIGHT / 2.0;
	view.b = -view.t;
	view.r = (VHEIGHT * (double)image.nx / image.ny) / 2.0;
	view.l = -view.r;
	view.fov = FOVINIT;
	view.focal = view.t / tan(DEG2RAD*view.fov/2.0);
	orientcamera(makev3(0.0, 0.0, 0.0), makev3(0.0, 0.0, -1.0));
	sp = malloc(sizeof(Sphere));
	sp->c = makev3(0.0, 0.0, -1.5);
	sp->r = 0.25;
	addsurface(makesurface(SPHERE, sp));
}

RGB *raytrace(void)
{
	int i, j;
	Ray r;

	if (image.pix == NULL)
		init();

	for (j = 0; j < image.ny; j++) {
		for (i = 0; i < image.nx; i++) {
			r = perspectiveray(i, j);
			writergb(shaderay(&r, 0.0, INF), i ,j);
		}
	}

	return image.pix;
}
