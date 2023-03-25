#include <pgm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


void ignore_comments(FILE* f) {
	char c = getc(f);
	while (c == '\n') c = getc(f);
	if (c != '#') {
		fseek(f, -1, SEEK_CUR);
		return;
	}

	while (c != '\n' && !feof(f)) c = getc(f);
}


int get_number(char* buffer, unsigned char* number) {
	int count = 1;
	char ch = *(buffer++);
	while (ch == ' ') {
		ch = *(buffer++);
		count++;
	}

	buffer--;

	while (1) {
		ch = *(buffer++);

		if (ch == ' ' || ch == 0 || ch == '\n')
			break;

		*number = *number * 10 + (ch - '0');

		count++;
	}

	return count;
}


char read_pgm(const char* filename, PGM* pgm) {
	FILE* f = fopen(filename, "rb");

	if (f == NULL)
		return 0;	

	ignore_comments(f);

	fscanf(f, "%s", pgm->type);
	
	if (pgm->type[0] != 'P')
		return 0;
	
	ignore_comments(f);

	fscanf(f, "%u %u", &pgm->width, &pgm->height);
	
	pgm->data = malloc(pgm->height * sizeof(char*));

	ignore_comments(f);
	
	int maxv;
	fscanf(f, "%d", &maxv);

	ignore_comments(f);

	fgetc(f); // new line	

	for (unsigned int i = 0; i < pgm->height; i++)
		pgm->data[i] = malloc(pgm->width * sizeof(char) * 4);
	
	if (pgm->type[1] == '5') {
		for (unsigned int i = 0; i < pgm->height; i++)
			fread(pgm->data[i], sizeof(char), pgm->width, f);
	} else if (pgm->type[1] == '2') {		
		unsigned int x = 0;
		unsigned int y = 0;
	
		char* buffer = calloc(1,1);
		
		while (!feof(f)) {
			char ch = getc(f);
	
			if (ch == ' ' || ch == '\n' || ch == '\t') {
				if (strlen(buffer) == 0)
					continue;
	
				pgm->data[y][x++] = 255-atoi(buffer);
				if (x == pgm->width) {
					x = 0;
					y++;
				}
	
				free(buffer);
				buffer = calloc(1,1);
	
				continue;
			}
	
			int len = strlen(buffer);
	
			buffer = realloc(buffer, len+2);
			buffer[len+1] = 0;
			buffer[len] = ch;
		}

		free(buffer);
	}
	
	fclose(f);
	return 1;
}
