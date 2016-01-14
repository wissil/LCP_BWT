#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <ctime>
#include "mwt.h"

#include <sdsl/vectors.hpp>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <signal.h>
#include <execinfo.h>

using namespace std;
using namespace sdsl;

string BWT;
string orderedAlphabet;
//wt_huff<rrr_vector<63>> wt;
//wt_huff<> wt;

//List of overal number of occurences of characters in string S which are strictly
//smaller than c, for each c in alphabet
vector<int> C;

void handler(int sig) {
    void *array[10];
    int size;
    
    // get void*'s for all entries on the stack
    size = backtrace(array, 10);
    
    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(EXIT_FAILURE);
}

//Data structure for intervals
struct Interval {
    int start;
	int end;
};

//Data structure for queue elements
struct QueueElement {
        Interval interval;
        int l;
};

int rnk(int position, char symbol, node *root, int ary) {

	int layer_number = ceil((7/(log((double)ary)/log(double(2)))));
	vector<int> code_word = WordCoder((int)symbol, ary, layer_number);
	int r = Rank(position, code_word, root);

	return r;
}

string uniqueCharsInInterval(Interval interval) {
    set<char> s;
    unsigned size = interval.end - interval.start;
    for(unsigned i=interval.start-1; i<interval.end; i++) {
        s.insert(BWT[i]);
        if(s.size()==orderedAlphabet.size()) {
            break;
        }
    }
    string uniqueChars;
    uniqueChars.assign(s.begin(), s.end());
    //cout << uniqueChars << endl;
    return uniqueChars;
}

//Algorithm 1
vector<Interval> getIntervals(Interval interval, node* root, int ary) {

    vector<Interval> intervals;
    string uniqueChars = uniqueCharsInInterval(interval);

    for(int i=0; i<uniqueChars.length(); i++) {
        //Number of occurences of character uniqueChars[i] before interval.start index in BWT
        int a = rnk(interval.start-1, uniqueChars[i], root, ary);
        //Number of occurences of character uniqueChars[i] before interval.end index in BWT
        int b = rnk(interval.end, uniqueChars[i], root, ary);

        int charIndex = orderedAlphabet.find(uniqueChars[i]);
        //cout << "c:" << C[charIndex] << endl;
        Interval newInterval;
        newInterval.start = a + C[charIndex] +1;
        newInterval.end = b + C[charIndex];
        intervals.push_back(newInterval);
    }
    return intervals;
}

int main(int argc, char* argv[]){

 signal(SIGSEGV, handler);
    //Loading data from file
	if (argc < 2) {
        cout << "Usage: " << argv[0] << " file" << endl;
        return 1;
    }
	cout << argv[1]<< endl;
	string input;
	string line;
	ifstream myfile;
  	myfile.open (argv[1]);
  	if (myfile.is_open()) {
    		while (getline (myfile,line)) {
      			input += line;
    		}
    		myfile.close();
  	}
  	else
		cout << "Unable to open file";

cout << "argv[2]" << endl;

	int ary = atoi(argv[2]);;
    clock_t begin = clock();

	//Generating suffix array with sdsl library
	csa_bitcompressed<> csa;
	construct_im(csa, input, 1);

	//cout << csa << endl;

	//csXprintf(cout, "%2I %2S %3s %3P %2p %3B   %:3T", csa );

    //Storing ordered aplhabet in global variable
    orderedAlphabet += '$';
    for(int i=1; i!=csa.comp2char.size(); i++) {
        if(csa.comp2char[i] != ' ') {
            orderedAlphabet += csa.comp2char[i];
        }
    }

	//Creating BWT array
	string SA = extract(csa, 0, csa.size()-1);

	char BWTc[csa.size()];
	for (int i = 0; i < csa.size(); i++) {
		if(csa[i] == 0) {
			BWTc[i] = '$';
		}
		else {
			BWTc[i] = SA[csa[i]-1];
		}
	}

	string BWTh(BWTc);

    BWT = BWTh;
	
	//Generating wavelet tree from BWT with sdsl library
	//construct_im(wt, BWT, 1);
	//Generating multiary wavelet tree
	node* wt = generateMultiaryWT(BWT, ary);

    //Counting overal number of occurences of characters in string S which are strictly
    //smaller than c, for each c in alphabet
    for(int i=0; i < orderedAlphabet.length(); i++) {
        if(i==0) {
            C.push_back(0);
        }
        else {
            int r = C[i-1] + rnk(BWT.length(), orderedAlphabet[i-1], wt, ary);
            C.push_back(r);
        }
    }

    //Algorithm 2
    vector<int> LCP;

    LCP.push_back(-1);
    for(int i=0; i<BWT.size()-1; i++) {
        LCP.push_back(-2);
    }
    LCP.push_back(-1);

    Interval firstInterval;
	firstInterval.start =1;
	firstInterval.end = BWT.size();

	QueueElement firstElement;
	firstElement.interval = firstInterval;
	firstElement.l = 0;

    queue<QueueElement> queueLCP;
    queueLCP.push(firstElement);

    while(!queueLCP.empty()) {
        QueueElement element;
        element = queueLCP.front();
        queueLCP.pop();

        vector<Interval> intervals = getIntervals(element.interval, wt, ary);
        for(vector<Interval>::iterator it = intervals.begin(); it != intervals.end(); ++it) {
        //cout << "["<<it->start << ".." << it->end << "], " << element.l <<endl;
            if(LCP[it->end] == -2) {
                QueueElement newElement;
                newElement.interval = *it;
                newElement.l = element.l + 1;
                queueLCP.push(newElement);
                LCP[it->end] = element.l;

            }
        }
    }

    //cout << LCP << endl;

    clock_t end = clock();
    //for(vector<int>::iterator it = LCP.begin(); it != LCP.end(); ++it) {
    //   cout << *it << " ";
    //}
    //cout << endl;


    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;

    cout << "time: " << elapsed_secs << endl;
}








