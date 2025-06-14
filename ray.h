enum {
	WIDTH = 1440,
	HEIGHT = 1080,
	DEPTH = 1080
};

typedef struct RGB {
    uint8_t r, g, b;
} RGB;

RGB *raytraceimage(void);
