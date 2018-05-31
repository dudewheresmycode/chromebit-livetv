/* #include <linux/input.h> */

#include <stdlib.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "display.h"


#define BUFFER_SIZE 16

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}



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

void fullscreen(int fd){
	emit(fd, EV_KEY, KEY_F11, 1);
	emit(fd, EV_SYN, SYN_REPORT, 0);
	emit(fd, EV_KEY, KEY_F11, 0);
	emit(fd, EV_SYN, SYN_REPORT, 0);
	sleep(1);
}
void mousemove(int fd, int x, int y){
	emit(fd, EV_REL, REL_X, x);
	emit(fd, EV_REL, REL_Y, y);
	emit(fd, EV_SYN, SYN_REPORT, 0);
	usleep(15000);
}
void scroll(int fd, int amnt){
	emit(fd, EV_REL, REL_WHEEL, amnt);
	emit(fd, EV_SYN, SYN_REPORT, 0);
	usleep(15000);
}
void click(int fd){
	emit(fd, EV_KEY, BTN_LEFT, 1);
	emit(fd, EV_SYN, SYN_REPORT, 0);
	emit(fd, EV_KEY, BTN_LEFT, 0);
	emit(fd, EV_SYN, SYN_REPORT, 0);
	usleep(15000);
}

void stepmouse(int fd, int x_d, int y_d){


/* 	int x_step_len = 50; //pixels per movement */
/* 	int y_step_len = 50; //pixels per movement */
/* 	if(abs(x_d) < 100){ x_step_len = x_d; } */
/* 	if(abs(y_d) < 100){ y_step_len = y_d; } */

	if(x_d != 0){
		emit(fd, EV_REL, REL_X, x_d);
	}
	if(x_d != 0){
		emit(fd, EV_REL, REL_Y, y_d);
	}
	emit(fd, EV_SYN, SYN_REPORT, 0);
	usleep(15000);

/* 	fprintf(stderr, "stepmouse: %d,%d / %d %d \n", x_d, y_d, x_step_len, y_step_len); */

/* 	int x_steps = ceil(abs(x_d)/x_step_len); */
/* 	fprintf(stderr, "stepmouse: %s", x_steps); */

/*
	if(x_d != 0){
		int x_steps = ceil(abs(x_d)/x_step_len);
		fprintf(stderr, "x_steps: %d \n", x_steps);
		//move x
		int x_it = 0;
		while(x_it <= x_steps){
			int x = ((x_it+x_steps) / x_steps) * x_d;
			emit(fd, EV_REL, REL_X, x);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			usleep(35000);
		}
	}

	if(y_d != 0){
		//move y
		int y_steps = ceil(abs(y_d)/y_step_len);
		fprintf(stderr, "y_steps: %d \n", y_steps);
		int y_it = 0;
		while(y_it <= y_steps){
			int y = ((y_it+y_steps) / y_steps) * y_d;
			emit(fd, EV_REL, REL_Y, y);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			usleep(35000);
		}
	}
*/

}


void resetmouse(int fd){
	//reset mouse

	struct screenInfo vinfo = get_display();
/* 	fprintf(stdout, "{\"width\":\"%d\", \"height\":\"%d\", \"bpp\":\"%d\"}", vinfo.x, vinfo.y, vinfo.bpp); */

	//move to extent
	stepmouse(fd, -1000, -1000);
	stepmouse(fd, 500, 100);
	//then halfwayish back up and to the left
	sleep(2);
/*
	int half_y = (bits_y/2) * -1;
	int half_x = (bits_x/2) * -1;
	fprintf(stderr, "move back %d, %d", half_x, half_y);
	stepmouse(fd, half_x, half_y);
*/

/* 	mousemove(fd, -100, -100); */
/* 	mousemove(fd, -100, -100); */
}

int starts_with(const char *a, const char *b){
	if(strncmp(a, b, strlen(b)) == 0) return 0;
	return 1;
}


char** str_split(char* a_str, const char a_delim){
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int* strnumber(char *num){
	int dec = 0, i, j, len;
	len = strlen(num);
	for(i=0; i<len; i++){
		dec = dec * 10 + ( num[i] - '0' );
	}
	return dec;
}
int main(int argc, char **argv)
{
/*
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
*/

	struct uinput_user_dev uud;
	int version, rc, fd;

	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

	if(fd < 0){
		printf("error");
		return 0;
	}
	sleep(1);

	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);
	ioctl(fd, UI_SET_KEYBIT, KEY_X);
	ioctl(fd, UI_SET_KEYBIT, KEY_F11);

	if(ioctl(fd, UI_SET_EVBIT, EV_SYN) < 0)
	  printf("error: ioctl");
	if(ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
	  printf("error: ioctl");
  if(ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
		printf("error: ioctl");
	if(ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
		printf("error: ioctl");
	if(ioctl(fd, UI_SET_RELBIT, REL_WHEEL) < 0)
		printf("error: ioctl");
	if(ioctl(fd, UI_SET_KEYBIT, BTN_MOUSE) < 0)
		printf("error: ioctl");
	if(ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
		printf("error: ioctl");
	if(ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT) < 0)
		printf("error: ioctl");

	if(ioctl(fd, UI_SET_EVBIT, EV_ABS) < 0)
		printf("error: ioctl");
	if(ioctl(fd, UI_SET_ABSBIT, ABS_WHEEL) < 0)
		printf("error: ioctl");

/*
	if(ioctl(fd, UI_SET_ABSBIT, ABS_X) < 0)
		printf("error: ioctl");
	if(ioctl(fd, UI_SET_ABSBIT, ABS_Y) < 0)
		printf("error: ioctl");
*/

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

/* 	resetmouse(fd); */

	fprintf(stdout, "{\"event\":\"ready\"}");
	fflush(stdout);
  char i[BUFFER_SIZE];
  fgets(i, BUFSIZ, stdin);
  while (strcmp(i, "quit\n") != 0) {

		char *clean = trimwhitespace(i);
		//fprintf(stdout, "{\"event\":\"key\", \"key\":\"%s\"}", i);
		//fflush(stdout);

		if (strcmp("fs", clean) == 0){
			fullscreen(fd);
			fprintf(stdout, "{\"event\":\"fullscreen\", \"key\":\"%s\"}", clean);
			fflush(stdout);
			//wait for keyevent to propalgate
		}
		if (starts_with(i, "mv") == 0){
			char** position = str_split(clean, ',');
      int *x = strtol(position[1], NULL, 0);
      int *y = strtol(position[2], NULL, 0);

/* 			int xx = strnumber(position[1]); */
/* 			int yy = strnumber(position[2]); */
/* 			int *x = &xx; */
/* 			int *y = &yy; */
/* 			fprintf(stderr, "found: %d, %d", x, y); */
			stepmouse(fd, x, y);
/* 			fprintf(stdout, "{\"event\":\"guide_show\", \"key\":\"%s\"}", clean); */
			fflush(stdout);
		}

		if (strcmp("gs", clean) == 0){
			stepmouse(fd, -500, 0);
			fprintf(stdout, "{\"event\":\"guide_show\", \"key\":\"%s\"}", clean);
			fflush(stdout);
		}
		if (strcmp("gh", clean) == 0){
			stepmouse(fd, 500, 0);
			fprintf(stdout, "{\"event\":\"guide_hide\", \"key\":\"%s\"}", clean);
			fflush(stdout);
		}
		//playpause (space bar)
		if (strcmp("pp", clean) == 0){
			emit(fd, EV_KEY, KEY_SPACE, 1);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			emit(fd, EV_KEY, KEY_SPACE, 0);
			emit(fd, EV_SYN, SYN_REPORT, 0);
			sleep(1);
			fprintf(stdout, "{\"event\":\"space\", \"key\":\"%s\"}", clean);
			fflush(stdout);
		}

		if (strcmp("click", clean) == 0){
			click(fd);
			fprintf(stdout, "{\"event\":\"click\", \"key\":\"%s\"}", clean);
			fflush(stdout);
		}
	
	  fgets(i, BUFSIZ, stdin);
  }



/*
   memset(&uud, 0, sizeof(uud));
   snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "uinput old interface");
   write(fd, &uud, sizeof(uud));
*/

	ioctl(fd, UI_DEV_DESTROY);
	close(fd);
//	fprintf(stdout, "{\"event\":\"closed\"}");
//	fflush(stdout);

	return 0;
}
