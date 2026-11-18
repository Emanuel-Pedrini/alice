#![allow(non_snake_case)]
use std::fs::File;
use std::io::prelude::*;
use std::env;
use std::env::{Args};
use std::fs;

struct ArgumentReader //SHE/HER
{
    Pointer : usize,
    Current : String,
    Arguments : Vec<String>
}

impl ArgumentReader 
{
    fn New() -> Self
    {
        let EnviromentArguments : Vec<String> = env::args().collect();
        let InitialPointer = 0;
        Self 
        {
            Pointer : InitialPointer,
            Current : EnviromentArguments.get(InitialPointer).cloned().unwrap_or_default(),
            Arguments : EnviromentArguments
        }
    }

    pub fn Set(&mut self, NewValue : usize)
    {
        self.Pointer = NewValue;
    }
    
    pub fn Next(&mut self, Offset : usize)
    {
        self.Set(self.Pointer + Offset);
    }

    pub fn GetByIndex(&self, Index : usize) -> Option<&str>
    {
        return self.Arguments.get(Index).map(String::as_str);
    }

    pub fn Peek(&mut self, Offset : usize) -> Option<&str>
    {
        return self.GetByIndex(self.Pointer + Offset);
    }

    pub fn Get(&mut self) -> Option<&str>
    {
        return self.GetByIndex(self.Pointer);
    }
}


fn main() -> std::io::Result<()> 
{
    let mut Reader = ArgumentReader::New();
    let Size = Reader.Arguments.len();
    let mut Interator = 0;
    while Interator < Size
    {
        println!("{}", Reader.GetByIndex(Interator).unwrap());
        Interator += 1;
    }
    return Ok(());
    
}
