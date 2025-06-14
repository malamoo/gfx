#include <math.h>

#include "vec.h"

vector vmake(double x, double y, double z)
{
	vector a;

	a.x = x;
	a.y = y;
	a.z = z;
	return a;
}

vector vmul(double k, vector a)
{
	a.x *= k;
	a.y *= k;
	a.z *= k;
	return a;
}

vector vdiv(double k, vector a)
{
	a.x /= k;
	a.y /= k;
	a.z /= k;
	return a;
}

vector vadd(vector a, vector b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

vector vsub(vector a, vector b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

double vdot(vector a, vector b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

vector vcross(vector a, vector b)
{
	vector c;

	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;
	return c;
}

double vlen(vector a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

vector vunit(vector a)
{
	return vdiv(vlen(a), a);
}

vector vflip(vector a)
{
	return vmul(-1, a);
}
