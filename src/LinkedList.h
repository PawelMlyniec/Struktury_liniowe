#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
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
  class Node
  {
    public:
    Node() :next(nullptr), prev(nullptr){}
    Node(const value_type &item) :item(item), next(nullptr), prev(nullptr){}

    value_type item;
    Node* next;
    Node* prev;

  };

  Node* first;
  Node* last;
  size_type nodeNr;

public:

  LinkedList()
  {
    nodeNr=0;
    Node *newNode = new Node();
    first = newNode;
    last = newNode;
  }

  LinkedList(std::initializer_list<Type> l)
  {
    nodeNr=0;
    Node *newNode = new Node();
    first = newNode;
    last = newNode;
    if(l.size()!=0)
        for(auto p=l.begin();p!=l.end();++p)
            append(*p);
  }

  LinkedList(const LinkedList& other)
  {
    nodeNr=0;
    Node *newNode = new Node();
    first = newNode;
    last = newNode;
    for(auto i=other.cbegin();i!=other.cend();i++)
        append(*i);
  }

  LinkedList(LinkedList&& other)
  {


    first = other.first;
    last = other.last;
    nodeNr=other.nodeNr;
    other.nodeNr = 0;
    other.last=nullptr;
    other.first=nullptr;

/*
    Node *newNode=new Node();
    other.first=newNode;
    other.last=newNode;*/
  }

  ~LinkedList()
  {
    Node* i=last;
    while(i!=first)
    {
        i=i->prev;
        delete i->next;

    }
    delete i;
    //throw std::runtime_error("TODO");

  }
LinkedList& operator=(const LinkedList& other)
  {
  /*
    LinkedList temp(other);
    std::swap(first,temp.first);
    std::swap(last,temp.last);*/

    if(first == other.first) return *this;
    erase(begin(), end());
    for(auto it = other.begin(); it !=other.end(); ++it)
    {
        append(*it);
    }
/*
    auto i=other.begin();
    auto j=begin();
    while(i!=other.end()&&j!=end())
    {
        if((*j)!=(*i))
        {
            if(j==end())
            {
                append(*i);
                ++i;
            }else if(i==other.end())
            {
                erase(j);
                ++j;
            }else
            {
                insert(j,*i);
                ++j;
                erase(j-1);
                ++i;++j;
            }
        }else
        {
            if(j!=end())
                ++j;
            if(i!=other.end())
                ++i;
        }

    }*/
     return *this;

}





  LinkedList& operator=(LinkedList&& other)
  {
  /*LinkedList temp(other);
    std::swap(first,temp.first);
    std::swap(last,temp.last);
    erase(temp.begin(),temp.end());
  */
  if(first==other.first){
    return *this;
    }
    erase(begin(),end());
    delete last;
    first=other.first;
    last=other.last;
    nodeNr=other.nodeNr;
    other.first=nullptr;
    other.last=nullptr;
    other.nodeNr=0;
    return *this;
  }

  bool isEmpty() const
  {
    if(nodeNr==0)
    return true;
    return false;
  }

  size_type getSize() const
  {
   return nodeNr;
  }

  void append(const Type& item)
  {
    Node* ptr=new Node(item);
    if(nodeNr==0)
    {

        first=ptr;
        last->prev=ptr;
        ptr->next=last;

    }else
    {
        ptr->prev=last->prev;
        last->prev->next=ptr;
        last->prev=ptr;
        ptr->next=last;
    }
    ++nodeNr;
  }

  void prepend(const Type& item)
  {
       Node* ptr=new Node(item);
    if(nodeNr==0)
    {

        first=ptr;
        last->prev=ptr;
        ptr->next=last;

    }else
    {
        ptr->prev=nullptr;
        first->prev=ptr;
        ptr->next=first;
        first=ptr;
    }
    ++nodeNr;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {

    if(isEmpty())
        append(item);
    else if(insertPosition==end())
        append(item);
    else if(insertPosition==begin())
        prepend(item);
    else
    {

        auto inserted= new Node(item);
        insertPosition.getNode()->prev->next=inserted;
        inserted->prev=insertPosition.getNode()->prev;
        insertPosition.getNode()->prev=inserted;
        inserted->next=insertPosition.getNode();
        ++nodeNr;
    }
  }

  Type popFirst()
  {
    if(isEmpty())
        throw std::logic_error("popFirst");
    Node* ptr = first;
    value_type returned=(ptr->item);
    erase(begin());
    /*if(nodeNr==1)
        first=last=nullptr;
    else
    {
        first=first->next;
        first->prev=nullptr;
    }

    delete ptr;
    --nodeNr;*/
    return returned;
  }

  Type popLast()
  {
    if(isEmpty())
        throw std::logic_error("popLast");

    Node* ptr = last->prev;
    value_type returned=(ptr->item);
    erase(--end());
    /*if(nodeNr==1)
    {
        first=last;
        last->prev=nullptr;
    }

    else
    {
        ptr->prev->next=last;
        last->prev=ptr->prev;

    }

    delete ptr;
    --nodeNr;*/
    return returned;
  }


  void erase(const const_iterator& possition)
  {

    if(isEmpty()==true||possition==cend())
        throw std::out_of_range("operator erase");
    if(possition==cbegin())
    {
        if(getSize()==1)
        {
            delete first;
            first=last;
        }else if(getSize()>1)
        {
            auto erased_to_be=first;
            first=first->next;
            first->prev=nullptr;
            delete erased_to_be;

        }
    }else if(possition==(cend()-1))

    {
        auto ptr=last->prev;
        last->prev=last->prev->prev;
        last->prev->next=last;
        delete ptr;

    }
    else
    {
        Node* erased=possition.getNode();
        erased->prev->next=erased->next;
        erased->next->prev=erased->prev;
        delete erased;
    }
    --nodeNr;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {


    auto ptr=firstIncluded;
    while(ptr!=lastExcluded)
    {
        auto erased = ptr;
        ++ptr;
        erase(erased);
    }
   /* for(auto it = firstIncluded;it != lastExcluded; ++it)
    {
        erase(it);
    }*/
  }


  iterator begin()
  {
    return iterator(first);
  }

  iterator end()
  {
    return iterator(last);

  }

  const_iterator cbegin() const
  {
    return const_iterator(first);
  }

  const_iterator cend() const
  {
    return const_iterator(last);
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
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

protected:
    Node* current;

public:
  Node* getNode() const
  {
    return current;
  }

  explicit ConstIterator()
  {}

  ConstIterator(Node* node):current(node){}

  reference operator*() const
  {
    if(current->next==nullptr)
        throw std::out_of_range("operator* not working");
    return (current->item);
  }

  ConstIterator& operator++()
  {
    if(current->next==nullptr)
        throw std::out_of_range("operator++ not working");
    current=current->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
     auto result = *this;
    ++(*this);
    return result;
  }

  ConstIterator& operator--()
  {
     if(current->prev==nullptr)
        throw std::out_of_range("operator-- not working");
    current=current->prev;
    return *this;
  }

  ConstIterator operator--(int)
  {
    auto result = *this;
    --(*this);
    return result;
  }

  ConstIterator operator+(difference_type d) const
  {
    auto it=*this;
    for(difference_type i=0; i<d;++i)
    {
        if(it.current->next==nullptr)
            throw std::range_error("operator + out of range");
        it.current=it.current->next;
    }
    return it;
  }

  ConstIterator operator-(difference_type d) const
  {
  auto it=*this;
    for(difference_type i=0; i<d;++i)
    {
        if(it.current->prev==nullptr)
            break;
        it.current=it.current->prev;
    }

    return it;
  }

  bool operator==(const ConstIterator& other) const
  {
    if(current==other.current)
        return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    if(current!=other.current)
        return true;
    return false;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;



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

#endif // AISDI_LINEAR_LINKEDLIST_H
