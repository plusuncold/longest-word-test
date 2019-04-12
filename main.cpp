#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "timer.h"

using namespace std;

void findLongestWord(const vector<string>& text);
void findLongestWordParallel(const vector<string>& text);
void findLongestWordSimple(const vector<string>& text);

int main() {
    string path = "corpus.txt";
    vector<string> text;
    ifstream file(path.c_str());
    string line;

    // Read the file in
    while(getline(file,line)) {
	text.push_back(line);
    }
    file.close();

    // Run the striding test
    auto id = Timer::timerBegin();
    findLongestWord(text);
    auto timeStriding = Timer::timerEnd(id);

    // Run the striding test (parallel)
    id = Timer::timerBegin();
    findLongestWordParallel(text);
    auto timeStridingParallel = Timer::timerEnd(id);
    
    // Run the simple test
    id = Timer::timerBegin();
    findLongestWordSimple(text);
    auto timeSimple = Timer::timerEnd(id);

    cout << "Striding: " << timeStriding/1000 << "ms\n"
	 << "Striding parallel " << timeStridingParallel/1000 << "ms\n"
	 << "Simple: " << timeSimple/1000 << "ms" << endl;

    return 0;
}

void innerTestForLongest(const string& line, const int start, const int end,
			 string& longestWord, int& longestWordLength) {
    // Iterate through every character in the range
    for (int index = start ; index <= end ; index++) {
	// If this chunk has a space
	if (line[index] == ' ') {
	    // If left chunk could have longest word check it
	    if (index - start > longestWordLength) {
		innerTestForLongest(line, start, index - 1, longestWord, longestWordLength);
	    }

	    // If right chunk could have longest word check it
	    if (end - (index + 1) > longestWordLength) {
		innerTestForLongest(line, index+1, end, longestWord, longestWordLength);
	    }
	    return;
	}
    }

    // If reached here then the word is continuous and thus the longest word
#pragma omp critical
    {
	longestWord = line.substr(start,end - start + 1);
	longestWordLength = end - start + 1;
    }
}

void findLongestWord(const vector<string>& text) {
    string word, longestWord;
    int longestWordLength = 0;

    // For every line in the corpus
    for (const string& line : text) {
	const int lineLength = line.size();

	// Stride along line spliting into longestWordLength chunks
	int start = 0, pos = longestWordLength + 1;
	while (pos < lineLength) {
	    if (line[pos] == ' ') {
		// Check if the chunk between start and (pos - 1) contains the longest word
		innerTestForLongest(line, start, pos - 1, longestWord, longestWordLength);

		// Set start to character after the found space
		start = pos + 1;
		// Stride forward read position to first position that could contain a longest word
		pos = start + longestWordLength + 1;
	    } else {
		pos++;
	    }
	}

	// Check if end of line has a longest word
	if (lineLength - 1 - start > longestWordLength) {
	    innerTestForLongest(line, start, lineLength - 1, longestWord, longestWordLength);
	}
    }

    cout << "Longest word is '" << longestWord << "' length " << longestWordLength << endl;
}
void findLongestWordParallel(const vector<string>& text) {
    string word, longestWord;
    int longestWordLength = 0;
    long lineCount = text.size();

    // For every line in the corpus
#pragma omp parallel for shared(longestWord, longestWordLength)
    for (int l = 0 ; l < lineCount ; l++) {
	const string& line = text[l];
	const int lineLength = line.size();

	// Stride along line spliting into longestWordLength chunks
	int start = 0, pos = longestWordLength + 1;
	while (pos < lineLength) {
	    if (line[pos] == ' ') {
		// Check if the chunk between start and (pos - 1) contains the longest word
		innerTestForLongest(line, start, pos - 1, longestWord, longestWordLength);

		// Set start to character after the found space
		start = pos + 1;
		// Stride forward read position to first position that could contain a longest word
		pos = start + longestWordLength + 1;
	    } else {
		pos++;
	    }
	}

	// Check if end of line has a longest word
	if (lineLength - 1 - start > longestWordLength) {
	    innerTestForLongest(line, start, lineLength - 1, longestWord, longestWordLength);
	}
    }

    cout << "Longest word is '" << longestWord << "' length " << longestWordLength << endl;
}

void findLongestWordSimple(const vector<string>& text) {
    string word, longestWord;
    int longestWordLength = 0;

    for (const string& line : text) {
	istringstream iss(line);
	string word;
	do {
	    iss >> word;
	    if (word.length() > longestWordLength) {
		longestWord = word;
		longestWordLength = word.length();
	    }
	} while (iss);
    }
    cout << "Longest word is '" << longestWord << "' length " << longestWordLength << endl;
}
