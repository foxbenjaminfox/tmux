//! A tmux plugin for trimming whitespace in format strings.
//!
//! To use it, compile it into a `.so` file, and put it in your tmux plugins directory.
//! (`~/.tmux/plugins`).
//!
//! ```sh
//! $ cargo build --release
//! $ install target/release/libtrim.so ~/.tmux/plugins/
//! ```
//!
//! Then you can use it by prefixing an expression you want to trim
//! in a `tmux` format string with `f:trim:`.
//!
//! For example `#{f:trim:#{session_name}}` would give the value of the `session_name`
//! variable, with all whitespace removed from both the left and the right.
use std::ffi::CString;
use tmux_plugin::format_function_plugin;

format_function_plugin!(b"trim\0", |arg| {
    match arg.to_str() {
      Ok(string) => {
        CString::new(string.trim().to_owned())
            .expect("Does not contain null bytes, as the source was a valid CString")
      },
      Err(_) => CString::new("Invalid UTF-8 in input").unwrap(),
    }
});
