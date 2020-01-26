#ifndef UNIQUEARRAY_H_UNIQUEARRAYIMP_H
#define UNIQUEARRAY_H_UNIQUEARRAYIMP_H



template <class Element, class Compare>
    UniqueArray<Element, Compare>::UniqueArray(unsigned int size):
            maxSize(size), elementCounter(0)  {
     arrayOfElements = new Element*[maxSize];
    for(unsigned int i = 0; i<size; i++){
        arrayOfElements[i] = NULL;
    }
}


template <class Element, class Compare>
UniqueArray<Element,Compare>::UniqueArray(const UniqueArray& other) :
         maxSize(other.maxSize), elementCounter(other.elementCounter), arrayOfElements(new Element const*[other.maxSize]) {

    for (int i=0; i<maxSize; i++) {
        if (other.arrayOfElements[i] != EMPTY_SLOT) {
            arrayOfElements[i] = new Element(*other.arrayOfElements[i]);
        }
    }
}


template <class Element, class Compare>
unsigned int UniqueArray<Element,Compare>::insert(const  Element& element) {//const MtmParkingLot::ParkingLot::Motorbike
    unsigned int index=0;

    const Element test = element;


    if (getIndex(element,index)) { /* If the element is in the array.*/
        return index;
    }

    if (elementCounter == maxSize) { /* If the array is full.*/
        throw UniqueArrayIsFullException();
    }

    findCellWithElement(index,EMPTY_SLOT);
    arrayOfElements[index] = new Element(element);
    elementCounter++;

    return index;
}



template <class Element, class Compare>
    bool UniqueArray<Element, Compare>::findCellWithElement( unsigned int& index , const Element* elementPtr)const  {

    for(unsigned int i = 0; i<maxSize; ++i){

        if(elementPtr == NULL){
            if(arrayOfElements[i]==NULL)
            {
                index = i;
                return true;
            }
        }

        else if(arrayOfElements[i]!=NULL) {
            Compare cmp;
            if(cmp(*arrayOfElements[i], *elementPtr))
            {
                index = i;
                return true;
            }
        }
    }

        return false;
}


template <class Element, class Compare>
    bool UniqueArray<Element, Compare>::getIndex(const Element &element, unsigned int &index) const {
        return findCellWithElement(index, &element);
}


template <class Element, class Compare>
UniqueArray<Element, Compare>::~UniqueArray() {
    for(unsigned int i= 0; i<maxSize; ++i){
        if(arrayOfElements[i]!=NULL){
            delete arrayOfElements[i];
        }
    }

    delete[] arrayOfElements;
}

template <class Element, class Compare>
bool UniqueArray<Element, Compare>::remove(const Element &element) {
    unsigned int index=0;
    if (findCellWithElement(index, &element)){
        delete arrayOfElements[index];
        arrayOfElements[index]=NULL;
        --elementCounter;
        return true;
    }
    return false;
}

template <class Element, class Compare>
const Element* UniqueArray<Element,Compare>::operator[]
        (const Element& element) const {
    unsigned int index=0;
    if (!getIndex(element,index)) { /* If the element is not in the array.*/
        return NULL;
    }

    return arrayOfElements[index];

}

template <class Element, class Compare>
unsigned int UniqueArray<Element, Compare>::getSize() const {
    return maxSize;
}
template <class Element, class Compare>
unsigned int UniqueArray<Element,Compare>::getCount() const {
    return elementCounter;
}
template <class Element, class Compare>
UniqueArray<Element,Compare> UniqueArray<Element,Compare>::filter(const Filter& f) const {
    UniqueArray<Element,Compare> newUniqueArray(*this);

    for (int i=0; i<maxSize; i++) {
        if ((arrayOfElements[i] != EMPTY_SLOT) && !f(*arrayOfElements[i])) {
            newUniqueArray.remove(*arrayOfElements[i]);
        }
    }


    return newUniqueArray;
}

/* Gets an index and returns pointer to the element with the given index.*/
template <class Element, class Compare>
const Element* UniqueArray<Element, Compare>::getElement(unsigned int index) {
	if (index >= maxSize) {
		return NULL;
	}
	return arrayOfElements[index];
}




#endif //UNIQUEARRAY_H_UNIQUEARRAYIMP_H
