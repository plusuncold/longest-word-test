mod single;
mod multi;
//mod msimpletest;
use std::env;

fn main() {
    if env::args().nth(2).unwrap_or("release".to_string()) == "--test".to_string() {
        println!("Running tests");
        //msimpletest::simple();
    } else{
        println!("Single threaded solutions:");
        println!("--------------------------");
        single::single();
        println!("");
        println!("Multi threaded solutions");
        println!("--------------------------");
        multi::multi();
        println!("");
    }
}
