/* #include <linux/input.h> */

#include <stdlib.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>



void emit(int fd, int type, int code, int val)
{
   struct input_event ie;

   ie.type = type;
   ie.code = code;
   ie.value = val;
   /* timestamp values below are ignored */
   ie.time.tv_sec = 0;
   ie.time.tv_usec = 0;

   write(fd, &ie, sizeof(ie));
}

void key(int fd, int code){
	emit(fd, EV_KEY, code, 1);
	emit(fd, EV_SYN, SYN_REPORT, 0);
	emit(fd, EV_KEY, code, 0);
	emit(fd, EV_SYN, SYN_REPORT, 0);
	usleep(50000);
}

int main(void)
{

	struct uinput_user_dev uud;
	int version, rc, fd;

	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	if(fd < 0){
		printf("error");
		return 0;
	}
	sleep(1);

	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ioctl(fd, UI_SET_EVBIT, EV_SYN);
	ioctl(fd, UI_SET_KEYBIT, KEY_ENTER);
	ioctl(fd, UI_SET_KEYBIT, KEY_3);
	ioctl(fd, UI_SET_KEYBIT, KEY_0);
	ioctl(fd, UI_SET_KEYBIT, KEY_6);

  memset(&uud, 0, sizeof(uud));
  snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "keyboard-emulator");
  uud.id.bustype = BUS_USB;
  uud.id.vendor  = 0x1234;
  uud.id.product = 0x5678;
  uud.id.version = 1;

	if(write(fd, &uud, sizeof(uud)) < 0){
		printf("write error");
	}

	if(ioctl(fd, UI_DEV_CREATE) < 0){
		printf("create error");
	}


	//wait for virtual device to propalgate
	sleep(1);

	//enter password
	key(fd, KEY_3);
	key(fd, KEY_0);
	key(fd, KEY_6);
	key(fd, KEY_3);

	key(fd, KEY_3);
	key(fd, KEY_0);
	key(fd, KEY_6);
	key(fd, KEY_3);
	
	key(fd, KEY_ENTER);


	//clean up
	ioctl(fd, UI_DEV_DESTROY);
	close(fd);

	return 0;
}
