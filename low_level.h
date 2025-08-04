#ifndef _LOW_LEVEL_H_
#define _LOW_LEVEL_H_

#include <stdint.h>
#include <stdlib.h>

#define LCD_ROWS 4
#define LCD_COLS 20
//#define lcd_char(chr) putchar(chr)
#define lcd_str(str) printf("%s", str);fflush(stdout)
#define lcd_cls() system("clear");
#define mediainfo_title_get()	"Radio Złote Przeboje Kraków"
#define lcd_flush_buffer()

#define PORTG dummy_byte
#define PORTE dummy_byte
#define PORTF dummy_byte

#define BUTTON_BUFFER_SIZE 7

#define NO_PRESS 0x00
#define SHORT_PRESS 0x01
#define LONG_PRESS 0x02

#define ROTARY_NEGATIVE 0x01
#define ROTARY_POSITIVE 0x02
#define ROTARY_NEUTRAL 0x00

#define _PORTG_RG13_MASK 0x00
#define _PORTE_RE2_MASK 0x01
#define _PORTE_RE5_MASK 0x02
#define _PORTF_RF2_MASK 0x03
#define ROTARY_IND 0x06

typedef enum {false = 0, true} bool;

typedef struct button {
	uint8_t btn_id;
    void (*push_proc)(void);
    void (*long_proc)(void);
} button_t;

extern uint8_t button_buffer[BUTTON_BUFFER_SIZE];
extern uint8_t dummy_byte;

void low_level_init(void);
void lcd_locate(uint8_t x, uint8_t y);
uint32_t millis (void);
void lcd_char(char chr);
uint16_t lcd_str_part(const char* str, const uint16_t len);
uint16_t lcd_utf8str_part(const char* str, const uint16_t len);
void lcd_str_padd_rest(const char* str, const uint16_t len, char padd);
void lcd_utf8str_padd_rest(const char* str, const uint16_t len, char padd);

char* get_station_url_from_file(uint16_t number, char* working_buffer, size_t working_buffer_len, char* stream_name, size_t stream_name_len);
char* get_file_path_from_audio_file_id(uint16_t number, char* working_buffer, size_t working_buffer_len, char* stream_name, size_t stream_name_len);
uint16_t get_max_stream_id(void);

void button_init(button_t* btn, void*, uint8_t id, void (*push_proc)(void), void (*long_proc)(void));
void button_handle(button_t* btn);
void rotary_init(void);
void rotary_register_callback(void (*cbk)(int8_t));
void VS1053_play_prev(void);
void VS1053_play_next(void);
void VS1053_setVolume(uint8_t new_volume);
uint8_t VS1053_getVolume(void);
char* VS1053_get_state_description(void);
void VS1053_play_http_stream_by_id(const uint16_t selected_stream_id);
int8_t rotary_handle(void);
void button_register_push_callback(button_t *btn, void (*push_proc)(void));
void button_register_long_callback(button_t *btn, void (*long_proc)(void));
void button_register_global_callback(void (*cbk)(void));
void lcd_set_backlight_state(uint8_t newState);

#endif // _LOW_LEVEL_H_
