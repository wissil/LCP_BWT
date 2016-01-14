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

int main(int argc, char* argv[]){ 
	string file = argv[1];
	
	cache_config cc = true;
	csa_wt<> csa;
	construct(csa, file, 1);

	cc.delete_files = true;
	lcp_wt<> LCP;

	 // Algorithm run: Start measuring time
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    
    // START THE ALGORITHM
    construct(LCP, file, 1);
	
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

	// get inFile name with no extension
    std::string filename = "./";
    filename.append(file.substr(0, file.find(".")));
    // append it with output extension
    filename.append("_lcp_SG_out.txt");
    std::ofstream output_file(filename);
    std::ostream_iterator<index_type> output_iterator(output_file, " ");
    std::copy(LCP.begin(), LCP.end(), output_iterator);
}
