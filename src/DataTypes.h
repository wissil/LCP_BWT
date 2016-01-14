#ifndef DataTypes_h
#define DataTypes_h

#include <string>

typedef long occurence_type;
typedef char symbol_type;
typedef long long index_type;

typedef struct {
    index_type startIndex;
    index_type endIndex;
} Interval;

typedef struct {
    Interval interval;
    index_type l;
} QueueElement;


#endif /* DataTypes_h */
