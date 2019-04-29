//! A tmux plugin that demonstrates how to write a notification plugin in Rust, by
//! appending a smiley emoticon to the name of any window you rename.
//!
//! To use it, compile it into a `.so` file, and put it in your tmux plugins directory.
//! (`~/.tmux/plugins`).
//!
//! ```sh
//! $ cargo build --release
//! $ install target/release/libsmileywindownames.so ~/.tmux/plugins/
//! ```
use tmux_plugin::notification_plugin;

use std::ffi::{CStr, CString};

notification_plugin!(b"window-renamed\0", |notify_entry| {
    let window = unsafe { (*notify_entry).window };
    let window_name = unsafe { CStr::from_ptr((*window).name) };
    let mut name = window_name
        .to_string_lossy()
        .into_owned();
    if !name.ends_with(":-)") {
        name.push_str(" :-)");
        let c_string = CString::new(name)
            .expect("Does not contain null bytes, as it was based off of a valid C str");
        unsafe {
            // Free the old name, and duplicate the new name so
            // that tmux can later free it safely.
            libc::free((*window).name as *mut _);
            (*window).name = libc::strdup(c_string.as_ptr())
        }
    }
});
