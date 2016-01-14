# include "Algorithm1A.h"


std::string Algorithm1A::uniqueCharsInInterval(index_type i, index_type j) {
        std::string uniqueChars;
        for(int k=0; k < alphabet.getSize(); k++) {
            if(rank.rank(j, alphabet.getSymbols().at(k)) -
               rank.rank(i-1, alphabet.getSymbols().at(k)) > 0) {
                
                uniqueChars += alphabet.getSymbols().at(k);
            }
        }
        return uniqueChars;
    }

//std::string Algorithm1A::uniqueCharsInInterval(index_type i, index_type j) {
//    std::set<char> s;
//    for(index_type k=i-1; k<j; k++) {
//        s.insert(BWT[k]);
//        if(s.size()==alphabet.getSize()) {
//            break;
//        }
//    }
//    string uniqueChars;
//    uniqueChars.assign(s.begin(), s.end());
//    return uniqueChars;
//}

    
std::vector<Interval> Algorithm1A::getIntervals(index_type i, index_type j) {
    std::vector<Interval> intervals;
    std::string uniqueChars = uniqueCharsInInterval(i, j);
    
    for(int k=0; k<uniqueChars.length(); k++) {
        //Number of occurences of character uniqueChars[i] before interval.start index in BWT
        index_type a = rank.rank(i-1, uniqueChars[k]);
        //Number of occurences of character uniqueChars[i] before interval.end index in BWT
        index_type b = rank.rank(j, uniqueChars[k]);
        
        symbol_type symbol = uniqueChars[k];
        Interval newInterval;
        newInterval.startIndex = (a + C.find(symbol)->second + 1);
        newInterval.endIndex = (b + C.find(symbol)->second);
        intervals.push_back(newInterval);
    }
    return intervals;
}
