#ifndef IAlgorithm1A_h
#define IAlgorithm1A_h

#include "IAlgorithm1.h"

class Algorithm1A : public IAlgorithm1 {

protected:
    IRank *rank;
    OrderedAlphabet *alphabet;
    std::map<symbol_type, occurence_type> *C;
    std::string *BWT;
    
public:
    Algorithm1A(IRank *r,
                OrderedAlphabet *a,
                std::map<symbol_type, occurence_type> *c,
                std::string *bwt):
    rank(r), alphabet(a), C(c), BWT(bwt) {
    }
    
    ~Algorithm1A(){
    }
    
    
    std::string uniqueCharsInInterval(index_type i, index_type j);
    
    std::vector<Interval> getIntervals(index_type i, index_type j);
    
};

#endif /* Algorithm1A_h */
