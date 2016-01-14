#include <iostream>

#include <execinfo.h>


#include <sdsl/vectors.hpp>
#include <sdsl/lcp.hpp>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <sdsl/config.hpp>


#include "OrderedAlphabet.hpp"
#include "DataTypes.h"
#include "WTRank.h"

#include "Algorithm1A.h"
#include "Algorithm2A.hpp"

using namespace std;
using namespace sdsl;

void handler(int sig) {
    void *array[10];
    int size;
    
    // get void*'s for all entries on the stack
    size = backtrace(array, 10);
    
    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}


void readFileToMemory(string *input, string filename);

std::string generateBWT(csa_bitcompressed<> csa);
std::map<symbol_type, occurence_type> calculateC(wt_huff<> wt,
                                                 OrderedAlphabet alphabet);
std::vector<Interval> getIntervals(int i, int j);

int main(int argc, char* argv[]){ 
	string file = argv[1];
	
	cache_config cc = false;
	csa_wt<> csa;
	construct(csa, file, 1);

	cc.delete_files = true;
	lcp_wt<> lcp;

	

	 // Algorithm run: Start measuring time
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    
    // START THE ALGORITHM
    construct(lcp, file, 1);
	cout << lcp << endl;
	
    // Algorithm stop: Stop measuring time
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    
    // Print algorithm run-time on screen
    auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();
    cout << "Algorithm Duration: " << duration / ((float)1000000) << " seconds" << endl;


    
    /*
     *  Measure memory usage of current process...
     *  Measuring RSS... Resident Set Size
     *  the portion of memory occupied by a process 
     *  that is held in main memory (RAM)
     *
     *  @param usage    Structure that is holding data of the memory usage
     */
    {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        cout << "Memory usage: "<< usage.ru_maxrss/((float)1024) << " MB of RAM" << endl;
    }
}

//! Calculate overall number of character occurances that are
//  strictly smaller than c, for each c in alphabet
/*!
 *  \param wt           Generated wavelet tree from given sequence S
 *  \param alphabet     Ordered alphabet
 */
std::map<symbol_type, occurence_type> calculateC(wt_huff<> wt,
                                                 OrderedAlphabet alphabet) {
    std::map<symbol_type, occurence_type> C;
    // for each symbol c: calculate number of occurences of smaller symbols
    for(int sym_idx=0; sym_idx<alphabet.getSize(); sym_idx++) {
        symbol_type c = alphabet.getSymbols().at(sym_idx);
        occurence_type n;
        
        if(sym_idx == 0) {
            n = 0;
        } else {
            symbol_type c_prev = alphabet.getSymbols().at(sym_idx-1);
            occurence_type n_prev = C.find(c_prev) -> second;
            n = n_prev + wt.rank(wt.size(), c_prev);
        }
        C.insert(pair<symbol_type, occurence_type>(c, n));
    }
    return C;
}


//! Generate a BWT transformation string
/*!
 *  \param csa  Suffix Array of a given sequence
 */
std::string generateBWT(csa_bitcompressed<> csa) {
    std::string bwt;
    for(int i=0; i<csa.bwt.size(); i++) {
        bwt.push_back(csa.bwt[i]);
    }
    return bwt;
}


//! Read an entire text file to memory
/*!
 *  \param *input       Pointer to a memory block of type string
 *  \param file_name    Name of a text file being loaded to memory
 */
void readFileToMemory(string *input, string file_name) {
    std::string line;
    std::ifstream file_stream;
    file_stream.open(file_name);
    
    // Loading file content to memory
    if (file_stream.is_open()) {
        while (getline(file_stream, line)) {
            *input += line;
        }
        file_stream.close();
    }
    else {
        std::cout << "Unable to open file. Terminating..." << endl;
        exit(EXIT_FAILURE);
    }
}
