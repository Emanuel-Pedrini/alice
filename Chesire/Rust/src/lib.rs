use std::ffi::CStr;
use std::io::ErrorKind;
use std::fs::{self, File};
use std::ops::Add;
use std::os::raw::{c_char};
use heck::{AsSnakeCase, ToSnakeCase};
use crate::FlSpace::Folder;

pub enum FlSpace {
    Folder(FlFolder),
    File(FlFile)
}
pub struct FlFile {
    name : String,
    content : String
}

impl FlFile {
    pub fn new(name : &str, content : &str) -> Self {
        Self {
            name : name.to_string(),
            content : content.to_string()
        }
    }
}
pub struct FlFolder {
    name : String,
    content : Vec<FlSpace>
}

impl FlFolder {
    pub fn new(name : &str, content : Vec<FlSpace>) -> Self {
        Self {
            name : name.to_string(),
            content : content
        }
    }
    pub fn create(&self, root : &str) {
        Rust_CreateFolder(&format!("{}{}", root, self.name));
        for space in self.content.iter() {
           match space {
               FlSpace::Folder(_folder) => {_folder.create(&format!("{}{}", root, &self.name));},
               FlSpace::File(_file) => {Rust_CreateFile(&format!("{}{}{}", root, &self.name, _file.name), &_file.content)}
           } 
        }
    }
}
pub struct FlProjectSpace {
    root : String,
    content : Vec<FlSpace>
}

impl FlProjectSpace {
    pub fn new(root : &str) -> Self {
        Self {
            root : root.to_string(),
            content : Vec::new()
        }
    }
    pub fn create(&self) {
        for space in self.content.iter() {
           match space {
               FlSpace::Folder(_folder) => {_folder.create(&self.root);},
               FlSpace::File(_file) => {Rust_CreateFile(&format!("{}{}", self.root, &_file.name), &_file.content);}
           } 
        }
    }
    pub fn add(&mut self, space : FlSpace) {
        self.content.push(space);
    }
}

pub fn ptr_to_str(ptr : *const c_char) -> String {
    let cstring : &CStr = unsafe { CStr::from_ptr(ptr) };
    let string : &str = match cstring.to_str() {
        Ok(s) => s,
        Err(_) => return String::new()
    };
    return string.to_string();
}

pub fn Rust_CreateFile(name : &str, content : &str) {
    match File::create_new(name) {
        Ok(_file) => {},
        Err(_error) => {}
    }
    fs::write(name, content).unwrap();
}

pub fn Rust_CreateFolder(name : &str) {
    fs::create_dir_all(name).unwrap();
}

#[unsafe(no_mangle)]
pub extern "C" fn Rust_CreateChesire(name : *const c_char) 
{
    let mut root = ptr_to_str(name);
    if !root.is_empty() {
        root = root.add("/");
        Rust_CreateFolder(&root);
    }

    let used_name = match ptr_to_str(name).as_str() {
        "" => { &String::from("") },
        _ => { &ptr_to_str(name) }
    };

    let initial_content = &format!("@initialize\nfunction {}(argc : int.32, argv : char.8**) -> int.32:\n    println! << \"Hello, world!\" \n    return 0", used_name.to_snake_case());

    let main =  FlFile::new("main.al", initial_content);
    let proj =  FlFile::new("proj.toml", "");
    let gitignore =  FlFile::new(".gitignore", "build/");

    let src = FlFolder::new("src/", vec![FlSpace::File(main)]);

    let build = FlFolder::new("build/", vec![]);

    let mut project = FlProjectSpace::new(&root);
    project.add(FlSpace::Folder(src));
    project.add(FlSpace::Folder(build));
    project.add(FlSpace::File(proj));
    project.add(FlSpace::File(gitignore));
    project.create();
}
