#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "low_level.h"

uint8_t dummy_byte = 0;

void lcd_locate(uint8_t x, uint8_t y) {
	printf("%c[%d;%df",0x1B,x+1,y);
	fflush(stdout);
}

uint32_t millis (void) {
  struct timeval t ;
  gettimeofday ( & t , NULL ) ;
  return t.tv_sec * 1000 + ( t.tv_usec + 500 ) / 1000 ;
}

uint16_t lcd_str_part(const char* str, const uint16_t len) {
    register char znak;
    uint16_t cnt = 0;
    while ( (znak=*(str++)) ) {
        lcd_char( znak );
        cnt++;
        if (cnt >= len) {
            break;
        }
    }
    fflush(stdout);
    return len-cnt;
}

uint16_t lcd_utf8str_part(const char* str, const uint16_t len) {
    char znak;
    uint16_t cnt=0;
    while ( (znak=*str) ) {
        str++;
        if (znak & (1<<7)) {
            uint16_t znak_utf = znak<<8 | (uint8_t)*str;
            switch (znak_utf) {
                case 0xC485:
                lcd_char('a');
                break;

                case 0xC487:
                lcd_char('c');
                break;
                
                case 0xC499:
                lcd_char('e');
                break;
                
                case 0xC582:
                lcd_char('l');
                break;
                
                case 0xC584:
                lcd_char('n');
                break;
                
                case 0xC3B3:
                lcd_char('o');
                break;
                
                case 0xC59B:
                lcd_char('s');
                break;
                
                case 0xC5BA:
                case 0xC5BC:
                lcd_char('z');
                break;
                
                case 0xC484:
                lcd_char('A');
                break;
                
                case 0xC486:
                lcd_char('C');
                break;
                
                case 0xC498:
                lcd_char('E');
                break;
                
                case 0xC581:
                lcd_char('L');
                break;
                
                case 0xC583:
                lcd_char('N');
                break;
                
                case 0xC393:
                lcd_char('O');
                break;
                
                case 0xC59A:
                lcd_char('S');
                break;
                
                case 0xC5B9:
                case 0xC5BB:
                lcd_char('Z');
                break;
                
                default:
                lcd_char('?');
                break;
            }
            str++;
        }
        else {
            lcd_char(znak);
        }
        cnt++;
        if (cnt >= len) {
            break;
        }
    }
    fflush(stdout);
    return len-cnt;
}

void lcd_str_padd_rest(const char* str, const uint16_t len, char padd) {
	uint8_t rest = lcd_str_part(str, LCD_COLS);
	while (rest) {
		lcd_char(padd);
		rest--;
	}
	fflush(stdout);
}

void lcd_utf8str_padd_rest(const char* str, const uint16_t len, char padd) {
	uint8_t rest = lcd_utf8str_part(str, LCD_COLS);
	while (rest) {
		lcd_char(padd);
		rest--;
	}
	fflush(stdout);
}

char* get_station_url_from_file(uint16_t number, char* stream_name, size_t stream_name_len) {
	char* res = NULL;
	switch(number) {
		case 1:
		strncpy(stream_name, "Antyradio", stream_name_len);
		res = "http://an01.cdn.eurozet.pl/ant-waw.mp3";
		break;
		
		case 2:
		strncpy(stream_name, "PR1", stream_name_len);
		res = "http://stream3.polskieradio.pl:8900/";
		break;
		
		case 3:
		strncpy(stream_name, "PR2", stream_name_len);
		res = "http://stream3.polskieradio.pl:8902/";
		break;
		
		case 4:
		strncpy(stream_name, "PR3", stream_name_len);
		res = "http://stream3.polskieradio.pl:8904/";
		break;
		
		case 5:
		strncpy(stream_name, "PR24", stream_name_len);
		res = "http://stream3.polskieradio.pl:8080/";
		break;
		
		case 6:
		strncpy(stream_name, "Kraków (32 kbps)", stream_name_len);
		res = "http://stream4.nadaje.com:9678/radiokrakow-s2";
		break;
		
		case 7:
		strncpy(stream_name, "Kraków", stream_name_len);
		res = "http://stream4.nadaje.com:9680/radiokrakow-s3";
		break;
		
		default:
		strncpy(stream_name, "", stream_name_len);
		res = NULL;
		break;
	}
	return res;
}

void button_init(button_t* btn, void*, uint8_t, void (*push_proc)(void), void (*long_proc)(void)) {
	
}

void button_handle(button_t* btn) {
	
}

void rotary_init(void) {}
void VS1003_play_prev(void) {}
void VS1003_play_next(void) {}
void VS1003_setVolume(uint8_t) {}
int8_t rotary_handle(void) { return 0; }
