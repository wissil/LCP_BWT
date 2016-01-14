#ifndef OrderedAlphabet_hpp
#define OrderedAlphabet_hpp

#include <sdsl/suffix_arrays.hpp>
#include "DataTypes.h"

using namespace std;
using namespace sdsl;

//! Ordered Alphabet class
/*!
 *  The alphabet always contains the smallest, "sentinel" character "NULL".
 *  Sentinel character = ASCII 0
 *  The alphabet is consisted of size number of symbols, including the sentinel.
 */
class OrderedAlphabet {
    
private:
    int size;
    std::string symbols;
    
    
public:
    OrderedAlphabet(csa_bitcompressed<> csa);
    ~OrderedAlphabet();
    
    typedef struct {
        index_type startIndex;
        index_type endIndex;
        std::string sequence;
    } Interval;
    
    int getSize();
    std::string getSymbols();
    
    OrderedAlphabet::Interval getInterval(index_type startIdx, index_type endIdx);
};

#endif /* OrderedAlphabet_hpp */
