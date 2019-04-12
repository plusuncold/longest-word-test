use std::env;
use std::io::BufReader;
use std::io::prelude::*;
use std::fs::{self, File};
use std::time;

fn test_for_longest(line: &str, start: usize, 
                    end: usize, mut longest: String, 
                    longest_len: usize) -> String {
    for x in start+1..end {
        if line.chars().nth(x) == Some(' ') {
            if end - x + 1 > longest_len {
                longest = test_for_longest(line, start, end - 1,
                                            longest, longest_len);
            }
            return longest
        }
    }
    line.chars().skip(start).take(end - start + 1).collect()
}

fn get_longest(words: std::str::SplitWhitespace) -> std::string::String {
    let mut longest_word = String::new();
    let mut longest_len: usize = 0;

    for word in words {
        if word.len() > longest_len {
            longest_len = word.len();
            longest_word = word.to_string();
        }
    }
    longest_word
}

fn main() {
    let filename = env::args().nth(1).unwrap();
    let file = fs::read_to_string(filename).expect("Unable to read file!!!");
    let words = file.split_whitespace();

    let start = time::Instant::now();
    let results = get_longest(words);
    let elapsed = start.elapsed();
    let seconds = ((elapsed.as_secs() as f64) + (elapsed.subsec_nanos() as f64 / 1_000_000_000.0)) * 1000.0;
    
    println!("Simple:");
    println!("Longest word is '{}' with a length of {} characters.", results, results.len());
    println!("It took {} milliseconds to find this word.", seconds);
    
    let filename = env::args().nth(1).unwrap();
    let file: std::string::String = fs::read_to_string(filename).expect("Unable to read file!!!");

    let start = time::Instant::now();

    let mut words: Vec<_> = file.split_whitespace().collect();
    words.sort_by(|a, b| b.len().cmp(&a.len()));

    let elapsed = start.elapsed();
    let seconds = ((elapsed.as_secs() as f64) + (elapsed.subsec_nanos() as f64 / 1_000_000_000.0)) * 1000.0;

    println!("'Pythonic':");
    println!("Longest word is '{}' with a length of {} characters.", words[0], words[0].len());
    println!("It took {} milliseconds to find this word.", seconds);

    let filename = env::args().nth(1).unwrap();
    let f = File::open(filename).unwrap();
    let cursor = BufReader::new(f);

    let start = time::Instant::now();
    let mut longest_len: usize = 0;
    let mut longest = String::new();

    for line in cursor.lines() {
        let words = line.unwrap();
        let line_len = words.len();
        let mut start: usize = 0;
        let mut pos = longest_len + 1;
        if line_len > longest_len {
            while pos < line_len {
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
