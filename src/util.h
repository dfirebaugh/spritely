
/* Assumes p is a valid color_t value */
extern void set_pixel_render_color(color_t p);

/* Get the enum color value for the given RGBA values, or BLACK if no enum value matches */
color_t get_pixel_render_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

extern void *checked_malloc(size_t size);

/* Return the index of the last occurrence of ch in buf
*   else return buf_len if ch is not present
*/
extern size_t find_last_occurrence(const char *const buf, const size_t buf_len, const char ch);

/*
* sprite_sheet_index_in_range
* Return 1 if the index is within the spritesheet range
*/
extern int sprite_sheet_index_in_range(const unsigned int index);

/*
* canvas_index_in_range
* Return 1 if the index is within the spritesheet range
*/
extern int canvas_index_in_range(const unsigned int index);
