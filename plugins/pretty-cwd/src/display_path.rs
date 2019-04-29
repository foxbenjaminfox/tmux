use std::ffi::OsStr;
use std::path::{Path, PathBuf};

/// A path formated for display, either absolute or rooted in the user's home directory.
#[derive(Debug)]
pub struct DisplayPath {
    path: PathBuf,
    home: bool,
}

impl DisplayPath {
    pub fn from_home_path<T, U>(username: T, rel_path: U) -> DisplayPath
    where
        T: AsRef<str>,
        U: AsRef<Path>,
    {
        DisplayPath {
            path: PathBuf::from(String::from("~") + username.as_ref()).join(rel_path),
            home: true,
        }
    }

    pub fn from_path_buf(path: PathBuf) -> DisplayPath {
        DisplayPath { path, home: false }
    }

    pub fn display_segments(&self) -> PathBuf {
        let name = self.path.file_name().unwrap_or_else(|| OsStr::new(""));
        let parent = self.path.parent().unwrap_or_else(|| Path::new(""));
        let parent_name = parent
            .file_name()
            .unwrap_or_else(|| OsStr::new(if self.home { "" } else { "/" }));
        PathBuf::from(parent_name).join(name)
    }
}
