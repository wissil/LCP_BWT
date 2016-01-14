#include "LinRank.hpp"


//! Calculate rank NOT based on a wavelet tree
//! Linear computation of rank, runs in O(n)
/*!
 *  \param index        Index of the end of a sequence we are searching the rank for
 *  \param symbol       Symbol in the sequence we are searching the rank for
 */
index_type LinRank::rank(index_type index, symbol_type symbol) {
    index_type count = 0;
    for(index_type i=0; i<index; i++) {
        if(BWT[i] == symbol) {
            count++;
        }
    }
    return count;
}