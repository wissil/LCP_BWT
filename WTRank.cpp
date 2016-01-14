#include "WTRank.h"

//! Calculate rank based on a wavelet tree
/*!
 *  \param index        Index of the end of a sequence we are searching the rank for
 *  \param symbol       Symbol in the sequence we are searching the rank for
 */
index_type WTRank::rank(index_type index, symbol_type symbol) {
    return wt->rank(index, symbol);
}