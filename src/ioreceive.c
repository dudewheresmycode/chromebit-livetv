#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include  <signal.h>

#define BUFFER_SIZE 16
#define EXIT_KEY  KEY_ESC
#define GRAB      1

int fd;

static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

void INThandler(int sig){
  ioctl(fd, EVIOCGRAB, GRAB);
  close(fd);
	exit(0);
}

int main(int argc, char *argv[])
{
/* 	usb-2.4G_Smart_Remote-event-kbd */
/*   const char *dev = "/dev/input/by-id/usb-Logitech_USB_Receiver-if02-event-mouse"; */
	const char *dev = argv[1];

	if (argc < 2){
    fprintf(stderr, "Missing device params %s\n", dev);
    fprintf(stderr, "usage: ioreceive /dev/input/event0 \n");
    return EXIT_FAILURE;
	}

  struct input_event ev;
  ssize_t n;
  char name[256]= "Unknown";

  fd = open(dev, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
    return EXIT_FAILURE;
  }

	int ret = ioctl(fd, EVIOCGRAB, GRAB);

  if(ret < 0){
		printf("Couldn't grab %s \n", dev);
		return EXIT_FAILURE;
  }


  char i[BUFFER_SIZE];
/*   fgets(i, BUFSIZ, stdin); */
/*   int shouldQuit = 0; */

/* 	while((fgets(i, BUFSIZ, stdin)) != NULL){ */
	while(1){
		signal(SIGINT, INThandler);
	  struct input_event event;
	  read(fd, &event, sizeof(event));
	  if (event.type == EV_KEY && event.value >= 0 && event.value <= 2){
			fprintf(stdout, "{\"event\":\"key\", \"code\":\"%3d\", \"value\":\"%d\"}", event.code,  event.value);
			fflush(stdout);
/*
	    if(event.code == EXIT_KEY){
	      ioctl(fd, EVIOCGRAB, GRAB);
	      close(fd);
	      return 0;
	    }
*/

	  }
/*
		fgets(i, BUFFER_SIZE, stdin);
		printf("input: %s", i);
		if(strcmp(i, "quit\n") == 0){
			printf("exiting...");
      ioctl(fd, EVIOCGRAB, GRAB);
      close(fd);
			return 0;
		}
*/
	}

/*
  while (strcmp(i, "quit\n") != 0) {
		printf("input: %s\n", i);
		fgets(i, BUFSIZ, stdin);
	}
*/

/*
	while(1){
	  struct input_event event;
	  read(fd, &event, sizeof(event));
	  if (event.type == EV_KEY && event.value >= 0 && event.value <= 2){
			fprintf(stdout, "{\"event\":\"key\", \"key\":\"%3d\", \"value\":\"%d\"}", event.value, event.code);
			fflush(stdout);
	    if(event.code == EXIT_KEY){
	      ioctl(fd, EVIOCGRAB, GRAB);
	      close(fd);
	      return 0;
	    }

	  }

	}
*/
/* 	printf("exited"); */

/*   ioctl(fd, EVIOCGRAB, GRAB); */
/*   close(fd); */

  return 0;
}