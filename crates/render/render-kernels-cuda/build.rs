#[cfg(target_os = "linux")]
use std::{env, fs, path::PathBuf};

#[cfg(not(target_os = "linux"))]
fn main() {}

#[cfg(target_os = "linux")]
fn main() {
    let out = PathBuf::from(env::var_os("OUT_DIR").expect("CUDA build output"));
    let mut bindings = String::new();
    let modules = include_str!("../render-core/shaders/kernels.txt");
    for module in modules.lines() {
        bindings.push_str(&format!(
            "pub const {}: &[u8] = &[];\n",
            module.to_uppercase()
        ));
    }
    bindings.push_str("pub const IMAGE_FORMAT: &str = \"cubin\";\n");
    bindings.push_str("pub const IMAGE_TARGET: &str = \"sm_86\";\n");
    fs::write(out.join("kernels.rs"), bindings).expect("write CUDA kernel bindings");
}
