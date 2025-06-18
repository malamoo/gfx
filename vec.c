#include <math.h>

#include "vec.h"

Vec3 vmake3(double x, double y, double z)
{
	Vec3 a;

	a.x = x;
	a.y = y;
	a.z = z;
	return a;
}

Vec3 vmul3(double k, Vec3 a)
{
	a.x *= k;
	a.y *= k;
	a.z *= k;
	return a;
}

Vec3 vdiv3(double k, Vec3 a)
{
	a.x /= k;
	a.y /= k;
	a.z /= k;
	return a;
}

Vec3 vadd3(Vec3 a, Vec3 b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vec3 vsub3(Vec3 a, Vec3 b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

double vdot3(Vec3 a, Vec3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 vcross3(Vec3 a, Vec3 b)
{
	Vec3 c;

	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;
	return c;
}

double vlen3(Vec3 a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vec3 vunit3(Vec3 a)
{
	return vdiv3(vlen3(a), a);
}

Vec3 vflip3(Vec3 a)
{
	return vmul3(-1, a);
}
