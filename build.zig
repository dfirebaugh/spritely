const std = @import("std");
const Build = std.Build;

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const spritely = b.addExecutable(.{
        .name = "spritely",
        .target = target,
        .root_source_file = .{ .path = "src/main.c" },
        .optimize = optimize,
    });
    spritely.addCSourceFiles(&c_src_files, &.{});
    spritely.addIncludePath(Build.LazyPath.relative("include"));
    spritely.linkSystemLibrary("c");
    spritely.linkSystemLibrary("SDL2");
    spritely.linkSystemLibrary("SDL2_ttf");
    spritely.linkSystemLibrary("SDL2_image");

    b.installArtifact(spritely);
}

const c_src_files = [_][]const u8{
    "src/spritely.c",
    "src/init.c",
    "src/util.c",
    "src/context.c",
    "src/file.c",
    "src/message_queue.c",
    "src/globals.c",
    "src/colors.c",
    "src/sfd.c",
    "src/draw_tools.c",
    "src/js_api.c",
    "src/entity.c",
    "src/sprite_sheet.c",
    "src/app_state.c",
    "src/sprite_editor.c",
};
