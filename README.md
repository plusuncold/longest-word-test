# Longest Word Test

## The Rules

1. Solution may be in any language and use any implementation or algorithm
1. Solution must find the longest word in a corpus. Words are only considered to be space and newline delimited text.
1. Solution is timed from first loading/reading of the corpus to the return of the longest word and/or length of the word.
  - Parsing arguments is not counted
  - Runtime initialization is not counted
  - Time should be measured in MS
1. Solutions are currently divided into two categories: single-threaded and concurrent.

## How to participate

Write your solution, add your name to the top, and send a pull request. It would be super awesome if you also updated the Dockerfile and the `run_tests.sh` to include your submission, but that's not necessary.

## Leaderboard

### Single-Threaded

| Name             | Language   | Time (ms) | Notes                   |
|------------------|------------|-----------|-------------------------|
| @dnesbitt03qub   | C++        | 104       | Striding implementation | 
| @pard68          | Rust       | 311       |                         |
| Jeffrey Corcoran | C#         | 810       |                         |
| @bbriggs         | Go         | 997       |                         |
| @drewpearce      | Cython     | 1137      | Loop w/ longest         |
| @dnesbitt03qub   | C++        | 1159      |                         |
| @zombeej         | JavaScript | 1292      |                         |
| @pard68          | Bash       | 1892      |                         |
| @specs           | Python     | 2250      | Loop w/ longest         |
| @specs           | Python     | 2656      | Comprehensions          |
| @specs           | Python     | 3091      | Loop and sort           |


### Concurrent
| Name             | Language   | Time (ms) | Notes                   |
|------------------|------------|-----------|-------------------------|
| Jeffrey Corcoran | C#         | 266       |                         | 


### Methodology
