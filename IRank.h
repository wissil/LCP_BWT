#ifndef IRank_h
#define IRank_h

#include "DataTypes.h"

//! Rank interface
class IRank {
public:
    IRank() {
    }
    
    ~IRank() {
    }
    
    virtual index_type rank(index_type index, symbol_type symbol)=0;
};

#endif /* Rank_h */
