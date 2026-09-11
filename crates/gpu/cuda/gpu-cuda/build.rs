#[cfg(not(target_os = "macos"))]
use std::{env, path::PathBuf};

#[cfg(target_os = "macos")]
fn main() {}

#[cfg(not(target_os = "macos"))]
fn main() {
    println!("cargo:rerun-if-changed=bridge.c");
    println!("cargo:rerun-if-env-changed=CUDA_HOME");
    println!("cargo:rerun-if-env-changed=CUDA_TOOLKIT_PATH");
    println!("cargo:rerun-if-env-changed=SHRIMPLY_MOCK_CUDA");

    let mock = env::var("SHRIMPLY_MOCK_CUDA").unwrap_or_default() == "1";
    let manifest = PathBuf::from(env::var_os("CARGO_MANIFEST_DIR").unwrap());

    let include_path = if mock {
        manifest.join("../../../../external/mock_cuda/include")
    } else {
        env::var_os("CUDA_TOOLKIT_PATH")
            .or_else(|| env::var_os("CUDA_HOME"))
            .map(PathBuf::from)
            .unwrap_or_else(|| PathBuf::from("/usr/local/cuda"))
            .join("include")
    };

    let mut build = cc::Build::new();
    build.file("bridge.c").include(include_path);
    if mock {
        build.define("SHRIMPLY_MOCK_CUDA", "1");
    }
    build.compile("shrimply_cuda_bridge");

    if !mock {
        println!("cargo:rustc-link-lib=dylib=cuda");
    }
}
