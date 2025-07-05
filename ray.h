typedef uint8_t RGB[3];

typedef struct Image Image;
struct Image {
	int nx;
	int ny;
	RGB *pix;
};

Image *raytrace(void);
