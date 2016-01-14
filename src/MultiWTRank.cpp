//
//  MultiWTRank.cpp
//  Toster
//
//  Created by Filip on 13/01/16.
//  Copyright © 2016 Filip. All rights reserved.
//

#include "MultiWTRank.hpp"


//! Implementation of rank function using Multiary Wavelet Tree
/*!
 *  \param index        Index of the end of a sequence we are searching the rank for
 *  \param symbol       Symbol in the sequence we are searching the rank for
 */
index_type MultiWTRank::rank(index_type index, symbol_type symbol) {
    int layer_number = ceil((7/(log((double)n_ary)/log(double(2)))));
    vector<int> code_word = mwt::WordCoder((int)symbol, n_ary, layer_number);
    return mwt::Rank((int)index, code_word, wt);
}