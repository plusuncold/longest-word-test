#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <omp.h>

#include "timer.h"

using namespace std;

constexpr short ITERATION_COUNT = 10;

void findLongestWord(const string& text);
void findLongestWordParallel(const string& text);
void findLongestWordSimple(const string& text);
void findLongestWordSimpleOptimized(const string& text);
//void findLongestWordSimpleOptimizedParallel(const string& text);

int main(int argc, char *argv[]) {
    if (argc < 2) {
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
    for (short iteration = 0 ; iteration < ITERATION_COUNT ; iteration++) {
	//findLongestWord(text);
    }
    auto timeStriding = Timer::timerEnd(id);
    timeStriding /= ITERATION_COUNT;

    // Run the striding test (parallel)
    id = Timer::timerBegin();
    for (short iteration = 0 ; iteration < ITERATION_COUNT ; iteration++) {
	findLongestWordParallel(text);
    }
    auto timeStridingParallel = Timer::timerEnd(id);
    timeStridingParallel /= ITERATION_COUNT;
    
    // Run the simple test
    id = Timer::timerBegin();
    findLongestWordSimple(text);
    auto timeSimple = Timer::timerEnd(id);

    // Run the simple test optimized
    id = Timer::timerBegin();
    for (short iteration = 0 ; iteration < ITERATION_COUNT ; iteration++) {
	findLongestWordSimpleOptimized(text);
    }
    auto timeSimpleOpt = Timer::timerEnd(id);
    timeSimpleOpt /= ITERATION_COUNT;
    
    // Run the simple test optimized (parallel)
    id = Timer::timerBegin();
    //findLongestWordSimpleOptimizedParallel(text);
    auto timeSimpleOptParallel = Timer::timerEnd(id);

    cout << "Striding: " << timeStriding/1000 << "ms\n"
	 << "Striding parallel " << timeStridingParallel/1000 << "ms\n"
	 << "Simple: " << timeSimple/1000 << "ms\n"
	<< "Simple opt " << timeSimpleOpt/1000 << "ms\n"
       //<< "Simple opt parallel " << timeSimpleOptParallel/1000 << "ms\n"
	 << endl;

    return 0;
}

void innerTestForLongest(const string& text, const long start, const long end, const long posStart, long& longestWordStart, int& longestWordLength) {

    // Iterate through every character in the range
    for (int index = start ; index < posStart ; index++) {
	// If this chunk has a space
	if (text[index] == ' ' || text[index] == '\n') {
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
	if (text[pos] == ' ' || text[pos] == '\n') {
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


void findLongestWordParallel(const string& text) {
    string word;
    long longestWordStart = 0;
    int longestWordLength = 0;
    long textLength = text.size();
    const int threadCount = /* omp_get_thread_limit() */ 12;
    if (threadCount < 2) {
	cout << "Thread error " << threadCount << endl;
	exit(-1);
    }

    vector<long> startPositionsForEachThread(threadCount+1,0);
    startPositionsForEachThread[threadCount] = textLength;
    for (short i = 0 ; i < threadCount ; i++) {
	// read along the line until a delimiter is found
	long startPosition = i * textLength / threadCount;
	while (text[startPosition] != ' ' && text[startPosition] != '\n') startPosition++;
	startPositionsForEachThread[i] = startPosition;

	//cout << "Start position " << i << " at " << startPosition << " char is $" << text[startPosition] << "$" << endl;
    }
    
    // For every line in the corpus
#pragma omp parallel for shared(longestWordStart, longestWordLength, startPositionsForEachThread)
    for (int t = 0 ; t < threadCount ; t++) {
	long localLongestWordStart = 0;
	int localLongestWordLength = 0;
	const long lStart = startPositionsForEachThread[t];
	const long lEnd = startPositionsForEachThread[t+1] - 1;
		    
	// Stride along line spliting into longestWordLength chunks
	int start = lStart, pos = start + localLongestWordLength + 1;
	int posStart = (pos < lEnd) ? pos : lEnd - 1;
	while (pos < lEnd) {
	    if (text[pos] == ' ' || text[pos] == '\n') {
		// Check if the chunk between start and (pos - 1) contains the longest word
		innerTestForLongest(text, start, pos - 1, posStart, localLongestWordStart, localLongestWordLength);
		
		// Set start to character after the found space
		start = pos + 1;
		// Stride forward read position to first position that could contain a longest word
		pos = start + localLongestWordLength + 1;
		posStart = (pos < lEnd) ? pos : lEnd - 1;
	    } else {
		pos++;
	    }
	}
	
	// Check if end of line has a longest word
	if (lEnd - 1 - start > localLongestWordLength) {
	    innerTestForLongest(text, start, lEnd - 1, posStart, localLongestWordStart, localLongestWordLength);
	}

#pragma omp critical
	{
	    if (localLongestWordLength > longestWordLength) {
		longestWordLength = localLongestWordLength;
		longestWordStart = localLongestWordStart;
	    }
	}
    }

    cout << "Longest word is '";
    int longestWordEnd = longestWordStart + longestWordLength - 1;
    for (int pos = longestWordStart ; pos <= longestWordEnd ; pos++)
	cout << text[pos];
    cout << "' length " << longestWordLength << "\n";
}


void findLongestWordSimple(const string& text) {
    string word, longestWord;
    int longestWordLength = 0;

    istringstream iss(text);
    do {
	iss >> word;
	if (word.length() > longestWordLength) {
	    longestWord = word;
	    longestWordLength = word.length();
	}
    } while (iss);

    cout << "Longest word is '" << longestWord << "' length " << longestWordLength << endl;
}

void findLongestWordSimpleOptimized(const string& text) {
    string longestWord;
    int longestWordLength = 0;

    int start = 0;
    const int textLength = text.size();
    for (int pos = 0 ; pos < textLength ; pos++) {
	if (text[pos] == ' ' || text[pos] == '\n') {
	    if ((pos - start) > longestWordLength) {
		longestWord = text.substr(start,pos-start);
		longestWordLength = longestWord.length();
	    }
	    start = pos + 1;
	}
    }

    if ((textLength - start) > longestWordLength) {
	longestWord = text.substr(start,textLength-start);
	longestWordLength = longestWord.length();
    }

    cout << "Longest word is '" << longestWord << "' length " << longestWordLength << endl;
}

/*
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
*/
