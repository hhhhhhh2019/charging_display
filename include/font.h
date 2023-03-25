#ifndef FONT_H
#define FONT_H

#include <pgm.h>


typedef struct {
	unsigned short x1,y1,x2,y2;
} Rect;

typedef struct {
	PGM image;
	Rect rects[256];
} Font;


Font* load_font(const char*, const char*);


#endif // FONT_H
