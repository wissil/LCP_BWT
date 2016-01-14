#ifndef WTRank_h
#define WTRank_h

#include "IRank.h"
#include <sdsl/wavelet_trees.hpp>

using namespace sdsl;

//! Rank based on a Wavelet Tree
class WTRank: public IRank {
    
private:
    wt_huff<> *wt;
    
public:
    WTRank(wt_huff<> *w_tree) {
        this->wt = w_tree;
    }
    
    ~WTRank(){
    }
    
    index_type rank(index_type index, symbol_type symbol);
};

#endif /* WTRank_h */
