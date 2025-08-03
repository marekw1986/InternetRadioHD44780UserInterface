#ifndef SCROLLABLE_LIST_H
#define	SCROLLABLE_LIST_H

#include <stdint.h>

typedef struct {
	char* (*get_content)(uint16_t, char*, size_t, char*, size_t);
	uint16_t (*get_max_item_id)(void);
} scrollable_list_callbacks_t;

#ifdef	__cplusplus
extern "C" {
#endif

void handle_scrollable_list(void);
void scrollable_list_set_callbacks(const scrollable_list_callbacks_t cbks);
void draw_scrollable_list(void);
void scrollable_list_move_cursor(int8_t val);
int32_t scrollable_list_get_selected_item_id(void);
void scrollable_list_set_selected_item_id(uint16_t id);
void scrollable_list_increment_selected_item_id_by_lcd_rows(void);
void scrollable_list_decrement_selected_item_id_by_lcd_rows(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SCROLLABLE_LIST_H */
