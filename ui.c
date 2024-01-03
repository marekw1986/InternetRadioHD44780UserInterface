#include <stdio.h>
#include <string.h>
#include <time.h>
//#include "hd44780.h"
#include "ui.h"
#include "low_level.h"
//#include "../vs1003/vs1003.h"
//#include "../common.h"

typedef enum {SCROLL, SCROLL_WAIT} scroll_state_t;

static button_t next_btn;
static button_t prev_btn;
static button_t state_button;

static ui_state_t ui_state = UI_HANDLE_MAIN_SCREEN;
static scroll_state_t scroll_state = SCROLL_WAIT;
static uint32_t scroll_timer;
static bool scroll_info = false;
static bool scroll_right = true;
static const char* scroll_begin;
static const char* scroll_ptr;

static uint8_t selected_line = 0;

static void ui_draw_main_screen(void);
static void ui_draw_scrollable_list(void);
static void ui_handle_main_screen(void);
static void ui_handle_scrollable_list(void);
static void ui_handle_updating_time(void);
static void ui_handle_scroll(void);

//Button functions
static void ui_button_switch_state(void);

void ui_init(void) {
    rotary_init();
    button_init(&prev_btn, &PORTG, _PORTG_RG13_MASK, &VS1003_play_prev, NULL);
    button_init(&next_btn, &PORTE, _PORTE_RE2_MASK, &VS1003_play_next, NULL);
    button_init(&state_button, &PORTE, _PORTE_RE4_MASK, &ui_button_switch_state, NULL);
    
	ui_state = UI_HANDLE_MAIN_SCREEN;
	ui_draw_main_screen();
}

void ui_switch_state(ui_state_t new_state) {
	switch(new_state) {
		case UI_HANDLE_MAIN_SCREEN:
		ui_draw_main_screen();
		break;
		
		case UI_HANDLE_SCROLLABLE_LIST:
		ui_draw_scrollable_list();
		break;
	}
}

static void ui_draw_main_screen(void) {
	if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    lcd_cls();
	ui_update_content_info(mediainfo_title_get());
    lcd_locate(3, 0);
    lcd_str("Volume: ");
    ui_update_volume();
}

static void ui_draw_scrollable_list(void) {
	if (ui_state != UI_HANDLE_SCROLLABLE_LIST) { return; }
	int stream_id = 1;
	char buf[32];
	char* url = NULL;
    lcd_cls();
	for (int line=0; line<4; line++) {
		url = get_station_url_from_file(stream_id, buf, sizeof(buf));
		if (url != NULL) {
			lcd_locate(line, 0);
			if (line == selected_line) {
				lcd_str("->");
			}
			else {
				lcd_str("  ");
			}
			lcd_str_padd_rest(buf, LCD_COLS-2, ' ');
		}
		stream_id++;
	}
}

void ui_update_volume(void) {
    char supbuf[16];
    
    if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    uint8_t volume = VS1003_getVolume();
    snprintf(supbuf, sizeof(supbuf)-1, "%d%s", volume, (volume < 100) ? " " : "");
    lcd_locate(3, 8);
    lcd_str(supbuf);
}

void ui_update_content_info(const char* str) {
	if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    if (strlen(str) <= LCD_COLS) {
        lcd_locate(1, 0);
        uint8_t rest = lcd_utf8str_part(str, LCD_COLS);
        while (rest) {
            lcd_char(' ');
            rest--;
        }
    }    
    else {
        scroll_info = true;
        scroll_right = true;
        scroll_begin = scroll_ptr = str;
        scroll_timer = millis();
        scroll_state = SCROLL_WAIT;
        lcd_locate(1, 0);
        lcd_utf8str_part(scroll_begin, LCD_COLS);
    }
    lcd_flush_buffer();
}

void ui_clear_content_info(void) {
	if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    scroll_info = false;
    lcd_locate(1, 0);
    for (int i=0; i<LCD_COLS; i++) {
        lcd_char(' ');
    }
    lcd_flush_buffer();
}

void ui_update_state_info(const char* str) {
	if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    ui_clear_state_info();
    lcd_flush_buffer();
    if (str) {
        lcd_locate(2,0);
        lcd_str(str);
    }
    lcd_flush_buffer();
}

void ui_clear_state_info(void) {
	if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    lcd_locate(2, 0);
    for (int i=0; i<LCD_COLS; i++) {
        lcd_char(' ');
    }
    lcd_flush_buffer();
}

void ui_handle(void) {
    switch(ui_state) {
		case UI_HANDLE_MAIN_SCREEN:
		ui_handle_main_screen();
		break;
		
		case UI_HANDLE_SCROLLABLE_LIST:
		ui_handle_scrollable_list();
		break;
	}
    int8_t tmp;
    if ( (tmp = rotary_handle()) ) {
        int8_t volume = VS1003_getVolume();
        volume += tmp;
        if (volume > 100) volume = 100;
        if (volume < 0) volume = 0;
        VS1003_setVolume(volume);                
    }
    button_handle(&next_btn);
    button_handle(&prev_btn);
}

static void ui_handle_main_screen(void) {
    ui_handle_scroll();
    ui_handle_updating_time();	
}

static void ui_handle_scrollable_list(void) {

}

static void ui_handle_scroll(void) {
    if (!scroll_info) {
        return;
    }
    switch(scroll_state) {
        case SCROLL:
        if (((uint32_t)(millis()-scroll_timer) > 800)) {
            if (scroll_right) {
                scroll_ptr++;
                lcd_locate(1, 0);
                lcd_utf8str_padd_rest(scroll_ptr, LCD_COLS, ' ');
                if (*scroll_ptr & (1<<7)) { scroll_ptr++; }
                if (scroll_ptr >= (scroll_begin+strlen(scroll_begin))-LCD_COLS) {
                    scroll_right = false;
                    scroll_state = SCROLL_WAIT;
                }
            }
            else {
                scroll_ptr--;
                lcd_locate(1, 0);
                if (scroll_ptr <= (scroll_begin)) {
                    scroll_right = true;
                    scroll_state = SCROLL_WAIT;
                }
                else {
					if ( *(scroll_ptr-1) & (1<<7)) { scroll_ptr--; }	
				}
                lcd_utf8str_padd_rest(scroll_ptr, LCD_COLS, ' ');
            }
    //        SYS_CONSOLE_PRINT("Whole: %s\r\n", scroll_buffer);
    //        SYS_CONSOLE_PRINT("Window: %s\r\n", supbuf);            
            scroll_timer = millis();
        }             
        break;
        
        case SCROLL_WAIT:
        if (((uint32_t)(millis()-scroll_timer) > 3000)) {
            scroll_timer = millis();
            scroll_state = SCROLL;
        }
        break;
        
        default:
        break;
    }
}

void ui_handle_updating_time(void) {
    static uint8_t last_second = 0;
    
    time_t rawtime = time(NULL);
    struct tm* current_time = localtime(&rawtime);
    
    if (ui_state != UI_HANDLE_MAIN_SCREEN) { return; }
    if (current_time->tm_sec != last_second) {
        last_second = current_time->tm_sec;
        char supbuf[32];
        snprintf(supbuf, sizeof(supbuf), "%02d:%02d:%02d  %02d-%02d-%04d", (uint8_t)current_time->tm_hour, (uint8_t)current_time->tm_min, (uint8_t)current_time->tm_sec, (uint8_t)current_time->tm_mday, (uint8_t)current_time->tm_mon+1, (uint16_t)current_time->tm_year+1900);
        lcd_locate(0, 0);
        lcd_str(supbuf);
    }  
}

// Button functions
static void ui_button_switch_state(void) {
    if (ui_state == UI_HANDLE_MAIN_SCREEN) {
        ui_switch_state(UI_HANDLE_SCROLLABLE_LIST);
    }
    else {
        ui_switch_state(UI_HANDLE_MAIN_SCREEN);
    }
}
