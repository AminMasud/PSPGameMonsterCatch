#ifndef TEST_PSPGU_H
#define TEST_PSPGU_H
/* Host-only color packing; never included by the PSP Makefile. */
#define GU_RGBA(r,g,b,a) ((unsigned int)(r) | ((unsigned int)(g)<<8) | ((unsigned int)(b)<<16) | ((unsigned int)(a)<<24))
#endif
