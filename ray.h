typedef uint8_t RGB[3];

struct Image {
	int nx;
	int ny;
	RGB *pix;
};

extern struct Image image;

RGB *raytrace(void);
