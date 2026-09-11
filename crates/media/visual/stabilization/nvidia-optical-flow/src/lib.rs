use shrimply_gpu_cuda::{CudaContext, CudaStream, sys};

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
#[repr(u32)]
pub enum Quality {
    Quality = 5,
    Balanced = 10,
    Fast = 20,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
#[repr(u32)]
pub enum OutputGrid {
    OneByOne = 1,
    TwoByTwo = 2,
    FourByFour = 4,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct Settings {
    pub quality: Quality,
    pub output_grid: OutputGrid,
    pub temporal_hints: bool,
}

impl Default for Settings {
    fn default() -> Self {
        Self {
            quality: Quality::Quality,
            output_grid: OutputGrid::TwoByTwo,
            temporal_hints: true,
        }
    }
}

#[derive(Clone, Copy, Default)]
#[repr(C)]
pub struct FlowVector {
    pub x: i16,
    pub y: i16,
}

pub struct FlowField {
    pub forward: Vec<FlowVector>,
    pub backward: Vec<FlowVector>,
    pub forward_cost: Vec<u8>,
    pub backward_cost: Vec<u8>,
    pub width: usize,
    pub height: usize,
    pub grid_size: u32,
}

pub struct OpticalFlow {
    width: u32,
    height: u32,
    settings: Settings,
}

impl OpticalFlow {
    pub fn new(
        _c: &CudaContext,
        _s: &CudaStream,
        width: u32,
        height: u32,
        settings: Settings,
    ) -> Result<Self, String> {
        Ok(Self {
            width,
            height,
            settings,
        })
    }
    pub fn matches(&self, width: u32, height: u32, settings: Settings) -> bool {
        self.width == width && self.height == height && self.settings == settings
    }
    pub fn estimate(
        &mut self,
        _i: sys::CUdeviceptr,
        _r: sys::CUdeviceptr,
        _rst: bool,
    ) -> Result<FlowField, String> {
        Err("NVOF is not supported on Intel GPU".to_string())
    }
}
