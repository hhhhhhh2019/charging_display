#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <draw.h>
#include <font.h>
#include <errno.h>

#include <linux/input.h>


const int width  = 720;
const int height = 1280;
const int width_add = 16;

const int cen_x = width  >> 1;
const int cen_y = height >> 1;

char* framebuffer;

Font* font;

char screen_state;
int lastscreenon_time = 0;
int lastupdate_time = 0;

struct input_event event;



char* get_battery_status() {
	FILE* f = fopen("/sys/class/power_supply/battery/capacity", "r");
	if (f == NULL)
		exit(2);
	
	int size = 3;
	
	char* buf = calloc(size+2,1);
	
	/*buf[0] = rand()%10+'0';
	buf[1] = rand()%10+'0';*/

	int count = fread(buf, size, 1, f);
	buf[count+2] = 0;
	buf[count+1] = '%';

	return buf;
}


void screen_on() {
	system("echo 255 > /sys/class/leds/lcd-backlight/brightness");	
	screen_state = 1;
	lastscreenon_time = time(NULL);
}

void screen_off() {
	system("echo 0 > /sys/class/leds/lcd-backlight/brightness");	
	screen_state = 0;
}


// только кнопка питания
void poll_events() {
	int f = open("/dev/input/event0", O_RDONLY);

	fd_set read_fds, write_fds, except_fds;

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_ZERO(&except_fds);
	FD_SET(f, &read_fds);

	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	if (select(f + 1, &read_fds, &write_fds, &except_fds, &timeout) > 0) {
		read(f, &event, sizeof(event));
		//printf("%d %d %d\n", event.type, event.code, event.value);

		if (event.code == 116 && event.value == 1) {
			if (screen_state == 1)
				screen_off();
			else
				screen_on();
		}
	}

	close(f);
}


int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Usage: %s <font.pgm> <font.font>\n", argv[0]);
		return 0;
	}

	framebuffer = calloc((width+width_add)*height*4, 1);
	update_screen(); // clear boot animation

	font = load_font(argv[1], argv[2]);

	screen_on();

	int tw = 100;
	int th = 64;

	while (1) {
		if (time(NULL) - lastscreenon_time >= 10) {
			screen_off();
		}

		if (screen_state == 1 && time(NULL) - lastupdate_time >= 5) {
			rect(cen_x-tw, cen_y-th, tw<<1, th<<1, 0,0,0);

			char* stat = get_battery_status();
			draw_string(font, stat, 360,640, 2);
			free(stat);

			update_screen_rect(cen_x-tw, cen_y-th, tw<<1, th<<1);
			//update_screen();

			lastupdate_time = time(NULL);
		}

		poll_events();

		sleep(0.1);
	}
}
