use std::fs::{File, read_to_string, self};
use std::io::Write;

use byteorder::WriteBytesExt; // This trait adds methods to writeable types
use byteorder::LittleEndian;

fn main() {
    let filename_arg : String = String::from("-f");
    let concentrate_arg : String = String::from("-c");
    let normalization_arg : String = String::from("-n");
    let delete_zero_arg : String = String::from("-z");

    let help_message = format!("
--------------------------------------------------------------
                  Field Reducer V_0.1

E field export file reducer for Rhodotron Simulation Software.
31/08/2023                                        M.Furkan Er

Only use this software with CST Studio E field export files.

{filename_arg}  :  file path input flag ({filename_arg} filepath)
{normalization_arg}  :  normalization flag, scales the field (-n <f32>(V/m))
{concentrate_arg}  :  concentration flag, saves the reduced data as f32 LE bit dump
{delete_zero_arg}  :  delete zero flag, deletes all zero field lines
--------------------------------------------------------------\n");

    let mut filename_flag : bool = false;
    let mut normalization_flag : bool = false;

    let mut concentrate: bool = false;    
    let mut delete_zero: bool = false;    
    let mut filename : String = String::new();
    let mut max_value : f32 = 0.0;

    for argument in std::env::args() {
        if filename_flag{
            filename_flag = false;
            filename = argument;
        }
        else if normalization_flag{
            normalization_flag = false;
            max_value = argument.parse::<f32>().unwrap_or(0.0);
        }
        

        else if argument.eq_ignore_ascii_case(&filename_arg){
            filename_flag = true;
        }
        else if argument.eq_ignore_ascii_case(&normalization_arg){
            normalization_flag = true;
        }
        else if argument.eq_ignore_ascii_case(&concentrate_arg){
            concentrate = true;
        }
        else if argument.eq_ignore_ascii_case(&delete_zero_arg){
            delete_zero = true;
        }
    }

    match fs::metadata(&filename) {
        Ok(_) => remove_unnecessary_data(&filename, 50, concentrate, max_value, delete_zero),
        Err(_) => println!("{}",help_message)
    }

}


fn remove_unnecessary_data(file_name : &str, step_count: usize, concentrate : bool, max_value : f32, delete_zeros : bool) {
    let mut data_max_value : f32 = 0.0;
    println!("-> Reading from {}...", file_name);

    let lines : Vec<String> = read_to_string(file_name) 
        .unwrap()  // panic on possible file-reading errors
        .lines()  // split the string into an iterator of string slices
        .map(String::from)  // make each slice into a string
        .collect();

    let size = lines.len();
    let step = size/step_count;

    let mut f32_vec : Vec<f32> = Vec::new();

    println!("-> Starting cleanup...\n");

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

            if delete_zeros == false || ( (ex_r != 0.0) || (ey_r != 0.0) || (ez_r != 0.0) ){
                if z == 0.0 && (ex_r*ex_r + ey_r*ey_r + ez_r*ez_r) > data_max_value*data_max_value {
                    data_max_value = (ex_r*ex_r + ey_r*ey_r + ez_r*ez_r).sqrt();
                }
                f32_vec.push(x);
                f32_vec.push(y);
                f32_vec.push(z);

                f32_vec.push(ex_r);
                f32_vec.push(ey_r);
                f32_vec.push(ez_r);
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
    println!("]\n");

    println!("-> Cleanup complete.");
    if delete_zeros{
        println!("-> {} zero E-field lines deleted.", zero_deleted);
    }

    let out_file_name = file_name.to_string() + if concentrate {"_clean_conc.dat"} else {"_clean.dat"};
    let mut file = File::create(out_file_name.clone()).unwrap();

    let normalization_const = if max_value == 0.0 {1.0} else {max_value/data_max_value};

    let size = f32_vec.len();
    let step = size/step_count;

    let mut count = 0;
    while count < f32_vec.len(){
        if count % 6 >= 3 {
            f32_vec[count] *= normalization_const;
        }
        else{
            // mm to m
            f32_vec[count] *= 0.001;
        }
        count += 1;
    }

    if normalization_const != 1.0{
        println!("-> Max |E| on data : {data_max_value}, normalized to : {max_value}, by mult_const : {normalization_const}");
    }

    println!("-> Writing to {out_file_name}.\n");
    print!("v");
    for _i in 0..step_count{
        print!("_");
    }
    print!("v\n[");

    if concentrate {
        count = 0;
        for f in f32_vec {
            file.write_f32::<LittleEndian>(f).unwrap();
            if step != 0 && count%step == step_count-1 {
                print!("#");
                std::io::stdout().flush().unwrap();
            }
            else if step == 0 {
                print!("#");
            }
            count+=1;
        }
    }
    else {
        file.write( "# x | y | z | Ex | Ey | Ez\n".as_bytes()).unwrap();
        for (i, f) in f32_vec.into_iter().enumerate() {
            file.write(format!("{f}{}", if (i%6) == 5 {"\n"} else {" "}).as_bytes()).unwrap();

            if step != 0 && i%step == step_count-1 {
                print!("#");
                std::io::stdout().flush().unwrap();
            }
            else if step == 0 {
                print!("#");
            }
        }
    }

    if step == 0 {
        for _i in 0..(step_count - size){
            print!("#");
        }
    }
    println!("]\n");

    println!("-> Clean data written to {}", out_file_name);
    let old : f64 = (fs::metadata(file_name).unwrap().len() as f64) / 1000000.0;
    let new: f64 = (fs::metadata(out_file_name).unwrap().len() as f64) / 1000000.0;
    println!("-> File size reduced from {:.2} Mb to {:.2} Mb", old, new);

    }