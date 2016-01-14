#ifndef Algorithm2A_hpp
#define Algorithm2A_hpp

#include "IAlgorithm2.h"

class Algorithm2A : public IAlgorithm2 {
    
protected:
    IAlgorithm1 *algorithm1;
    
    std::vector<Interval> getIntervals(index_type i, index_type j) {
        return algorithm1->getIntervals(i, j);
    }
    
public:
    Algorithm2A(IAlgorithm1 *a) : algorithm1(a) {
    }
    
    ~Algorithm2A(){
    }
    
    std::vector<index_type> calculateLCP(std::string BWT);
};

#endif /* Algorithm2A_hpp */
