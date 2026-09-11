use shrimply_gpu_cuda::{CudaContext, CudaStream};
use std::sync::Arc;

pub struct OptixDenoiser {
    width: u32,
    height: u32,
}

pub struct DenoiseInputs {
    pub beauty: u64,
    pub refraction: u64,
    pub albedo: u64,
    pub normal: u64,
}

impl OptixDenoiser {
    pub fn new(
        _context: Arc<CudaContext>,
        _stream: &CudaStream,
        width: u32,
        height: u32,
    ) -> Result<Self, String> {
        Ok(Self { width, height })
    }

    pub fn denoise(&mut self, _stream: &CudaStream, _inputs: DenoiseInputs) -> Result<(), String> {
        Err("OptiX is not supported on Intel GPU".to_string())
    }

    pub fn matches(&self, width: u32, height: u32) -> bool {
        self.width == width && self.height == height
    }
}
