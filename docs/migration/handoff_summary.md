# Intel Support Migration: Agent Handoff Summary

Welcome! You are taking over a major architectural migration for Shrimply. The goal of this epic is to decouple Shrimply's rendering engine from its strict NVIDIA CUDA lock-in, enabling it to run natively on Intel, AMD, and Apple Silicon GPUs.

## 1. What Has Been Completed (Phase 1)
The previous agent successfully completed the foundational decoupling phase. 
* **Branch**: All work is currently committed and pushed to `feature/intel-support` on the user's fork.
* **Compile-Time Bypass**: We introduced a `mock_cuda` driver. If the build system detects that `nvcc` (the NVIDIA compiler) is missing (like on an Intel laptop), it intercepts the CUDA dependencies and compiles against dummy C wrappers (`bridge.c`). This allows the entire massive `shrimply` workspace to compile successfully on Intel without invasive feature-flag refactoring.
* **Software Decoding Fallback**: The `video-decoder` crate originally crashed if it couldn't create a CUDA device context. We wrote a fallback that launches FFmpeg's software decoder if hardware decoding fails.
* **Format Conversion**: Since FFmpeg software decodes to `AV_PIX_FMT_YUV420P` (which Shrimply's pipeline didn't understand), we wrote a CPU conversion loop in `shrimply_visual_frame::ffmpeg` to transcode it on-the-fly to `NV12`.
* **UI Fixes**: Patched GLSL versioning (`#version 300 es`) for the preview window so it supports Wayland OpenGL ES, and fixed missing timeline icons.

## 2. Current State & Known Limitations
If you run `make run` on an Intel machine right now, the application boots, the UI works, projects load, audio plays, and video decoding succeeds. 

**However, the video preview is completely blank.**
This is expected behavior. The rendering compositor (`crates/media/visual/visual-cuda`) still relies on the `mock_cuda` driver. The mock driver returns "Success" for every GPU command, but memory copies and shader executions do absolutely nothing. Thus, empty frames are returned.

## 3. Your Mission (Phase 2)
To make the video visualizer actually render on Intel, Shrimply needs a cross-platform graphics compositor. Shrimply already uses Vulkan (`ash`) in `crates/media/visual/3d/3dgs/3dgs-vulkan`, so Vulkan is our chosen backend.

**Your exact next steps are mapped out in detail in the `vulkan_compositor_migration.md` artifact.**
Please read that document thoroughly. 

### Immediate Next Step
Your first task is **Step 1** from the migration plan: scaffold the `crates/media/visual/visual-vulkan` crate. 
- You should look at `visual-cuda` as a reference for the module structure (`context.rs`, `memory.rs`, `compositor.rs`).
- Add the new crate to the workspace `Cargo.toml`.
- Initialize basic `ash` Vulkan instance and device boilerplate inside `visual-vulkan`.
