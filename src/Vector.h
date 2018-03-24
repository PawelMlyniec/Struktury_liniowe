#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#define startSize 10
namespace aisdi
{

template <typename Type>
class Vector
{
public:

  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
    size_type capacity;
    pointer head;
    size_type elemNr;

public:
 size_type getCapacity() const
 {
    return capacity;
 }

 void biggerTable( )
    {
       auto newHead = new value_type[2*(capacity)];
       for(size_type i=0;i<elemNr;++i)
            newHead[i] = head[i];
        std::swap(head,newHead);
        delete [] newHead;
    capacity=(2*(capacity));

    }

  Vector():capacity(startSize),elemNr(0)
  {

  head = new value_type[startSize];

  }

  Vector(std::initializer_list<Type> l): capacity(l.size()+startSize),elemNr(0)
  {
    head = new value_type[l.size()];
    auto ptr=l.begin();

        while(ptr!=l.end())
        {
            append(*ptr);
            ++ptr;
        }

  }

  Vector(const Vector& other):capacity(other.getCapacity() + 10),elemNr(0)
  {
    head = new value_type[capacity];

    for(auto ptr=other.begin();ptr!=other.end();++ptr)
    {
        append(*ptr);
    }
  }

  Vector(Vector&& other)
  {
    head = other.head;
    capacity=other.capacity;
    elemNr = other.elemNr;
    other.capacity=0;
    other.elemNr=0;
    other.head=new value_type[other.capacity];


  }

  ~Vector()
  {
    delete [] head;
  }

  Vector& operator=(const Vector& other)
  {
    if(head==other.head)
        return *this;
   elemNr=0;
   delete [] head;
    capacity= (other.getCapacity()+startSize);
    head = new value_type[capacity];
    for(auto i = other.begin(); i!=other.end();++i)
    {
        append(*i);
    }

    return *this;
  }

  Vector& operator=(Vector&& other)
  {

    if(head==other.head)
        return *this;
    elemNr=other.getSize();
    capacity = other.getCapacity();
    if(head)
        delete [] head;
    head = other.head;

    other.head = nullptr;
    other.elemNr = 0;
    other.capacity = 0;

   return *this;

  }

  bool isEmpty() const
  {
    return(elemNr==0);

  }

  size_type getSize() const
  {
    return elemNr;
  }

  void append(const Type& item)
  {
    if((capacity)==(elemNr+1))
        biggerTable();
     elemNr+=1;
    (*(end()-1))=item;



  }

  void prepend(const Type& item)
  {
    if(capacity==elemNr+1)
        biggerTable();
    elemNr+=1;
    auto i=(end()-1);
    while(i!=(begin()))
    {
        *(i)=*(i-1);
        i=i-1;
    }
    *(i)=item;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(capacity==(elemNr+1))
        biggerTable();
    elemNr+=1;
    auto i=(end()-1);
    while(i!=insertPosition)
    {
    *(i)=*(i-1);
    --i;
    }
    *(i)=item;
  }

  Type popFirst()
  {
   if(isEmpty())
        throw std::logic_error("pop first");
    Type returned=(*begin());
    if(getSize()!=1)
        for(auto i=(begin());i!=(end()-1);++i)
            *i=*(i+1);
    elemNr-=1;
    return returned;

  }

  Type popLast()
  {
  if(isEmpty())
        throw std::logic_error("pop last");
    Type returned = *(end()-1);
    elemNr-=1;
    return returned;

  }

  void erase(const const_iterator& possition)
  {
    if(isEmpty()||possition==end())
        throw std::out_of_range("erase error");

    for(auto it = iterator(possition);it!=(end()-1);++it)
        *(it)=*(it+1);

    elemNr-=1;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(firstIncluded==lastExcluded)
        return;
    size_type deletedNr = 0;
    bool countingFlg = true;
    auto destinationIt = iterator(firstIncluded);
    auto sourceIt = iterator(lastExcluded);
    while(sourceIt!=end()||countingFlg)
    {

       if(sourceIt!=end())
       {
            *(destinationIt)= *(sourceIt);
            ++sourceIt;
       }
       if(destinationIt==lastExcluded)
            countingFlg=false;
       if(countingFlg)
            ++deletedNr;
        if(destinationIt!=end())
            ++destinationIt;


    }
    elemNr-=deletedNr;

  }

  iterator begin()
  {
    return iterator(const_iterator(head,head,elemNr));

  }

  iterator end()
  {

   return iterator(const_iterator(head+elemNr,head,elemNr));
  }

  const_iterator cbegin() const
  {
    return const_iterator(head,head,elemNr);

  }

  const_iterator cend() const
  {

    return const_iterator((head+elemNr),head,elemNr);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;
private:
    pointer current;
    pointer head;
    size_type elemNr;
public:
  ConstIterator(pointer Current,pointer Head,size_type ElemNr):current(Current),head(Head),elemNr(ElemNr){}


    pointer getCurrent() const
    {
    return current;
    }

  explicit ConstIterator()
  {}


  reference operator*() const
  {

    if(getCurrent()==(head+elemNr))
        throw std::out_of_range("operator* not working");
    return *current;
  }

  ConstIterator& operator++()
  {

    if(getCurrent()==(head+elemNr))
        throw std::out_of_range("operator++ not working");
    current=(current+1);
    return *this;
  }


  ConstIterator operator++(int)
  {
  if(getCurrent()==(head+elemNr))
        throw std::out_of_range("operator++ not working");

    auto result = *this;
    ++(*this);
    return result;
  }

  ConstIterator& operator--()
  {
    if(current==head)
        throw std::out_of_range("operator --");
    current=(current-1);
    return *this;
  }

  ConstIterator operator--(int)
  {
    const_iterator result = *this;
    --(*this);
    return result;
  }

  ConstIterator operator+(difference_type d) const
  {
    if((head+elemNr)-current<d)
        throw std::range_error("operator +");
        auto it=*this;
        it.current=(it.current+d);
    return it;
  }

  ConstIterator operator-(difference_type d) const
  {
    if(current-head<(d-1))
        throw std::range_error("operator -");
    auto it=*this;
        it.current=(it.current-d);
    return it;
  }

  bool operator==(const ConstIterator& other) const
  {
    if(current==other.getCurrent())
        return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    if(current!=other.getCurrent())
        return true;
    return false;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_VECTOR_H
