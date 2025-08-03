#ifndef SCROLLABLE_LIST_H
#define	SCROLLABLE_LIST_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

void handle_scrollable_list(void);
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
