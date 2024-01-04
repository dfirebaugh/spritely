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

    const gfx = b.addStaticLibrary(.{
        .name = "gfx",
        .optimize = optimize,
        .target = target,
        .root_source_file = .{ .path = "src/modules/graphics.zig" },
    });

    if (target.isNativeOs() and target.getOsTag() == .linux) {
        // The SDL package doesn't work for Linux yet, so we rely on system
        // packages for now.
        gfx.linkSystemLibrary("SDL2");
        gfx.linkLibC();
    } else {
        const sdl_dep = b.dependency("sdl", .{
            .optimize = .ReleaseFast,
            .target = target,
        });
        gfx.linkLibrary(sdl_dep.artifact("SDL2"));
    }

    gfx.addIncludePath(Build.LazyPath.relative("include"));

    exe.addCSourceFiles(&c_src_files, &.{});
    exe.linkLibrary(gfx);
    exe.addIncludePath(Build.LazyPath.relative("include"));
    exe.linkLibC();

    const run_cmd = b.addRunArtifact(exe);
    const run_step = b.step("run", "Run spritely");
    run_step.dependOn(&run_cmd.step);

    b.installArtifact(exe);
}

const c_src_files = [_][]const u8{
    "src/apprt.c",
    "src/input.c",
    "src/color.c",
    "src/color_picker.c",
    "src/state.c",
    "src/palette.c",
    "src/grid_context.c",
    "src/sprite_editor.c",
    "src/pixel_buffer.c",
    "src/editor_tool.c",
    "src/canvas.c",
    "src/sprite_sheet.c",
};
