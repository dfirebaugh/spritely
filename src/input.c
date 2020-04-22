#include "input.h"

static int XYInRect(const SDL_Rect rect)
{
    return ((mouse.x >= rect.x && mouse.x <= rect.x + rect.w) && (mouse.y >= rect.y && mouse.y <= rect.y + rect.h));
}

static void sprite_canvas_left_click(context *ctx)
{
    context *current_cell = sprite_sheet_cells + current_sprite;

    char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    {
        if (XYInRect(ctx->rects[i]))
        {
            ctx->pixels[i] = main_color;

            /* copy pixels from canvas to spritesheet */
            memcpy(current_cell->pixels, sprite_canvas_ctx.pixels, sizeof(ctx->pixels));
        }
    }
}

static void sprite_canvas_right_click(context *ctx)
{
    char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    {
        if (XYInRect(ctx->rects[i]))
        {
            main_color = sprite_canvas_ctx.pixels[i];
        }
    }
}

static void cursor_move_sprite_selection(context *ctx)
{
    memcpy(sprite_selection_cursor.rects, ctx->rects, sizeof(ctx->rects));
}

static void sprite_sheet_left_click(context *ctx)
{
    char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    {
        if (XYInRect(ctx->rects[i]))
        {
            current_sprite = i;

            /* copy pixels from spritesheet to canvas */
            memcpy(sprite_canvas_ctx.pixels, sprite_sheet_cells[i].pixels, sizeof(ctx->pixels));
            cursor_move_sprite_selection(&sprite_sheet_cells[i]);
        }
    }
}

static void color_picker_click(context *ctx)
{
    char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
        if (XYInRect(ctx->rects[i]))
            main_color = i;
}

static void buffer_copy_from_canvas()
{
    memcpy(pixel_buffer, sprite_canvas_ctx.pixels, sizeof(sprite_canvas_ctx.pixels));
}

static void buffer_copy_to_canvas()
{
    memcpy(sprite_canvas_ctx.pixels, pixel_buffer, sizeof(sprite_canvas_ctx.pixels));
    memcpy(sprite_sheet_cells[current_sprite].pixels, pixel_buffer, sizeof(sprite_canvas_ctx.pixels));
}

extern void process_inputs()
{
    SDL_Event event;

    /* Loop through waiting messages and process them */
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {

        /* Closing the Window or pressing Escape will exit the program */
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_MOUSEMOTION:
            mouse.x = event.motion.x;
            mouse.y = event.motion.y;
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                sprite_canvas_left_click(&sprite_canvas_ctx);
                color_picker_click(&color_picker_ctx);
                sprite_sheet_left_click(&select_window_ctx);
                break;
            case SDL_BUTTON_RIGHT:
                sprite_canvas_right_click(&sprite_canvas_ctx);
                color_picker_click(&color_picker_ctx);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                sprite_canvas_left_click(&sprite_canvas_ctx);
                color_picker_click(&color_picker_ctx);
                sprite_sheet_left_click(&select_window_ctx);
                break;
            case SDL_BUTTON_RIGHT:
                sprite_canvas_right_click(&sprite_canvas_ctx);
                color_picker_click(&color_picker_ctx);
                break;
            default:
                break;
            }
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                exit(0);
                break;

            case SDLK_s:
            case SDLK_DOWN:
                if (lctrl)
                    save_file();
                break;

            case SDLK_d:
            case SDLK_RIGHT:
                break;

            case SDLK_a:
            case SDLK_LEFT:
                break;

            case SDLK_w:
            case SDLK_UP:
                break;

            case SDLK_r:
                break;
            case SDLK_LCTRL:
                lctrl = 1;
                break;
            case SDLK_c:
                if (lctrl)
                    printf("copy\n");
                buffer_copy_from_canvas();
                break;
            case SDLK_v:
                if (lctrl)
                    printf("paste\n");
                buffer_copy_to_canvas();
                break;
            case SDLK_SPACE:
                break;

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_LCTRL:
                lctrl = 0;
                break;

            default:
                break;
            }

        case SDL_USEREVENT:
            break;

        default:
            break;
        }
    }
}
