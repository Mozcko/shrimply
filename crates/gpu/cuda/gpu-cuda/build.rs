#[cfg(not(target_os = "macos"))]
use std::{env, path::PathBuf};

#[cfg(target_os = "macos")]
fn main() {}

#[cfg(not(target_os = "macos"))]
fn main() {
    println!("cargo:rerun-if-changed=bridge.c");
    let manifest = PathBuf::from(env::var_os("CARGO_MANIFEST_DIR").unwrap());
    let include_path = manifest.join("../../../../external/mock_cuda/include");
    cc::Build::new()
        .file("bridge.c")
        .include(include_path)
        .compile("shrimply_cuda_bridge");
    // Don't link libcuda!
}
