#include <math.h>
#include <stdlib.h>

#include "ray.h"

typedef struct Vec3 Vec3;
typedef struct Ray Ray;
typedef struct Surface Surface;
typedef struct Sphere Sphere;
typedef struct Group Group;
typedef struct Hitrecord Hitrecord;
typedef struct Frame Frame;

struct Vec3 {
	double c[3];
};

struct Ray {
	Vec3 o;
	Vec3 d;
};

struct Surface {
	int type;
	void *p;
};

struct Sphere {
	Vec3 c;
	float r;
};

struct Group {
	Surface *surface;
	int nsurface;
};

struct Hitrecord {
	Surface *surface;
	float t;
	Vec3 n;
};

struct Frame {
	Vec3 p;
	Vec3 u, v, w;
};

static Vec3 v3mul(float k, Vec3 a);
static Vec3 v3div(float k, Vec3 a);
static Vec3 v3add(Vec3 a, Vec3 b);
static Vec3 v3sub(Vec3 a, Vec3 b);
static double v3dot(Vec3 a, Vec3 b);
static Vec3 v3cross(Vec3 a, Vec3 b);
static double v3len(Vec3 a);
static Vec3 v3unit(Vec3 a);
static Vec3 evalray(Ray *r, double t);
static Hitrecord hit(Surface *s, Ray *r, double t0, double t1);
static Hitrecord hit_sphere(Surface *s, Ray *r, double t0, double t1);
static Frame makeframe(Vec3 p, Vec3 a, Vec3 b);
static Vec3 evaloffset(Frame *f, Vec3 a);

enum {
	SPHERE,
	GROUP,
};

enum {
	NCHANNEL = 3,
	BLACK = 0,
	WHITE = 255,
};

static const double INF = 1/0.0;
static const Vec3 BACK = {0, 0, 1};
static const Vec3 UP = {0, 1, 0};
static const Vec3 ZERO = {0, 0, 0};
static const double PIXELOFFSET = 0.5;
static const double IMGWIDTH = 0.0036;
static const double FOCALLEN = 0.0036;

static Hitrecord (*hitfn[])(Surface *, Ray *, double, double) = {
	hit_sphere,
};

static Vec3 v3mul(float k, Vec3 a) {
	double *c = a.c;

	return (Vec3){k*c[0], k*c[1], k*c[2]};
}

static Vec3 v3div(float k, Vec3 a) {
	double *c = a.c;

	return (Vec3){c[0]/k, c[1]/k, c[2]/k};
}

static Vec3 v3add(Vec3 a, Vec3 b) {
	double *ac = a.c, *bc = b.c;

	return (Vec3){ac[0]+bc[0], ac[1]+bc[1], ac[2]+bc[2]};
}

static Vec3 v3sub(Vec3 a, Vec3 b) {
	double *ac = a.c, *bc = b.c;

	return (Vec3){ac[0]-bc[0], ac[1]-bc[1], ac[2]-bc[2]};
}

static double v3dot(Vec3 a, Vec3 b) {
	double *ac = a.c, *bc = b.c;

	return ac[0]*bc[0] + ac[1]*bc[1] + ac[2]*bc[2];
}

static Vec3 v3cross(Vec3 a, Vec3 b) {
	double *ac = a.c, *bc = b.c;

	return (Vec3){
	    ac[1]*bc[2] - ac[2]*bc[1],
	    ac[2]*bc[0] - ac[0]*bc[2],
	    ac[0]*bc[1] - ac[1]*bc[0],
	};
}

static double v3len(Vec3 a) {
	double *c = a.c;

	return sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
}

static Vec3 v3unit(Vec3 a) {
	return v3div(v3len(a), a);
}

static Vec3 evalray(Ray *r, double t) {
	return v3add(r->o, v3mul(t, r->d));
}

static Hitrecord hit(Surface *s, Ray *r, double t0, double t1) {
	return (*hitfn[s->type])(s, r, t0, t1);
}

static Hitrecord hit_sphere(Surface *s, Ray *r, double t0, double t1) {
	Sphere *sp = s->p;
	Vec3 d, e, c;
	double A, B, C, R, disc, s1, s2, tmp;

	d = r->d;
	e = r->o;
	c = sp->c;
	R = sp->r;
	A = v3dot(d, d);
	B = 2 * v3dot(d, v3sub(e, c));
	C = v3dot(v3sub(e, c), v3sub(e, c)) - R*R;

	disc = B*B - 4*A*C;
	if (disc < 0)
		return (Hitrecord){0};

	s1 = (-B + sqrt(disc)) / 2*A;
	s2 = (-B - sqrt(disc)) / 2*A;
	if (s1 > s2) {
		tmp = s1;
		s1 = s2;
		s2 = tmp;
	}

	if (t0 <= s1 && s1 <= t1) {
		return (Hitrecord){
		    .surface = s,
		    .t = s1,
			.n = v3div(R, v3sub(evalray(r, s1), c)),
		};
	}

	if (t0 <= s2 && s2 <= t1) {
		return (Hitrecord){
		    .surface = s,
		    .t = s2,
			.n = v3div(R, v3sub(evalray(r, s2), c)),
		};
	}

	return (Hitrecord){0};
}

static Frame makeframe(Vec3 p, Vec3 a, Vec3 b) {
	Frame f = {.p = p};

	f.w = v3unit(a);
	f.u = v3unit(v3cross(b, f.w));
	f.v = v3cross(f.w, f.u);
	return f;
}

static Vec3 evaloffset(Frame *f, Vec3 a) {
	double *c = a.c;

	return v3add(v3add(v3mul(c[0], f->u), v3mul(c[1], f->v)), v3mul(c[2], f->w));
}

int renderimage(uint8_t *pixel, int nx, int ny) {
	int i, j, k;
	double l, r, b, t;
	double u, v, d;
	double aspect;
	Frame camera;
	Ray ray;
	Hitrecord rec;
	Sphere sphere = {
	    .c = {0, 0, -2},
	    .r = 0.25,
	};
	Surface surface = {
	    .type = SPHERE,
	    .p = &sphere,
	};

	aspect = nx / (double)ny;
	r = (IMGWIDTH) / 2;
	l = -r;
	t = (IMGWIDTH/aspect) / 2;
	b = -t;
	d = FOCALLEN;
	camera = makeframe(ZERO, BACK, UP);
	ray.o = camera.p;
	for (j = 0; j < ny; j++) {
		for (i = 0; i < nx; i++) {
			u = l + (i+PIXELOFFSET) * ((r-l)/nx);
			v = b + (j+PIXELOFFSET) * ((t-b)/ny);
			ray.d = evaloffset(&camera, (Vec3){u, v, -d});
			rec = hit(&surface, &ray, 0, INF);
			k = (j*nx + i) * NCHANNEL;
			if (rec.surface == NULL) {
				pixel[k++] = BLACK;
				pixel[k++] = BLACK;
				pixel[k++] = BLACK;
			} else {
				pixel[k++] = WHITE;
				pixel[k++] = WHITE;
				pixel[k++] = WHITE;
			}
		}
	}
	return 0;
}
