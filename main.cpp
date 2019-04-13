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
void findLongestWordSimpleOptimized(const vector<string>& text);
void findLongestWordSimpleOptimizedParallel(const vector<string>& text);

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
    //findLongestWordSimple(text);
    auto timeSimple = Timer::timerEnd(id);

    // Run the simple test optimized
    id = Timer::timerBegin();
    //findLongestWordSimpleOptimized(text);
    auto timeSimpleOpt = Timer::timerEnd(id);

    // Run the simple test optimized (parallel)
    id = Timer::timerBegin();
    //findLongestWordSimpleOptimizedParallel(text);
    auto timeSimpleOptParallel = Timer::timerEnd(id);

    cout << "Striding: " << timeStriding/1000 << "ms\n"
	 << "Striding parallel " << timeStridingParallel/1000 << "ms\n"
       //<< "Simple: " << timeSimple/1000 << "ms\n"
       //<< "Simple opt " << timeSimpleOpt/1000 << "ms\n"
       //<< "Simple opt parallel " << timeSimpleOptParallel/1000 << "ms\n"
	 << endl;

    return 0;
}

struct lw {
    int start;
    int lineNumber;
    int length;
};

void innerTestForLongest(const string& line, const int start, const int end, const int posStart, const int lineNumber, lw& longestWord) {

    // Iterate through every character in the range
    for (int index = start ; index < posStart ; index++) {
	// If this chunk has a space
	if (line[index] == ' ') {
	    // If left chunk could have longest word check it
	    if (index - start > longestWord.length) {
		innerTestForLongest(line, start, index - 1, index - 1, lineNumber, longestWord);
	    }

	    // If right chunk could have longest word check it
	    if (end - (index + 1) > longestWord.length) {
		innerTestForLongest(line, index+1, end, posStart, lineNumber, longestWord);
	    }
	    return;
	}
    }

    // If reached here then the word is continuous and thus the longest word
#pragma omp critical
    {
	if (end - start + 1 > longestWord.length) { // Test incase the longest word has changed
	    longestWord.start = start;
	    longestWord.length = end - start + 1;
	    longestWord.lineNumber = lineNumber;
	}
    }
}

void findLongestWord(const vector<string>& text) {
    string word;
    lw longestWord;
    longestWord.length = 0;
    int lineCount = text.size();

    // For every line in the corpus
    for (int l = 0 ; l < lineCount ; l++) {
	const string& line = text[l];
	const int lineLength = line.size();

	// Stride along line spliting into longestWordLength chunks
	int start = 0, pos = longestWord.length + 1;
	int posStart = (pos < lineLength) ? pos : lineLength - 1;
	while (pos < lineLength) {
	    if (line[pos] == ' ') {
		// Check if the chunk between start and (pos - 1) contains the longest word
		innerTestForLongest(line, start, pos - 1, posStart, l, longestWord);

		// Set start to character after the found space
		start = pos + 1;
		// Stride forward read position to first position that could contain a longest word
		pos = start + longestWord.length + 1;
		posStart = (pos < lineLength) ? pos : lineLength - 1;
	    } else {
		pos++;
	    }
	}

	// Check if end of line has a longest word
	if (lineLength - 1 - start > longestWord.length) {
	    innerTestForLongest(line, start, lineLength - 1, posStart, l, longestWord);
	}
    }

    const string& longestWordLine = text[longestWord.lineNumber];
    cout << "Longest word is '";
    for (int pos = longestWord.start ; pos < longestWord.start + longestWord.length ; pos++)
	cout << longestWordLine[pos];
    cout << "' length " << longestWord.length << "\n";
}
void findLongestWordParallel(const vector<string>& text) {
    string word;
    lw longestWord;
    longestWord.length = 0;
    long lineCount = text.size();

    // For every line in the corpus
#pragma omp parallel for shared(longestWord)
    for (int l = 0 ; l < lineCount ; l++) {
	const string& line = text[l];
	const int lineLength = line.size();

	// Stride along line spliting into longestWordLength chunks
	int start = 0, pos = longestWord.length + 1;
	int posStart = (pos < lineLength) ? pos : lineLength - 1;
	while (pos < lineLength) {
	    if (line[pos] == ' ') {
		// Check if the chunk between start and (pos - 1) contains the longest word
		innerTestForLongest(line, start, pos - 1, posStart, l, longestWord);

		// Set start to character after the found space
		start = pos + 1;
		// Stride forward read position to first position that could contain a longest word
		pos = start + longestWord.length + 1;
		posStart = (pos < lineLength) ? pos : lineLength - 1;
	    } else {
		pos++;
	    }
	}

	// Check if end of line has a longest word
	if (lineLength - 1 - start > longestWord.length) {
	    innerTestForLongest(line, start, lineLength - 1, posStart, l, longestWord);
	}
    }

    const string& longestWordLine = text[longestWord.lineNumber];
    cout << "Longest word is '";
    for (int pos = longestWord.start ; pos < longestWord.start + longestWord.length ; pos++)
	cout << longestWordLine[pos];
    cout << "' length " << longestWord.length << "\n";
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

void findLongestWordSimpleOptimized(const vector<string>& text) {
    string longestWord;
    int longestWordLength = 0;
    long lineCount = text.size();

    // For every line in the corpus
    for (const string& line : text) {
    	int start = 0;
	const int lineLength = line.size();
	for (int pos = 0 ; pos < lineLength ; pos++) {
	    if (line[pos] == ' ') {
		if ((pos - start) > longestWordLength) {
		    longestWord = line.substr(start,pos-start);
		    longestWordLength = longestWord.length();
		}
		start = pos + 1;
	    }
	}

	if ((lineLength - start) > longestWordLength) {
	    longestWord = line.substr(start,lineLength-start);
	    longestWordLength = longestWord.length();
	}
    }
    cout << "Longest word is '" << longestWord << "' length " << longestWordLength << endl;
}

void findLongestWordSimpleOptimizedParallel(const vector<string>& text) {
    string longestWord;
    int longestWordLength = 0;
    long lineCount = text.size();

    // For every line in the corpus
#pragma omp parallel for shared(longestWord, longestWordLength)
    for (int l = 0 ; l < lineCount ; l++) {
	const string& line = text[l];
    	int start = 0;
	const int lineLength = line.size();
	for (int pos = 0 ; pos < lineLength ; pos++) {
	    if (line[pos] == ' ') {
#pragma omp critical
		if ((pos - start) > longestWordLength) {
		    longestWord = line.substr(start,pos-start);
		    longestWordLength = longestWord.length();
		}
		start = pos + 1;
	    }
	}

#pragma omp critical
	if ((lineLength - start) > longestWordLength) {
	    longestWord = line.substr(start,lineLength-start);
	    longestWordLength = longestWord.length();
	}
    }
    cout << "Longest word is '" << longestWord << "' length " << longestWordLength << endl;
}
