#include "OrderedAlphabet.hpp"


//! Public constructor
//! Constructs the ordered alphabet from the suffix array
/*!
 *  \param csa  Compressed suffix array
 */
OrderedAlphabet::OrderedAlphabet(csa_bitcompressed<> csa) {
    size = 0;
    for (int i=0; i<csa.comp2char.size(); i++) {
        symbol_type current = csa.comp2char[i];
        symbols.push_back(current);
        size += 1;
    }

}

//! Public destructor
OrderedAlphabet::~OrderedAlphabet() {
}

//! Method that returns a number of different symbols of this alphabet.
int OrderedAlphabet::getSize() {
    return size;
}


//! Method that returns all symbols of this alphabet.
std::string OrderedAlphabet::getSymbols() {
    return symbols;
}

//! Method that returns a string interval
/*!
 *  \param startIdx     Starting Index of the interval
 *  \param endIdx       Ending Index of the interval
 */
OrderedAlphabet::Interval OrderedAlphabet::getInterval(index_type startIdx, index_type endIdx) {
    index_type len = endIdx - startIdx;
    std::string seq = symbols.substr(startIdx, len);
    
    // create interval
    Interval interval;
    interval.startIndex = startIdx;
    interval.endIndex = endIdx;
    interval.sequence = seq;
    return interval;
}