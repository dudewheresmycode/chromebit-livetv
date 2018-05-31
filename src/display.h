#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

static struct screenInfo {
	int x, y, bpp;
};

static struct screenInfo get_display(){
	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
	char *fbp = 0;
	int x = 0, y = 0;
	long int location = 0;
	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1) {
	    perror("Error: cannot open framebuffer device");
	    return;
	}
	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
	    perror("Error reading fixed information");
	    return;
	}
	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
	    perror("Error reading variable information");
	    return;
	}
	struct screenInfo i1 = {
		vinfo.xres, vinfo.yres, vinfo.bits_per_pixel
	};
	/*     fprintf(stdout, "{\"width\":\"%d\", \"height\":\"%d\", \"bpp\":\"%d\"}", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel); */
	// Figure out the size of the screen in bytes
	//
	close(fbfd);
	return i1;
}
