#ifndef PGM_H
#define PGM_H


typedef struct {
	unsigned int width;
	unsigned int height;
	char type[2];
	unsigned char **data;
} PGM;


char read_pgm(const char*, PGM*);


#endif // PGM_H
