#ifndef _LOW_LEVEL_H_
#define _LOW_LEVEL_H_

#include <stdint.h>

#define LCD_COLS 20
#define lcd_char(chr) putchar(chr)
#define lcd_str(str) printf("%s", str);fflush(stdout)
#define VS1003_getVolume() 50
#define mediainfo_title_get()	"Radio Złote Przeboje Kraków"
#define lcd_flush_buffer()

typedef enum {false = 0, true} bool;

void lcd_locate(uint8_t x, uint8_t y);
uint32_t millis (void);
uint16_t lcd_str_part(const char* str, const uint16_t len);
uint16_t lcd_utf8str_part(const char* str, const uint16_t len);
void lcd_str_padd_rest(const char* str, const uint16_t len, char padd);
void lcd_utf8str_padd_rest(const char* str, const uint16_t len, char padd);

char* get_station_url_from_file(uint16_t number, char* stream_name, size_t stream_name_len);

#endif // _LOW_LEVEL_H_
