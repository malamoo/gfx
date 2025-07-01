typedef struct Vector3 {
	double x;
	double y;
	double z;
} Vector3;

Vector3 makev3(double, double, double);
Vector3 addv3(Vector3, Vector3);
Vector3 subv3(Vector3, Vector3);
Vector3 mulv3(Vector3, double);
Vector3 divv3(Vector3, double);
double dotv3(Vector3, Vector3);
Vector3 crossv3(Vector3, Vector3);
Vector3 flipv3(Vector3);
double lenv3(Vector3);
Vector3 unitv3(Vector3);
