#ifndef MultiWTRank_hpp
#define MultiWTRank_hpp

#include "IRank.h"
#include "mwt.h"


class MultiWTRank : public IRank {
    
private:
    mwt::node *wt;
    int n_ary;
    
public:
    MultiWTRank(mwt::node *root, int ary) {
        wt = root;
        n_ary = ary;
    }
    
    ~MultiWTRank() {
    }
    
    index_type rank(index_type index, symbol_type symbol);
};

#endif /* MultiWTRank_hpp */
