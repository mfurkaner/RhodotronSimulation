use std::fs::{File, read_to_string, self};
use std::io::Write;

use byteorder::WriteBytesExt; // This trait adds methods to writeable types
use byteorder::LittleEndian;

fn main() {
    let filename_arg : String = String::from("-f");
    let concentrate_arg : String = String::from("-c");

    let help_message = format!("
--------------------------------------------------------------
                  Field Reducer V_0.1

E field export file reducer for Rhodotron Simulation Software.
31/08/2023                                        M.Furkan Er

Only use this software with CST Studio E field export files.

{filename_arg}  :  file path input flag ({filename_arg} filepath)
{concentrate_arg}  :  concentration flag, saves the reduced data as f32 LE bit dump
--------------------------------------------------------------\n");

    let mut filename_flag : bool = false;
    let mut concentrate: bool = false;    

    let mut filename : String = String::new();

    for argument in std::env::args() {
        if filename_flag{
            filename_flag = false;
            filename = argument;
        }
        

        else if argument.eq_ignore_ascii_case(&filename_arg){
            filename_flag = true;
        }
        else if argument.eq_ignore_ascii_case(&concentrate_arg){
            concentrate = true;
        }
    }

    match fs::metadata(&filename) {
        Ok(_) => remove_unnecessary_data(&filename, 50, concentrate),
        Err(_) => println!("{}",help_message)
    }

}


fn remove_unnecessary_data(file_name : &str, step_count: usize, concentrate : bool) {

    println!("-> Reading from {}...", file_name);

    let lines : Vec<String> = read_to_string(file_name) 
        .unwrap()  // panic on possible file-reading errors
        .lines()  // split the string into an iterator of string slices
        .map(String::from)  // make each slice into a string
        .collect();

    let mut new_lines = String::new();
    new_lines += "# x | y | z | Ex | Ey | Ez\n";
    let size = lines.len();
    let step = size/step_count;

    let mut f32_vec : Vec<f32> = Vec::new();

    println!("-> Starting cleanup...");

    print!("v");
    for _i in 0..step_count{
        print!("_");
    }
    print!("v\n[");

    let mut zero_deleted : i32 = 0;

    for i in 0..size {

        if lines[i].starts_with("#"){
            continue;
        }
        let mut iter = lines[i].split_whitespace();
        
        if iter.clone().count() == 9{
            let x = iter.next().unwrap().parse::<f32>().unwrap();
            let y = iter.next().unwrap().parse::<f32>().unwrap();
            let z = iter.next().unwrap().parse::<f32>().unwrap();
            
            let ex_r = iter.next().unwrap().parse::<f32>().unwrap();
            let _ex_i = iter.next().unwrap().parse::<f32>().unwrap();
            let ey_r = iter.next().unwrap().parse::<f32>().unwrap();
            let _ey_i = iter.next().unwrap().parse::<f32>().unwrap();
            let ez_r = iter.next().unwrap().parse::<f32>().unwrap();
            let _ez_i = iter.next().unwrap().parse::<f32>().unwrap();

            if (ex_r != 0.0) || (ey_r != 0.0) || (ez_r != 0.0){
                if concentrate {
                    f32_vec.push(x);
                    f32_vec.push(y);
                    f32_vec.push(z);

                    f32_vec.push(ex_r);
                    f32_vec.push(ey_r);
                    f32_vec.push(ez_r);
                }
                else{
                    let new_line = format!("{} {} {} {} {} {}\n", x, y, z , ex_r, ey_r, ez_r);
                    new_lines = new_lines + &new_line;
                }
            }
            else{
                zero_deleted += 1;
            }
        }

        if step != 0 && i%step == step_count-1 {
            print!("#");
            std::io::stdout().flush().unwrap();
        }
        else if step == 0 {
            print!("#");
        }
    }

    if step == 0 {
        for _i in 0..(step_count - size){
            print!("#");
        }
    }
    println!("]");

    println!("-> Cleanup complete.\n-> {} zero E-field lines deleted, imaginary columns removed.", zero_deleted);

    let out_file_name = file_name.to_string() + if concentrate {"_clean_conc.dat"} else {"_clean.dat"};

    let mut file = File::create(out_file_name.clone()).unwrap();

    if concentrate {
        //let mut count = 0;
        for f in f32_vec {
            /*
            if count % 6 == 0{
                file.write(b"\n").unwrap();
            }*/
            file.write_f32::<LittleEndian>(f).unwrap();
            //count += 1;
        }
    }
    else {
        file.write_all(new_lines.as_bytes()).unwrap();
    }

    println!("-> Clean data written to {}", out_file_name);

    }