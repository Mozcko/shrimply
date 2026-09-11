//! Vulkan-based cross-platform video rendering and compositing backend for Shrimply.

pub mod context;
pub mod memory;
pub mod compositor;
pub mod decode;

pub use context::VulkanContext;
