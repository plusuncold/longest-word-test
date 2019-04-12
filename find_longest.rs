use std::env;
use std::fs;
use std::time;

trait StringUtils {
    fn substring(&self, start: usize, len: usize) -> &str;
}

impl StringUtils for str {
    fn substring(&self, start: usize, len: usize) -> &str {
        let mut char_pos = 0;
        let mut byte_start = 0;
        let mut it = self.chars();
        loop {
            if char_pos == start { break; }
            if let Some(c) = it.next() {
                char_pos += 1;
                byte_start += c.len_utf8();
            }
            else { break; }
        }
        char_pos = 0;
        let mut byte_end = byte_start;
        loop {
            if char_pos == len { break; }
            if let Some(c) = it.next() {
                char_pos += 1;
                byte_end += c.len_utf8();
            }
            else { break; }
        }
        &self[byte_start..byte_end]
    }
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

fn test_for_longest(line: &str, start: usize,
                    end: usize, mut longest_word: std::string::String,
                    longest_len: usize) -> std::string::String {
    for x in start+1..end {
        if line.chars().nth(x) == Some(' ') {
            if end - x + 1 > longest_len {
                longest_word = test_for_longest(line, start, end - 1,
                                                  longest_word, longest_len);
            }
            return longest_word
        }
    }
    line.substring(start, end-start+1).to_string()
}

fn striding_get_longest(file: std::string::String) -> std::string::String {
    let mut longest_word = "".to_string();
    let mut longest_len: usize = 0;

    for line in file.lines() {
        let line_len = line.len();
        let mut start: usize = 0;
        let mut pos = longest_len + 1;
        while pos < line_len {
            if line.chars().nth(pos) == Some(' ') {
                longest_word = test_for_longest(line, start, pos - 1,
                                                  longest_word, longest_len);
                longest_len = longest_word.len();
                start = pos + 1;
                pos = start + longest_len + 1;
            } else {
                pos = pos + 1;
            }
        }
        if line_len - 1 - start > longest_len {
            longest_word = test_for_longest(line, start, pos - 1,
                                                longest_word, longest_len);
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
    let file = fs::read_to_string(filename).expect("Unable to read file!!!");
    let start = time::Instant::now();
    let results = striding_get_longest(file);
    let elapsed = start.elapsed();
    let seconds = ((elapsed.as_secs() as f64) + (elapsed.subsec_nanos() as f64 / 1_000_000_000.0)) * 1000.0;
    
    println!("Striding:");
    println!("Longest word is '{}' with a length of {} characters.", results, results.len());
    println!("It took {} milliseconds to find this word.", seconds);
}
