use std::env;
use std::fs;
use std::time;

fn test_for_longest(line: &str, start: usize, 
                    end: usize, mut longest: String, 
                    longest_len: usize) -> String {
    for x in start+1..end {
        if line.chars().nth(x) == Some(' ') {
            //check to the left
            if x - start > longest_len {
                longest = test_for_longest(line, start, end - 1,
                                            longest, longest_len);
            }
            //check to the right
            if end - x + 1 > longest_len {
                longest = test_for_longest(line, start, end - 1,
                                            longest, longest_len);
            }
            return longest
        }
    }
    line.chars().skip(start).take(end - start + 1).collect()
}

fn striding_longest(file: String) -> String {
    let mut longest_len: usize = 0;
    let mut longest = String::new();
    for words in file.lines() {
        let line_len = words.len();
        if line_len > longest_len {
            let mut start: usize = 0;
            let mut pos = longest_len + 1;
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
            if line_len - 1 - start > longest_len {
                longest = test_for_longest(&words, start, pos - 1,
                                            longest, longest_len);
            }
        }
    }
    longest
}

fn main() {
    let filename = env::args().nth(1).unwrap();
    let file: String = fs::read_to_string(filename).expect("Bummer, man!");

    let start = time::Instant::now();
    let result = striding_longest(file);
    let elapsed = start.elapsed();
    let seconds = ((elapsed.as_secs() as f64) + (elapsed.subsec_nanos() as f64 / 1_000_000_000.0)) * 1000.0;
    println!("Striding:");
    println!("Longest word is '{}' with a length of {} characters.", result, result.len());
    println!("It took {} milliseconds to find this word.", seconds);
}
