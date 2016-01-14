#include "Algorithm2A.hpp"


std::vector<index_type> Algorithm2A::calculateLCP(std::string BWT) {
    std::vector<index_type> LCP;
    
    LCP.push_back(1);
    for(int i=0; i<BWT.size()-1; i++) {
        LCP.push_back(-2);
    }
    LCP.push_back(-1);
    
    Interval firstInterval;
    firstInterval.startIndex = 1;
    firstInterval.endIndex = BWT.size();
    
    QueueElement firstElement;
    firstElement.interval = firstInterval;
    firstElement.l = 0;
    
    queue<QueueElement> queueLCP;
    queueLCP.push(firstElement);
    
    while(!queueLCP.empty()) {
        QueueElement element;
        element = queueLCP.front();
        queueLCP.pop();
        
        vector<Interval> intervals = getIntervals(element.interval.startIndex,
                                                  element.interval.endIndex);
        
        for(vector<Interval>::iterator it = intervals.begin(); it != intervals.end(); ++it) {
            if(LCP[it->endIndex] == -2) {
                QueueElement newElement;
                newElement.interval = *it;
                newElement.l = element.l + 1;
                queueLCP.push(newElement);
                LCP[it->endIndex] = element.l;
            }
        }
    }
    return LCP;
}
