#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include "low_level.h"
#include "ui.h"

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

int main (char** argv, int argc) {
	set_conio_terminal_mode();
	system("clear");
	low_level_init();
	ui_init();
	while (1) {
		if (kbhit()) {
			int data = getch();
			if (data >= 0) {
				char c = (char)data;
				fflush(stdout);
				
				if (c == 'q') break;
				switch(c) {
					case 's':
					button_buffer[_PORTE_RE5_MASK] = SHORT_PRESS;
					break;
					
					case 'S':
					button_buffer[_PORTE_RE5_MASK] = LONG_PRESS;
					break;
				}
			}
		}
		ui_handle();
		sleep(1);
	}
	system("clear");
	return 1;
}
