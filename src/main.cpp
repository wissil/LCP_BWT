#include <execinfo.h>

#include <sdsl/vectors.hpp>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/wavelet_trees.hpp>


#include "OrderedAlphabet.hpp"
#include "DataTypes.h"

#include "WTRank.h"
#include "LinRank.hpp"
#include "MultiWTRank.hpp"


#include "Algorithm1A.h"
#include "Algorithm2A.hpp"

#include <sys/time.h>
#include <sys/resource.h>

#include <fstream>
#include <iterator>

#include <sdsl/lcp.hpp>
#include <signal.h>



using namespace std;
using namespace sdsl;

// =================================================================================================
// =================================================================================================


//! Utility function that traces the stack if segmentation fault occurs
/*!
 *  \param sig      Signal that aborts the program from running
 */
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

// =================================================================================================
// =================================================================================================
void readFileToMemory(std::string *input, std::string inFile);

std::string generateBWT(csa_bitcompressed<> csa);
std::map<symbol_type, occurence_type> calculateC(IRank *rank,
                                                 OrderedAlphabet alphabet,
                                                 std::string BWT);
std::vector<Interval> getIntervals(int i, int j);
void writeToFile(std::vector<index_type> LCP, std::string inFile);

std::vector<index_type> run(IRank *rank, OrderedAlphabet a, std::string BWT);
// =================================================================================================
// ==================================================================================================


/**
 *  Program entry-point :: main function
 *
 */
int main(int argc, char* argv[]){
    
    signal(SIGSEGV, handler);
    
    std::string input;
    std::string inFileName = argv[1];
    
    // Options: non_wt, m_wt, b_wt
    std::string option = argv[2];
    bool non_wt = false;
    bool m_wt = false;
    bool b_wt = false;
    
    // Variable used only for multiary Wavelet tree
    // It is passed as the 4th argument to main, only if string m_wt is inputed
    int ary = 1;
    
    if(option.compare("m_wt") == 0) {
        ary = atoi(argv[3]);
        m_wt = true;
    } else if(option.compare("non_wt") == 0) {
        non_wt = true;
    } else if(option.compare("b_wt") == 0) {
        b_wt = true;
    } else {
        // Error
        fprintf(stderr, "Wrong option for rank is passed. Terminating ...");
        exit(EXIT_FAILURE);
    }
    
    
    // Loading data from file
    readFileToMemory(&input, inFileName);
    
    //Generating suffix array with sdsl library
    csa_bitcompressed<> csa;
    construct_im(csa, input, 1);
    
    // Creating an ordered alphabet from a known suffix array
    OrderedAlphabet alphabet(csa);
    
    // Generating BWT transform of a given sequence
    std::string BWT = generateBWT(csa);
    
    // Decide what rank function to use
    std::vector<index_type> LCP;
    
    // Algorithm run: Start measuring time
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    
    if(m_wt) {
        // Multiary Wavelet Tree rank function :: student implementation
        mwt::node *m_wtree = mwt::generateMultiaryWT(BWT, ary);
        MultiWTRank *m_wt_rank = new MultiWTRank(m_wtree, ary);
        LCP = run(m_wt_rank, alphabet, BWT);
    } else if(b_wt) {
        // Binary Wavelet Tree rank function :: SDSL implementation
        wt_huff<> b_wtree;
        construct_im(b_wtree, BWT, 1);
        WTRank *b_wt_rank = new WTRank(&b_wtree);
        LCP = run(b_wt_rank, alphabet, BWT);
    } else if(non_wt) {
        // Implementation of rank function not using Wavelet Tree
        LinRank *lin_rank = new LinRank(BWT);
        LCP = run(lin_rank, alphabet, BWT);
    } else {
        // should not happen
    }
    
    // Algorithm stop: Stop measuring time
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    
    // Print algorithm run-time on screen
    auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
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
    
    // Write LCP vector to a file
    writeToFile(LCP, inFileName);
}


// =================================================================================================
// =================================================================================================


std::vector<index_type> run(IRank *rank, OrderedAlphabet alphabet, std::string BWT) {
    // Calculate overall number of character occurances smaller than c
    // for every character c
    std::map<symbol_type, occurence_type> C = calculateC(rank, alphabet, BWT);
    
    // Algorithm 1 implementation
    Algorithm1A *algorithm1A = new Algorithm1A(rank, &alphabet, &C, &BWT);
    
    // Algorithm 2 implementation
    Algorithm2A *algorithm2A = new Algorithm2A(algorithm1A);
    
    return algorithm2A->calculateLCP(BWT);
}

//! Write the comouted LCP array to the output file
/*!
 *  \param LCP          Computed LCP array
 *  \param inFile       Name of the input file: used for the output file name
 */
void writeToFile(std::vector<index_type> LCP, std::string inFile) {
    // get inFile name with no extension
    std::string filename = "./";
    filename.append(inFile.substr(0, inFile.find(".")));
    // append it with output extension
    filename.append("_lcp_out.txt");
    std::ofstream output_file(filename);
    std::ostream_iterator<index_type> output_iterator(output_file, " ");
    std::copy(LCP.begin(), LCP.end(), output_iterator);
}


//! Calculate overall number of character occurances that are
//  strictly smaller than c, for each c in alphabet
/*!
 *  \param wt           Generated wavelet tree from given sequence S
 *  \param alphabet     Ordered alphabet
 */
std::map<symbol_type, occurence_type> calculateC(IRank *rank,
                                                 OrderedAlphabet alphabet,
                                                 std::string BWT){
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
            n = n_prev + rank->rank(BWT.length(), c_prev);
        }
        C.insert(pair<symbol_type, occurence_type>(c, n));
    }
    return C;
}



//! Generate a BWT transformation string
//BWT[i] = S[SA[i]−1] for all i with SA[i]=0 and BWT[i]='\0'
/*!
 *  \param csa  Suffix Array of a given sequence
 */
std::string generateBWT(csa_bitcompressed<> csa) {
    symbol_type sentinel_symbol = csa.comp2char[0];
    std::string s = extract(csa, 0, csa.size()-1);
    std::string bwt;
    for(int i=0; i<csa.bwt.size(); i++) {
        if(csa[i] == 0) {
            bwt.push_back(sentinel_symbol);
        } else {
            bwt.push_back(s[csa[i] - 1]);
        }
    }
    return bwt;
}

//! Generate a BWT transformation string
/*!
 *  \param csa  Suffix Array of a given sequence
 */
/*std::string generateBWT(csa_bitcompressed<> csa) {
    std::string bwt;
    for(int i=0; i<csa.bwt.size(); i++) {
        bwt.push_back(csa.bwt[i]);
    }
    return bwt;
}*/


//! Read an entire text file to memory
/*!
 *  \param *input       Pointer to a memory block of type string
 *  \param inFile       Name of a text file being loaded to memory
 */
void readFileToMemory(string *input, string inFile) {
    std::string line;
    std::ifstream file_stream;
    file_stream.open(inFile);
    
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
