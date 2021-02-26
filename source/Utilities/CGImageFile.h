#ifndef CG_IMAGEFILE_H
#define CG_IMAGEFILE_H

extern int cgImageWrite(const char *filename, unsigned char *data, int width, int height);
extern unsigned char *cgImageLoad(const char *filename, int *width, int *height, bool flip_vertically = false);

#endif
