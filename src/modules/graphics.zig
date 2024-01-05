const std = @import("std");
const c = @cImport({
    @cInclude("SDL2/SDL.h");
    @cInclude("graphics.h");
});

const Graphics = struct {
    renderer: *c.SDL_Renderer,
    window: *c.SDL_Window,
    screen_width: i32,
    screen_height: i32,
};

fn initSDL() !void {
    if (c.SDL_Init(c.SDL_INIT_VIDEO) < 0) {
        return std.log.err("Could not initialize SDL2: {s}", .{c.SDL_GetError()});
    }
}

export fn graphics_init(screen_width: i32, screen_height: i32) ?*Graphics {
    const allocator = std.heap.page_allocator;

    if (c.SDL_Init(c.SDL_INIT_VIDEO) < 0) {
        std.log.err("Could not initialize SDL2: {s}", .{c.SDL_GetError()});
        return null;
    }

    const window = c.SDL_CreateWindow("spritely", c.SDL_WINDOWPOS_CENTERED, c.SDL_WINDOWPOS_CENTERED, screen_width, screen_height, c.SDL_WINDOW_SHOWN | c.SDL_WINDOW_RESIZABLE);
    if (window == null) {
        std.log.err("Could not create window: {s}", .{c.SDL_GetError()});
        return null;
    }

    const renderer = c.SDL_CreateRenderer(window, -1, c.SDL_RENDERER_ACCELERATED);
    if (renderer == null) {
        std.log.err("Could not create a renderer: {s}", .{c.SDL_GetError()});
        c.SDL_DestroyWindow(window);
        return null;
    }

    const sdlResult = c.SDL_RenderSetLogicalSize(renderer, screen_width, screen_height);
    if (sdlResult != 0) {
        std.log.err("SDL_RenderSetLogicalSize failed: {s}", .{c.SDL_GetError()});
        c.SDL_DestroyRenderer(renderer);
        c.SDL_DestroyWindow(window);
        return null;
    }

    _ = c.SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    const graphics = allocator.create(Graphics) catch {
        std.log.err("Failed to allocate memory for Graphics", .{});
        c.SDL_DestroyRenderer(renderer orelse return null);
        c.SDL_DestroyWindow(window);
        return null;
    };

    graphics.* = Graphics{
        .renderer = renderer orelse return null,
        .window = window orelse return null,
        .screen_width = screen_width,
        .screen_height = screen_height,
    };

    return graphics;
}

export fn graphics_destroy(g: *Graphics) void {
    c.SDL_DestroyWindow(g.window);
    c.SDL_DestroyRenderer(g.renderer);
    c.SDL_Quit();
}

export fn graphics_set_draw_color(g: *Graphics, r: u8, g1: u8, b: u8, a: u8) void {
    _ = c.SDL_SetRenderDrawColor(g.renderer, r, g1, b, a);
}

fn rect_to_sdl_rect(rect: *const c.Rect) c.SDL_Rect {
    var sdlRect: c.SDL_Rect = undefined;
    sdlRect.x = rect.x;
    sdlRect.y = rect.y;
    sdlRect.w = rect.w;
    sdlRect.h = rect.h;
    return sdlRect;
}

export fn graphics_draw_rect(g: *Graphics, rect: *const c.Rect) void {
    var r = rect_to_sdl_rect(rect);
    _ = c.SDL_RenderDrawRect(g.renderer, &r);
}

export fn graphics_fill_rect(g: *Graphics, rect: *const c.Rect) void {
    var r = rect_to_sdl_rect(rect);
    _ = c.SDL_RenderFillRect(g.renderer, &r);
}

export fn graphics_clear(g: *Graphics) void {
    _ = c.SDL_RenderClear(g.renderer);
}

export fn graphics_render_present(g: *Graphics) void {
    c.SDL_RenderPresent(g.renderer);
}

export fn graphics_get_screen_width(g: *Graphics) i32 {
    return g.screen_width;
}

export fn graphics_get_screen_height(g: *Graphics) i32 {
    return g.screen_height;
}
