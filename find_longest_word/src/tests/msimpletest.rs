use std::env;
use std::fs;
use std::time;
extern crate rayon;
use rayon::prelude::*;

fn get_longest(words: std::str::SplitWhitespace) -> std::string::String {
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

fn main() {
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

    //Tried to find an elegant way to get around the below but due to ownership
    //and `std::str::SplitWhitespace` not being well documented to allocate an
    //empty variable of that type, I settled for just reusing the prior `file` and
    //`filename` variables and reassigning them to the same thing...
    //There are two issues. First, `filename` is owned by `fs::read_to_string`
    //and I can't take ownership of `filename` here. Second `file` is consumed by
    //`split_whitespace()` and I wasn't able to `clone()` it because `words` lifetime ends
    //before `files`, which is why I went hunting for the means of allocating an empty
    //`std::str::SplitWhitespace`.
    //Long live the jankiness!
    let filename = env::args().nth(1).unwrap();
    let file: std::string::String = fs::read_to_string(filename).expect("");

    let start = time::Instant::now();

    //My pythonic-esque solution
    //Create a Vec of the words, then sort words by length.
    //The answer will be the 0th word in the Vec.
    //May be able to use `filter()` or another method to 
    //increase speed but this was just a fun solution so I didn't bother
    let mut words: Vec<_> = file.split_whitespace().collect();
    words.sort_by(|a, b| b.len().cmp(&a.len()));

    let elapsed = start.elapsed();
    let seconds = ((elapsed.as_secs() as f64) + (elapsed.subsec_nanos() as f64 / 1_000_000_000.0)) * 1000.0;

    println!("'Pythonic':");
    println!("Longest word is '{}' with a length of {} characters.", words[0], words[0].len());
    println!("It took {} milliseconds to find this word.", seconds);
}
