use std::fs::File;
use std::io::prelude::*;
use std::env;
use std::fs;

fn main() -> std::io::Result<()> {
    let mut arguments = env::args();
    for str in &mut arguments
    {
        println!("fuck {}", str);

        if (str == "create") {

        fs::create_dir("alice")?;

            
        }

        if (str == "initialize") {


 let mut file = File::create("alice.al")?;
    file.write_all(b"fn main() i32:
    println(\"Hello, world!\")
    end")?;

        }
    }
    return Ok(());
}