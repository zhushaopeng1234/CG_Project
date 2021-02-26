#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "CGImageFile.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "CGContext/CG.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
//---------------------------------------------------------------------------
// IMAGE LOAD/WRITE API
//---------------------------------------------------------------------------

extern int cgImageWrite(const char *filename, unsigned char *data, int width, int height)
{
	const char *ext;

	ext=strrchr(filename,'.');
	if (!ext) {
		fprintf(stderr,"cgImageWrite: can't determine type of '%s'\n",filename);
		return 0;
	}

	ext++;
	if (!strcmp(ext,"TGA") || !strcmp(ext,"tga")) {
		return stbi_write_tga(filename,width,height,4,data);
	}
	if (!strcmp(ext,"PNG") || !strcmp(ext,"png")) {
		return stbi_write_png(filename,width,height,4,data,0);
	}
	if (!strcmp(ext,"bmp") || !strcmp(ext,"BMP")) {
		return stbi_write_bmp(filename,width,height,4,data);
	}
	if (!strcmp(ext,"jpg") || !strcmp(ext,"JPG")) {
		return stbi_write_jpg(filename,width,height,4,data,90);
	}


	fprintf(stderr,"cgImageWrite: unknown type '%s'\n",ext);
	return 0;
}

extern unsigned char *cgImageLoad(const char *filename, int *width, int *height, bool flip_vertically)
{
	unsigned char *res = nullptr;

	*width=0;
	*height=0;

	stbi_set_flip_vertically_on_load(flip_vertically);

	int c ;
	res = stbi_load(filename,width,height,&c,4);

	if(!res)
		fprintf(stderr,"cgImageLoad: Can not load file %s!\n",filename);
	return  res;
}

