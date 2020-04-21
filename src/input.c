#include "input.h"

void print_sprite(const char sprite_sheet_index)
{
    int i;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
        printf("%x", sprite_sheet[sprite_sheet_index][i]);

    printf("\n");
}

void push_to_sprite_sheet(const char sprite_sheet_index, const char *spr)
{
    char i;
    for (i = 0; i < SPRITE_CANVAS_SIZE; i++)
        sprite_sheet[sprite_sheet_index][i] = spr[i];
    print_sprite(sprite_sheet_index);
}

uint *get_sprite(const char index)
{
    print_sprite(index);
    return sprite_sheet[index];
}

static void canvas_to_spritesheet(const char sprite_sheet_index, context *ctx)
{
    char spr[SPRITE_CANVAS_SIZE];
    char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    {
        spr[i] = sprite_canvas_ctx.pixels[i].color;
    }
    push_to_sprite_sheet(sprite_sheet_index, spr);
}

static void sprite_canvas_left_click(context *ctx)
{
    context *current_cell = sprite_sheet_cells+current_sprite;

    char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    {
        if (XYInRect(ctx->pixels[i].rect))
        {
            ctx->pixels[i].color = main_color;
            current_cell->pixels[i].color = main_color;
            canvas_to_spritesheet(i, ctx);
        }
    }
}

static void sprite_canvas_right_click(context *ctx)
{
    char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    {
        if (XYInRect(ctx->pixels[i].rect))
        {
            main_color = sprite_canvas_ctx.pixels[i].color;
        }
    }
}

static void sprite_sheet_left_click(context *ctx)
{

    char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
    {
        if (XYInRect(ctx->pixels[i].rect))
        {
            current_sprite = i;
            printf("%d\n", current_sprite);
        }
    }
}

static void color_picker_click(context *ctx)
{
    char i;
    for (i = 0; i < (ctx->row_size * ctx->col_size); i++)
        if (XYInRect(ctx->pixels[i].rect))
            main_color = i;
}

void process_inputs()
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

            case SDLK_SPACE:
                break;

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            break;

        case SDL_USEREVENT:
            break;

        default:
            break;
        }
    }
}
