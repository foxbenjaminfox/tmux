//! A tmux plugin for displaying a prettified version of the current working directory
//!
//! To use it, compile it into a `.so` file, and put it in your tmux plugins directory.
//! (`~/.tmux/plugins`).
//!
//! ```sh
//! $ cargo build --release
//! $ install target/release/libprettycwd.so ~/.tmux/plugins/
//! ```
//!
//! Then you can use it as the `pane_cwd_pretty` format variable in tmux.
//! For example:
//!
//! ```tmux
//! set-window-option -g window-status-current-format \
//!     '#{window_index}: #[fg=green]#{pane_current_command} #[fg=blue]#{pane_cwd_pretty}'
//! ```
pub mod display_path;

use crate::display_path::DisplayPath;
use dirs::home_dir;
use std::ffi::CString;
use std::path::PathBuf;
use tmux_plugin::format_plugin;
use users::get_current_username;

const MAX_CWD_LEN: usize = 19;

format_plugin!(b"pane_cwd_pretty\0", |ft| {
    let error_msg = CString::new("error!").unwrap();

    let pid = unsafe {
        let wp = (*ft).wp;
        if wp.is_null() {
            return error_msg;
        }
        libc::tcgetpgrp((*wp).fd)
    };

    let cwd = procinfo::pid::cwd(pid)
        .or_else(|_| {
            procinfo::pid::cwd(unsafe {
                libc::getsid(pid)
            })
        });

    match cwd {
        Ok(dir) => format_pretty_path(dir),
        Err(_) => CString::new("").unwrap(),
    }
});

pub fn format_pretty_path(cwd: PathBuf) -> CString {
    let display_path = home_dir()
        .and_then(|home| cwd.strip_prefix(&home).ok())
        .and_then(|cwd_without_home| {
            get_current_username()
                .map(|username| username.to_string_lossy().into_owned())
                .map(|username| DisplayPath::from_home_path(username, cwd_without_home))
        })
        .unwrap_or_else(|| DisplayPath::from_path_buf(cwd));

    let segments: PathBuf = display_path.display_segments();

    // segments is valid UTF8, because of the above .to_string_lossy call
    let mut result = String::from(segments.to_str().expect("Should be valid utf-8"));

    let len = result.len();
    if len > MAX_CWD_LEN {
        result = String::from("…") + &result[len - MAX_CWD_LEN + 3..];
    }

    // result has no null bytes, because neither paths nor usernames can contain null bytes
    CString::new(result).expect("Should not contain null bytes")
}
