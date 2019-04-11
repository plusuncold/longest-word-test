use std::env;
use std::fs;
use std::time;

fn get_longest(words: std::str::SplitWhitespace) -> (std::string::String, usize) {
    let mut longest_word = String::new();
    let mut longest_len: usize = 0;

    for word in words {
        if word.len() > longest_len {
            longest_len = word.len();
            longest_word = word.to_string();
        }
    }
    (longest_word, longest_len)
}

fn main() {
    let filename = env::args().nth(1).unwrap();
    let file = fs::read_to_string(filename).expect("Unable to read file!!!");
    let words = file.split_whitespace();

    let start = time::Instant::now();
    let results = get_longest(words);
    let elapsed = start.elapsed();
    let seconds = ((elapsed.as_secs() as f64) + (elapsed.subsec_nanos() as f64 / 1_000_000_000.0)) * 1000.0;
    
    println!("Longest word is '{}' with a length of {} characters.", results.0, results.1);
    println!("It took {} milliseconds to find this word.", seconds);
}
