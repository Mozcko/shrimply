# Shrimply Vulkan Migration Plan (Phase 2)

## Current State (Phase 1 Completed)
We have successfully decoupled Shrimply from its strict compile-time dependency on NVIDIA hardware. 
- **Mock CUDA Driver**: By implementing `mock_cuda`, the `visual-cuda` crate compiles cleanly on Intel without needing `nvcc` or a real CUDA installation. 
- **Software Decoder**: The `video-decoder` crate now gracefully falls back to CPU-based FFmpeg software decoding (`YUV420P` -> `NV12` conversion) when hardware decoding fails.
- **Result**: The application launches, the UI is fully responsive, project logic works, and audio plays on Intel laptops. However, because the `mock_cuda` stubs bypass kernel launches and memory copies, the video rendering pipeline produces empty (black/transparent) frames.

To achieve functional video rendering on Intel, AMD, and Apple Silicon, Shrimply needs a generic Vulkan compositing backend.

## Roadmap to `visual-vulkan`

### 1. Create the `visual-vulkan` Crate
Shrimply's rendering engine is currently isolated in `crates/media/visual/visual-cuda`. We need to build a sibling crate `crates/media/visual/visual-vulkan`.
- **Dependencies**: Use `ash` (already in workspace for `3dgs-vulkan`) for Vulkan bindings, and `shrimply-visual-core`.
- **Architecture**: Port the structure of `visual-cuda`:
  - `context.rs`: Vulkan device and queue initialization.
  - `memory.rs`: Vulkan memory allocation and texture/buffer management.
  - `compositor.rs`: The rendering loop (equivalent to `CudaVideoCompositor`).
  - `decode.rs`: Logic to receive CPU `VisualFrame`s and upload them via Vulkan staging buffers.

### 2. Implement `preview-runtime-vulkan`
The `preview-runtime-cuda` crate bridges `visual-cuda` with the UI surfaces (OpenGL or Skia). We need `preview-runtime-vulkan`.
- **Interop**: Implement a mechanism to share Vulkan textures with the GTK/Qt UI surfaces. Since Wayland and Windows support Vulkan natively, you can use `VK_EXT_external_memory_host` or `VK_KHR_external_memory_fd` to export the final composited frame to OpenGL/Skia.
- **Provider**: Implement `PreviewProvider` to serve frames to the UI.

### 3. Port Compute Shaders to Slang
The `visual-cuda` crate relies on raw PTX/CUBIN shaders (e.g., `preview.cubin`). Shrimply already has `slang-build` integrated.
- **Action**: Rewrite the core compositing kernels (NV12 to RGBA conversion, transformations, blending, alpha matting) from Rust/CUDA to Slang.
- **Compilation**: Configure `slang-build` to compile these shaders to **SPIR-V** for `visual-vulkan`. (As a bonus, Slang can also compile to PTX, allowing you to eventually replace the Rust CUDA kernels entirely).

### 4. GPU Modifiers and Effects
`visual-cuda` implements GPU-accelerated modifiers (Blur, Anime4K upscaling, Stabilization). 
- For Phase 2, implement **pass-through or fallback** for these effects in `visual-vulkan`. 
- Once basic video playback and transformation work, incrementally port the modifier kernels (like Gaussian Blur) to Slang compute shaders.

### 5. Runtime Backend Selection
In `crates/ui/preview/preview-gtk/src/lib.rs` (and Qt equivalents), implement runtime detection:
- If a Vulkan device is available (Intel/AMD), initialize `preview-runtime-vulkan`.
- If an NVIDIA device is available (and CUDA loads successfully), initialize `preview-runtime-cuda`.

## Instructions for Claude (or other Agents)
1. **Bootstrap**: Create `crates/media/visual/visual-vulkan` and copy the boilerplate structure from `visual-cuda`.
2. **Memory Manager**: Write a simple Vulkan allocator using `ash` that can create `VK_FORMAT_G8_B8R8_2PLANE_420_UNORM` (NV12) images.
3. **Staging Buffer**: Replicate the CPU-to-GPU upload logic from `visual-frame/src/lib.rs` but use a Vulkan staging buffer instead of `cuMemcpy2D`.
4. **Shader**: Write a basic Slang compute shader that samples the NV12 Vulkan texture and outputs to an RGBA storage image.
5. **UI Integration**: Plumb the RGBA texture back to the preview provider. Use `make run` on an Intel environment to test.
