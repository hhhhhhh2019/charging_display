#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>


const int width  = 736;
const int height = 1280;

char* image;


#define char_margin 2
#define char_scale 3
#define char_width  8
#define char_height 16

#define status_x width/2
#define status_y height*4/5



typedef struct {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	unsigned int value;
} input_event;



char symbols[10][char_width*char_height] = {
	{0,0,1,1,1,1,0,0,
	 0,1,0,0,0,0,1,0,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 0,1,0,0,0,0,1,0,
	 0,0,1,1,1,1,0,0},

	{0,0,0,0,0,0,1,1,
	 0,0,0,0,0,1,0,1,
	 0,0,0,0,1,0,0,1,
	 0,0,0,1,0,0,0,1,
	 0,0,1,0,0,0,0,1,
	 0,1,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1},

	{0,0,1,1,1,1,0,0,
	 0,1,0,0,0,0,1,0,
	 1,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,1,0,
	 0,0,0,0,0,1,0,0,
	 0,0,0,0,1,0,0,0,
	 0,0,0,1,0,0,0,0,
	 0,0,1,0,0,0,0,0,
	 0,1,0,0,0,0,0,0,
	 1,0,0,0,0,0,0,0,
	 1,1,1,1,1,1,1,1},

	{0,0,1,1,1,1,0,0,
	 0,1,0,0,0,0,1,0,
	 1,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,1,0,
	 0,0,0,0,1,1,1,0,
	 0,0,0,0,0,0,1,0,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 0,1,0,0,0,0,1,0,
	 0,0,1,1,1,1,0,0},

	{1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,1,1,1,1,1,1,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1},

	{1,1,1,1,1,1,1,1,
	 1,0,0,0,0,0,0,0,
	 1,0,0,0,0,0,0,0,
	 1,0,0,0,0,0,0,0,
	 1,0,0,0,0,0,0,0,
	 1,0,0,0,0,0,0,0,
	 1,0,0,0,0,0,0,0,
	 1,1,1,1,0,0,0,0,
	 0,0,0,0,1,1,0,0,
	 0,0,0,0,0,0,1,0,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,1,0,
	 1,1,1,1,1,1,0,0},

	{0,0,0,1,1,1,1,0,
	 0,0,1,0,0,0,0,1,
	 0,1,0,0,0,0,0,0,
	 0,1,0,0,0,0,0,0,
	 1,0,0,0,0,0,0,0,
	 1,0,0,0,0,0,0,0,
	 1,0,0,0,0,0,0,0,
	 1,0,0,0,0,0,0,0,
	 1,0,1,1,1,1,0,0,
	 1,1,0,0,0,0,1,0,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 0,1,0,0,0,0,1,0,
	 0,0,1,1,1,1,0,0},

	{1,1,1,1,1,1,1,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,1,0,
	 0,0,0,0,0,0,1,0,
	 0,0,0,0,0,0,1,0,
	 0,0,0,0,0,0,1,0,
	 0,0,0,0,0,1,0,0,
	 0,0,0,0,0,1,0,0,
	 0,0,0,0,0,1,0,0,
	 0,0,0,0,0,1,0,0,
	 0,0,0,0,1,0,0,0,
	 0,0,0,0,1,0,0,0,
	 0,0,0,0,1,0,0,0,
	 0,0,0,0,1,0,0,0},

	{0,0,1,1,1,1,0,0,
	 0,1,0,0,0,0,1,0,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 0,1,0,0,0,0,1,1,
	 0,0,1,1,1,1,0,0,
	 0,1,0,0,0,0,1,0,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 0,1,0,0,0,0,1,0,
	 0,0,1,1,1,1,0,0},

	{0,0,1,1,1,1,0,0,
	 0,1,0,0,0,0,1,0,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 1,0,0,0,0,0,0,1,
	 0,1,0,0,0,0,1,1,
	 0,0,1,1,1,1,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,0,1,
	 0,0,0,0,0,0,1,0,
	 0,0,0,0,0,0,1,0,
	 1,0,0,0,0,1,0,0,
	 0,1,1,1,1,0,0,0},
};


char* get_battery_status() {
	FILE* f = fopen("/sys/class/power_supply/battery/capacity", "r");
	if (f == NULL)
		exit(2);
	
	int size = 3;
	
	char* buf = calloc(size+1,1);
	buf[fread(buf, size, 1, f)+1] = 0;

	return buf;
}


void screen_off() {
	FILE* f = fopen("/sys/class/leds/lcd-backlight/brightness", "w");
	if (f == NULL)
		exit(2);
	fwrite("0", 1, 1, f);
	fclose(f);
}

void screen_on() {
	FILE* f = fopen("/sys/class/leds/lcd-backlight/brightness", "w");
	if (f == NULL)
		exit(2);
	fwrite("255\0", 3, 1, f);
	fclose(f);
}


void set_pixel(int x, int y, char r, char g, char b) {
	int id = (y * width + x) * 4;
	image[id + 0] = b;
	image[id + 1] = g;
	image[id + 2] = r;
	image[id + 3] = 255;
}


void rect(int x, int y, int w, int h, int r, int g, int b) {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			set_pixel(x + i, y + j, r,g,b);
		}
	}
}


void clear(int x, int y, int w, int h) {
	rect(x,y,w,h,0,0,0);
}



void symbol(int x, int y, int scale, char ch) {
	for (int i = 0; i < char_width * scale; i++) {
		for (int j = 0; j < char_height * scale; j++) {
			char v = symbols[ch][j / scale * char_width + i / scale] & 255;
			set_pixel(x+i,y+j, v*255,v*255,v*255);
		}
	}
}

// x,y - center
void print_text(int x, int y, int scale, char* str) {
	int len = strlen(str);
	x -= (len * (char_width + char_margin) / 2) * scale;
	y -= char_height / 2 * scale;

	for (int i = 0; i < len; i++) {
		symbol(x, y, scale, str[i] - '0');
		x += char_width * scale + char_margin * scale;
	}
}


int main(int argc, char **argv) {
	input_event event;

	int count = width * height * 4;

	image = calloc(count, 1);
	
	
	rect(width/2-150,height/2-300,300,600, 255,255,255);

	while (1) {	
		screen_on();

		char* status = get_battery_status();
		int status_int = atoi(status);
		print_text(status_x, status_y, char_scale, status);
		free(status);

		for (int i = 0; i < 580; i++) {
			char r = 0; char g = 0; char b = 0;

			if ((int)((float)(580-i)/580*100) <= status_int) {
				g = 255;
			}

			for (int j = 0; j < 280; j++)
				set_pixel(width/2-140+j, height/2-290+i, r,g,b);
		}

		FILE* fb = fopen("/dev/fb0", "w");
		fwrite(image, count, 1, fb);
		fclose(fb);

		sleep(5);

		screen_off();

		FILE* f = fopen("/dev/input/event0", "rb");

		char last_event_type = -1;
		int last_event_time = 0;
		int event_time = 0;
	
		while (1) {
			char* buffer = malloc(sizeof(input_event));
			int i = 0;
			while (i < sizeof(input_event))
				buffer[i++] = fgetc(f);
			memcpy(&event, buffer, sizeof(input_event));

			if (event.code != 116)
				continue;

			if (last_event_type == 116 && event.value != 0 && event_time - last_event_time > 500)
				system("reboot");
			
			if (event.value == 0 && event.code == 116) { // power button
				break;
			}

			last_event_type = event.value;
			event_time++;

			sleep(0.1);
		}


		fclose(f);
	}
}
