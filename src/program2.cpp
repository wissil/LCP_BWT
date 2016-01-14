#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <ctime>

#include <sdsl/vectors.hpp>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/wavelet_trees.hpp>

using namespace std;
using namespace sdsl;

string BWT;
string orderedAlphabet;
//wt_huff<rrr_vector<63>> wt;
wt_huff<> wt;

//List of overal number of occurences of characters in string S which are strictly
//smaller than c, for each c in alphabet
vector<int> C;

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

//Data structure for queue elements
struct QueueElementNew {
        Interval interval;
	int size;
        int l;
};

string uniqueCharsInInterval(Interval interval) {
    /*set<char> s;
    unsigned size = interval.end - interval.start;
    for(unsigned i=interval.start-1; i<interval.end; i++) {
        s.insert(BWT[i]);
        if(s.size()==orderedAlphabet.size()) {
            break;
        }
    }
    string uniqueChars;
    uniqueChars.assign(s.begin(), s.end());
    */
    string uniqueChars;
    for(int i=0; i< orderedAlphabet.size(); i++) {
        if(wt.rank(interval.end, orderedAlphabet[i]) - wt.rank(interval.start-1, orderedAlphabet[i]) >0)
        {
            uniqueChars += orderedAlphabet[i];
        }
    }
    return uniqueChars;
}

//Algorithm 1
vector<Interval> getIntervals(Interval interval) {

    vector<Interval> intervals;
    string uniqueChars = uniqueCharsInInterval(interval);

    for(int i=0; i<uniqueChars.length(); i++) {
        //Number of occurences of character uniqueChars[i] before interval.start index in BWT
        int a = wt.rank(interval.start-1, uniqueChars[i]);
        //Number of occurences of character uniqueChars[i] before interval.end index in BWT
        int b = wt.rank(interval.end, uniqueChars[i]);

        int charIndex = orderedAlphabet.find(uniqueChars[i]);
        Interval newInterval;
        newInterval.start = a + C[charIndex] +1;
        newInterval.end = b + C[charIndex];
        intervals.push_back(newInterval);
    }
    return intervals;
}

int main(int argc, char* argv[]){

    //Loading data from file
	if (argc < 2) {
        cout << "Usage: " << argv[0] << " file" << endl;
        return 1;
    }
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

    clock_t begin = clock();

	//Generating suffix array with sdsl library
	csa_bitcompressed<> csa;
	construct_im(csa, input, 1);

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
	construct_im(wt, BWT, 1);

    //Counting overal number of occurences of characters in string S which are strictly
    //smaller than c, for each c in alphabet
    for(int i=0; i < orderedAlphabet.length(); i++) {
        if(i==0) {
            C.push_back(0);
        }
        else {
            int rank = C[i-1] + wt.rank(wt.size(), orderedAlphabet[i-1]);
            C.push_back(rank);
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

        vector<Interval> intervals = getIntervals(element.interval);
        for(vector<Interval>::iterator it = intervals.begin(); it != intervals.end(); ++it) {
            if(LCP[it->end] == -2) {
                QueueElement newElement;
                newElement.interval = *it;
                newElement.l = element.l + 1;
                queueLCP.push(newElement);
                LCP[it->end] = element.l;
            }
        }
    }

    clock_t end = clock();
    //for(vector<int>::iterator it = LCP.begin(); it != LCP.end(); ++it) {
    //    cout << *it << " ";
    //}
    cout << endl;


    double elapsed_secs = double(end-begin) / CLOCKS_PER_SEC;

    cout << "time: " << elapsed_secs << endl;
}








