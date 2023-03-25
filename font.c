#include <font.h>
#include <pgm.h>
#include <stdlib.h>
#include <stdio.h>


Font* load_font(const char* pgm_name, const char* font_name) {
	Font* result = calloc(sizeof(Font), 1);
	read_pgm(pgm_name, &result->image);

	FILE* f = fopen(font_name, "r");

	int count;

	fscanf(f, "%d", &count);
	getc(f);

	for (int i = 0; i < count; i++) {
		unsigned char id;
		unsigned short x1,y1,x2,y2;
		fscanf(f, "%hu %hu %hu %hu %c", &x1,&y1,&x2,&y2, &id);
		result->rects[id].x1 = x1;
		result->rects[id].y1 = y1;
		result->rects[id].x2 = x2;
		result->rects[id].y2 = y2;

		if (i != count - 1)
			getc(f);
	}

	fclose(f);

	return result;
}
