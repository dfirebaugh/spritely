const std = @import("std");
const Build = std.Build;

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "spritely",
        .target = target,
        .root_source_file = .{ .path = "src/main.c" },
        .optimize = optimize,
    });
    exe.addCSourceFiles(&c_src_files, &.{});
    exe.addIncludePath(Build.LazyPath.relative("include"));
    exe.linkLibC();
    exe.linkSystemLibrary("SDL2");
    // exe.addStaticLibrary("SDL2");
    // exe.linkSystemLibrary("SDL2_ttf");
    // exe.linkSystemLibrary("SDL2_image");
    // const zig_sdl = b.dependency("sdl", .{
    //     .target = target,
    //     .optimize = .ReleaseFast,
    // });
    // exe.linkLibrary(zig_sdl.artifact("SDL2"));

    b.installArtifact(exe);
}

const c_src_files = [_][]const u8{
    "src/apprt.c",
    "src/input.c",
    "src/event.c",
    "src/color.c",
    "src/state.c",
    "src/palette.c",
    "src/graphics.c",
    "src/grid_context.c",
    "src/sprite_editor.c",
    "src/pixel_buffer.c",
    "src/editor_tool.c",
    "src/canvas.c",
    "src/sprite_picker.c",
};
