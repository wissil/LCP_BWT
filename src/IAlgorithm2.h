#ifndef IAlgorithm2_h
#define IAlgorithm2_h

#include "IAlgorithm1.h"

//! Algorithm2 interface
class IAlgorithm2 {
    
public:

    IAlgorithm2(){
    }
    
    ~IAlgorithm2() {
    }
    
    virtual std::vector<index_type> calculateLCP(std::string BWT)=0;
};

#endif /* IAlgorithm2_h */
