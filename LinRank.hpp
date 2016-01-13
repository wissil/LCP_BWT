#ifndef LinRank_hpp
#define LinRank_hpp

#include "IRank.h"

//! Rank based on a Wavelet Tree
class LinRank: public IRank {
   
private:
    std::string BWT;
    
public:
    LinRank(std::string bwt) {
        this->BWT = bwt;
    }
    
    ~LinRank(){
    }
    
    index_type rank(index_type index, symbol_type symbol);
};
#endif /* LinRank_hpp */
