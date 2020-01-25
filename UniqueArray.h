#ifndef MTMPARKINGLOT_UNIQUEARRAY_H
#define MTMPARKINGLOT_UNIQUEARRAY_H


template <class Element, class Compare = std::equal_to<Element>>
class UniqueArray {


    const Element* const EMPTY_SLOT = NULL;
    const unsigned int maxSize;
    bool findCellWithElement( unsigned int& index , const Element* elementPtr )const;//return true if found empty cell, false otherwise
    unsigned int elementCounter;
    Element ** arrayOfElements;

public:

    UniqueArray(unsigned int size);
    UniqueArray(const UniqueArray& other);
    ~UniqueArray();
    UniqueArray& operator=(const UniqueArray&) = delete;
    unsigned int insert(const Element& element);
    bool getIndex(const Element& element, unsigned int& index) const;
    const Element* operator[] (const Element& element) const;
    bool remove(const Element& element);
    unsigned int getCount() const;
    unsigned int getSize() const;




    class Filter {
    public:
        virtual bool operator() (const Element&) const = 0;
    };
    UniqueArray filter(const Filter& f) const;

    class UniqueArrayIsFullException{};


    class MyIterator : public std::iterator<std::input_iterator_tag, int>
    {
        Element** p;
    public:
        MyIterator(UniqueArray& x) :p(x.arrayOfElements) {}
        MyIterator(const MyIterator& mit) : p(mit.p) {}
        MyIterator& operator++() {++p;return *this;}
        MyIterator operator++(int) {MyIterator tmp(*this); operator++(); return tmp;}
        bool operator==(const MyIterator& rhs) const {return p==rhs.p;}
        bool operator!=(const MyIterator& rhs) const {return p !=rhs.p;}
        Element** operator*() {return p;}
    };
    
};

#include "UniqueArrayImp.h"


#endif //MTMPARKINGLOT_UNIQUEARRAY_H
