#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "low_level.h"
#include "ui.h"

int main (char** argv, int argc) {
	system("clear");
	lcd_ui_init();
	while (1) {
		lcd_ui_handle();
		sleep(1);
	}
	return 1;
}
