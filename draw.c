#include <draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <font.h>


extern unsigned int width;
extern unsigned int height;
extern unsigned int width_add;

extern char* framebuffer;


void set_pixel(unsigned int x, unsigned int y, char r, char g, char b) {
	if (x >= width || y >= height)
		return;

	unsigned int id = (y * (width + width_add) + x) * 4;
	framebuffer[id + 0] = b;
	framebuffer[id + 1] = g;
	framebuffer[id + 2] = r;
	framebuffer[id + 3] = 255;
}

void get_pixel(unsigned int x, unsigned int y, char* r, char* g, char* b) {
	unsigned int id = (y * (width + width_add) + x) * 4;
	*b = framebuffer[id + 0];
	*g = framebuffer[id + 1];
	*r = framebuffer[id + 2];
}


void rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, char r, char g, char b) {
	for (unsigned int i = y; i < y + h; i++)
		for (unsigned int j = x; j < x + w; j++)
			set_pixel(j,i, r,g,b);
}


void update_screen() {
	FILE* f = fopen("/dev/fb0", "wb");
	if (f == NULL)
		exit(2);
	fwrite(framebuffer, (width+width_add)*height*4, 1, f);
	fclose(f);
}

void update_screen_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	FILE* f = fopen("/dev/fb0", "wb");
	if (f == NULL)
		exit(2);
	for (unsigned int i = y; i < y + h; i++) {
		fseek(f, (i*(width+width_add)+x)*4, SEEK_SET);
		fwrite(framebuffer+(x+i*(width+width_add))*4, w*4, 1, f);
	}
	fclose(f);
}


void draw_symbol(Font* font, char ch, unsigned int x, unsigned int y) {
	unsigned int w = font->rects[ch].x2 - font->rects[ch].x1;
	unsigned int h = font->rects[ch].y2 - font->rects[ch].y1;

	for (unsigned int i = 0; i < w; i++) {
		for (unsigned int j = 0; j < h; j++) {
			char v = font->image.data[font->rects[ch].y1 + j][font->rects[ch].x1 + i];
			set_pixel(x+i, y+j, v,v,v);
		}
	}
}

void draw_string(Font* font, char* str, unsigned int x, unsigned int y, unsigned int padding) {
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int len = strlen(str);

	for (unsigned int i = 0; i < len; i++) {
		width += font->rects[str[i]].x2 - font->rects[str[i]].x1 + padding;
		unsigned int h = font->rects[str[i]].y2 - font->rects[str[i]].y1;
		if (h > height)
			height = h;
	}

	for (unsigned int i = 0; i < len; i++) {
		draw_symbol(font, str[i], x-width/2, y-height/2);
		x += font->rects[str[i]].x2 - font->rects[str[i]].x1 + padding;
	}
}
