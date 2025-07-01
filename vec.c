#include <math.h>

#include "vec.h"

Vector3 makev3(double x, double y, double z)
{
	Vector3 v;

	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

Vector3 addv3(Vector3 a, Vector3 b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vector3 subv3(Vector3 a, Vector3 b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

Vector3 mulv3(Vector3 a, double k)
{
	a.x *= k;
	a.y *= k;
	a.z *= k;
	return a;
}

Vector3 divv3(Vector3 a, double k)
{
	a.x /= k;
	a.y /= k;
	a.z /= k;
	return a;
}

double dotv3(Vector3 a, Vector3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector3 crossv3(Vector3 a, Vector3 b)
{
	Vector3 c;

	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;
	return c;
}

Vector3 flipv3(Vector3 a)
{
	return mulv3(a, -1.0);
}

double lenv3(Vector3 a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vector3 unitv3(Vector3 a)
{
	return divv3(a, lenv3(a));
}
