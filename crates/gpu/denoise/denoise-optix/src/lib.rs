use shrimply_gpu_cuda::{CudaContext, CudaStream, sys};
pub struct OptixDenoiser;
impl OptixDenoiser {
    pub fn new(
        _context: &CudaContext,
        _stream: &CudaStream,
        _width: u32,
        _height: u32,
        _albedo: bool,
        _normals: bool,
    ) -> Result<Self, String> {
        Ok(Self)
    }
    pub fn matches(&self, _width: u32, _height: u32, _albedo: bool, _normals: bool) -> bool {
        true
    }
    pub fn compute_intensity(
        &mut self,
        _color: sys::CUdeviceptr,
        _intensity: sys::CUdeviceptr,
    ) -> Result<(), String> {
        Err("Optix not supported".into())
    }
    pub fn invoke(
        &mut self,
        _color: sys::CUdeviceptr,
        _albedo: Option<sys::CUdeviceptr>,
        _normals: Option<sys::CUdeviceptr>,
        _output: sys::CUdeviceptr,
        _intensity: sys::CUdeviceptr,
        _blend: f32,
    ) -> Result<(), String> {
        Err("Optix not supported".into())
    }
}
