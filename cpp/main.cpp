#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <omp.h>

#include "timer.h"

#define CHECK_BIT(var,pos) !!((var) & (1<<(pos)))

using namespace std;

void findLongestWord(const string& text);
//void findLongestWordParallel(const string& text);
void findLongestWordSimple(const vector<string>& text);
void findLongestWordSimpleOptimized(const vector<string>& text);
void findLongestWordSimpleOptimizedParallel(const vector<string>& text);

int main(int argc, char *argv[]) {
    if (argc > 2) {
	cout << "Requires corpus locations" << endl;
	exit(0);
    }
    string path(argv[1]);
    
    ifstream file(path.c_str());

    if (!file.is_open()) {
	cout << "No file at: " << path << endl;
	exit(-1);
    }
    
    stringstream buffer;
    buffer << file.rdbuf();
    string text(buffer.str());
    file.close();

    // Run the striding test
    auto id = Timer::timerBegin();
    findLongestWord(text);
    auto timeStriding = Timer::timerEnd(id);

    // Run the striding test (parallel)
    id = Timer::timerBegin();
    //findLongestWordParallel(text);
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

inline bool isDelimiter(const string& text, const long pos) {
    if (CHECK_BIT(text[pos],6)) return false;
    if (text[pos] == ' ') return true;
    if (text[pos] == '\n') return true;
    return false;
}

void innerTestForLongest(const string& text, const long start, const long end, const long posStart, long& longestWordStart, int& longestWordLength) {

    // Iterate through every character in the range
    for (int index = start ; index < posStart ; index++) {
	// If this chunk has a space
	if (isDelimiter(text,index)) {
	    // If left chunk could have longest word check it
	    if (index - start > longestWordLength) {
		innerTestForLongest(text, start, index - 1, index - 1, longestWordStart, longestWordLength);
	    }

	    // If right chunk could have longest word check it
	    if (end - (index + 1) > longestWordLength) {
		innerTestForLongest(text, index+1, end, posStart, longestWordStart, longestWordLength);
	    }
	    return;
	}
    }

    // If reached here then the word is continuous and thus the longest word
    longestWordStart = start;
    longestWordLength = end - start + 1;
}

void findLongestWord(const string& text) {
    string word;
    long longestWordStart = 0;
    int longestWordLength = 0;

    const int textLength = text.size();
    
    // Stride along text spliting into longestWordLength chunks
    int start = 0, pos = longestWordLength + 1;
    int posStart = (pos < textLength) ? pos : textLength - 1;
    while (pos < textLength) {
	if (isDelimiter(text,pos)) {
	    // Check if the chunk between start and (pos - 1) contains the longest word
	    innerTestForLongest(text, start, pos - 1, posStart, longestWordStart, longestWordLength);
	    
	    // Set start to character after the found space
	    start = pos + 1;
	    // Stride forward read position to first position that could contain a longest word
	    pos = start + longestWordLength + 1;
	    posStart = (pos < textLength) ? pos : textLength - 1;
	} else {
	    pos++;
	}
    }
    
    // Check if end of line has a longest word
    if (textLength - 1 - start > longestWordLength) {
	innerTestForLongest(text, start, textLength - 1, posStart, longestWordStart, longestWordLength);
    }
    
    cout << "Longest word is '";
    for (int pos = longestWordStart ; pos < longestWordStart + longestWordLength ; pos++)
	cout << text[pos];
    cout << "' length " << longestWordLength << "\n";
}

/*
void findLongestWordParallel(const string& text) {
    string word;
    long longestWordStart = 0;
    int longestWordLength = 0;
    long textLength = text.size();
    const short threadCount = omp_get_max_threads();
    
    // For every line in the corpus
#pragma omp parallel for shared(longestWord)
    for (int t = 0 ; t < threadCount ; t++) {
	lw localLongestWord;
	localLongestWord.length = 0;
	const int lStart = lineCount * omp_get_thread_num() / threadCount;
	const int lEnd = lineCount * (omp_get_thread_num() + 1) / threadCount - 1;
	
	for (int l = lStart ; l <= lEnd && l < lineCount ; l++) {
	    const string& line = text[l];
	    const int lineLength = line.size();
	    
	    // Stride along line spliting into longestWordLength chunks
	    int start = 0, pos = localLongestWord.length + 1;
	    int posStart = (pos < lineLength) ? pos : lineLength - 1;
	    while (pos < lineLength) {
		if (line[pos] == ' ') {
		    // Check if the chunk between start and (pos - 1) contains the longest word
		    innerTestForLongest(line, start, pos - 1, posStart, l, localLongestWord);
		    
		    // Set start to character after the found space
		    start = pos + 1;
		    // Stride forward read position to first position that could contain a longest word
		    pos = start + localLongestWord.length + 1;
		    posStart = (pos < lineLength) ? pos : lineLength - 1;
		} else {
		    pos++;
		}
	    }
	    
	    // Check if end of line has a longest word
	    if (lineLength - 1 - start > localLongestWord.length) {
		innerTestForLongest(line, start, lineLength - 1, posStart, l, localLongestWord);
	    }
	}

#pragma omp critical
	{
	    if (localLongestWord.length > longestWord.length) {
		longestWord.length = localLongestWord.length;
		longestWord.lineNumber = localLongestWord.lineNumber;
		longestWord.start = localLongestWord.start;
	    }
	}
    }

    const string& longestWordLine = text[longestWord.lineNumber];
    cout << "Longest word is '";
    for (int pos = longestWord.start ; pos < longestWord.start + longestWord.length ; pos++)
	cout << longestWordLine[pos];
    cout << "' length " << longestWord.length << "\n";
}
*/

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
