use std::env;
use std::io::BufReader;
use std::io::prelude::*;
use std::fs::{self, File};
use std::time;

fn test_for_longest(line: &str, start: usize, 
                    end: usize, mut longest: String, 
                    longest_len: usize) -> String {
    //iterate through line, char by char
    //x is the index of char
    for x in start+1..end {
        //check if xth char is a space
        //this is functionally the same as `string[x]` in other languages
        //this is safer though because Rust  strings are utf8 and can 
        //be one or two chars in length
        if line.chars().nth(x) == Some(' ') {
            //if x is a space and the len of the substring is greater
            //than the prior longest, check if substring is longester
            if end - x + 1 > longest_len {
                longest = test_for_longest(line, start, end - 1,
                                            longest, longest_len);
            }
            //If x is a space, return `longest`
            return longest
        }
    }
    //return the longest
    //due to Rust's treatment of all strings as UTF8
    //This will `skip()` to the first char in the substring
    //we `take()` all char from `start` to the end of the word
    //then `collect()` it back into `std::string::String`
    //We use `collect()` because `line` is a `&str` and we want
    //to return a `String` so it needs to be transformed via iteration
    line.chars().skip(start).take(end - start + 1).collect()
}

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
    let words = file.split_whitespace();

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

    //See lines 71 through 81 above for why I am re-reallocating `filename`
    let filename = env::args().nth(1).unwrap();
    //This time, instead of reading the file to a string we will stream it
    let f = File::open(filename).unwrap();
    //BufReader is one of three ways of streaming text, it's decidedly the fastest
    let cursor = BufReader::new(f);

    let start = time::Instant::now();
    let mut longest_len: usize = 0;
    let mut longest = String::new();

    //iterate through each line of the cursor
    for line in cursor.lines() {
        //`lines` is "wrapped" in a `<Result>`, need to call `unwrap()` to get to string within
        let words = line.unwrap();
        let line_len = words.len();
        let mut start: usize = 0;
        let mut pos = longest_len + 1;
        //some lines are shorter than the longest word, no need to even look at them
        if line_len > longest_len {
            while pos < line_len {
                //if the words[x] char is a space...
                if words.chars().nth(pos) == Some(' ') {
                    longest = test_for_longest(&words, start, pos - 1,
                                               longest, longest_len);
                    longest_len = longest.len();
                    start = pos + 1;
                    pos = start + longest_len + 1;
                } else {
                    pos = pos + 1;
                }
            }
            if line_len -1 - start > longest_len {
                longest = test_for_longest(&words, start, pos - 1,
                                            longest, longest_len);
            }
        }
    }
    let elapsed = start.elapsed();
    let seconds = ((elapsed.as_secs() as f64) + (elapsed.subsec_nanos() as f64 / 1_000_000_000.0)) * 1000.0;
    println!("Striding:");
    println!("Longest word is '{}' with a length of {} characters.", longest, longest_len);
    println!("It took {} milliseconds to find this word.", seconds);
}
