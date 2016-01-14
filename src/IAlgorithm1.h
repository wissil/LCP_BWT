#ifndef IAlgorithm1_h
#define IAlgorithm1_h

#include "OrderedAlphabet.hpp"
#include "IRank.h"

//! Algorithm1 interface
class IAlgorithm1 {
    
protected:
    virtual std::string uniqueCharsInInterval(index_type i, index_type j)=0;
 
    
public:
    
    IAlgorithm1(){
    }
    
    ~IAlgorithm1() {
    }
    
    virtual std::vector<Interval> getIntervals(index_type i, index_type j)=0;
};

#endif /* Algorithm1_h */
