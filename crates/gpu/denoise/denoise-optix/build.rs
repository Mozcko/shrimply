use std::{env, path::PathBuf};

fn main() {
    println!("cargo:rerun-if-changed=src/bridge.c");
    println!("cargo:rerun-if-env-changed=CUDA_HOME");
    println!("cargo:rerun-if-env-changed=CUDA_TOOLKIT_PATH");
    println!("cargo:rerun-if-env-changed=OPTIX_ROOT");
    println!("cargo:rerun-if-env-changed=SHRIMPLY_MOCK_CUDA");

    let mock = env::var("SHRIMPLY_MOCK_CUDA").unwrap_or_default() == "1";
    let manifest = PathBuf::from(env::var_os("CARGO_MANIFEST_DIR").unwrap());

    let mut build = cc::Build::new();
    build.file("src/bridge.c");

    if mock {
        build.define("SHRIMPLY_MOCK_CUDA", "1");
        build.include(manifest.join("../../../../external/mock_cuda/include"));
    } else {
        let cuda = env::var_os("CUDA_TOOLKIT_PATH")
            .or_else(|| env::var_os("CUDA_HOME"))
            .map(PathBuf::from)
            .unwrap_or_else(|| PathBuf::from("/usr/local/cuda"));
        build.include(cuda.join("include"));
        if let Some(optix) = env::var_os("OPTIX_ROOT") {
            build.include(PathBuf::from(optix).join("include"));
        }
    }

    build.compile("shrimply_optix_bridge");
}
