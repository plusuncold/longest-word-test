#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "timer.h"

using namespace std;

void findLongestWord(const vector<string>& text);
void findLongestWordSimple(const vector<string>& text);

int main() {
    string path = "corpus.txt";
    vector<string> text;
    ifstream file(path.c_str());
    string line;
    while(getline(file,line)) {
	text.push_back(line);
    }
    file.close();

    auto id = Timer::timerBegin();
    findLongestWord(text);
    auto timeStriding = Timer::timerEnd(id);

    id = Timer::timerBegin();
    findLongestWordSimple(text);
    auto timeSimple = Timer::timerEnd(id);

    cout << "Striding: " << timeStriding << "us\n"
	 << "Simple: " << timeSimple << "us" << endl;

    return 0;
}

void innerTestForLongest(const string& line, const int start, const int end,
			 string& longestWord, int& longestWordLength) {
    for (int index = start ; index <= end ; index++) {
	//cout << "Is " << line[index] << " a space?" << endl;
	if (line[index] == ' ') {
	    if (end - (index + 1) > longestWordLength) {
		//cout << "Searching smaller: $" << line.substr(index+1,end - index) << "$" << endl;
		innerTestForLongest(line, index+1, end, longestWord, longestWordLength);
	    }
	    return;
	}
    }

    // If reached here then the word is continuous and thus the longest word
    longestWord = line.substr(start,end - start + 1);
    longestWordLength = end - start + 1;
    //cout << "Longest word is '" << longestWord << "' length " << longestWordLength << endl;
}

void findLongestWord(const vector<string>& text) {
    string word, longestWord;
    int longestWordLength = 0;

    for (const string& line : text) {
	const int lineLength = line.size();
	//cout << "$" << line << "$" << lineLength << endl;
	// Stride along line spliting into longestWordLength chunks
	int start = 0, pos = longestWordLength + 1;
	while (pos < lineLength) {
	    //cout << "start " << start << " pos " << pos << endl;
	    if (line[pos] == ' ') {
		//cout << "char $" << line[pos] << "$ space found - word $"
		//    << line.substr(start, pos - start) << "$" << endl;
		innerTestForLongest(line, start, pos - 1, longestWord, longestWordLength);
		start = pos + 1;
		pos = start + longestWordLength + 1;
	    } else {
		//cout << line[pos] << " no space" << endl;
		pos++;
	    }
	}
	//cout << "after stride loop" << endl;
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
