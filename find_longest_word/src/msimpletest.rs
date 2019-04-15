use std::env;
use std::fs;
use std::time;
extern crate rayon;
use rayon::prelude::*;

fn get_longest(words: rayon::str::SplitWhitespace) -> std::string::String {
    //allocate two mutable varibles to store the longest word and its length
    let mut longest_word = String::new();
    let mut longest_len: usize = 0;

    //iterate through the `str::SplitWhitespace` word by word
    for word in words {
        //if the word is longer than the prior longest word, set it as the longest
        if word.len() > longest_len {
            longest_len = word.len();
            longest_word = word.to_string();
        }
    }
    //return the longest word found
    longest_word
}

pub fn simple() {
    //get $1 from sys
    let filename = env::args().nth(1).unwrap();
    //read argv as a string
    let file: std::string::String = fs::read_to_string(filename).expect("");
    //use the string method `split_whitespace()` to split each word in string into an iterator
    let words = file.par_split_whitespace();

    let start = time::Instant::now();
    let results = get_longest(words);
    let elapsed = start.elapsed();
    let seconds = ((elapsed.as_secs() as f64) + (elapsed.subsec_nanos() as f64 / 1_000_000_000.0)) * 1000.0;

    println!("Simple:");
    println!("Longest word is '{}' with a length of {} characters.", results, results.len());
    println!("It took {} milliseconds to find this word.", seconds);
}
