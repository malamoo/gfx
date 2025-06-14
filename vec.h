typedef struct vector {
	double x, y, z;
} vector;

vector vmake(double x, double y, double z);
vector vmul(double k, vector a);
vector vdiv(double k, vector a);
vector vadd(vector a, vector b);
vector vsub(vector a, vector b);
double vdot(vector a, vector b);
vector vcross(vector a, vector b);
double vlen(vector a);
vector vunit(vector a);
vector vflip(vector a);
