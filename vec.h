typedef struct Vec3 {
	double x, y, z;
} Vec3;

Vec3 vmake3(double x, double y, double z);
Vec3 vmul3(double k, Vec3 a);
Vec3 vdiv3(double k, Vec3 a);
Vec3 vadd3(Vec3 a, Vec3 b);
Vec3 vsub3(Vec3 a, Vec3 b);
double vdot3(Vec3 a, Vec3 b);
Vec3 vcross3(Vec3 a, Vec3 b);
double vlen3(Vec3 a);
Vec3 vunit3(Vec3 a);
Vec3 vflip3(Vec3 a);
