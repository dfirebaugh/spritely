
/* Pass save_individual_sprites as non-zero to
*   also output individual images for each sprite.
*   The file naming scheme for individual images is appending "_(XX,YY)"
*   where XX and YY are the x and y positions of the sprite in the grid.
*   (00,00) is the top left sprite
*/
extern void save_file(int save_individual_sprites);
extern void open_file();
