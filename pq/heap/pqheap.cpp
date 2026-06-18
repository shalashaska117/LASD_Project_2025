#include <stdexcept>

namespace lasd {

/* ************************************************************************** */

// Constructor from TraversableContainer
template <typename Data>
PQHeap<Data>::PQHeap(const TraversableContainer<Data>& cont) : HeapVec<Data>(cont) {
  this->capacity = cont.Size();
}

// Constructor from MappableContainer
template <typename Data>
PQHeap<Data>::PQHeap(MappableContainer<Data>&& cont) : HeapVec<Data>(std::move(cont)) {
  this->capacity = cont.Size();
}

/* ************************************************************************** */

// Tip (maximum)
template <typename Data>
const Data& PQHeap<Data>::Tip() const {
  if (size == 0) throw std::length_error("Priority queue is empty");
  return (*this)[0];
}

/* ************************************************************************** */

// RemoveTip (removes the maximum)
template <typename Data>
void PQHeap<Data>::RemoveTip() {
  if (size == 0) throw std::length_error("Priority queue is empty");
  std::swap((*this)[0], (*this)[size - 1]);
  --size;
  HeapifyDown(0);
}

/* ************************************************************************** */

// TipNRemove (returns and removes the maximum)
template <typename Data>
Data PQHeap<Data>::TipNRemove() {
  if (size == 0) throw std::length_error("Priority queue is empty");
  Data tmp = std::move((*this)[0]);
  std::swap((*this)[0], (*this)[size - 1]);
  --size;
  HeapifyDown(0);
  return tmp;
}

/* ************************************************************************** */

// Insert (copy)
template <typename Data>
void PQHeap<Data>::Insert(const Data& dat) {
  if (size == capacity)
    Resize((capacity == 0) ? 1 : capacity * 2);

  Vector<Data>::elements_[size] = dat; 
  ++size;
  HeapifyUp(size - 1);
}

/* ************************************************************************** */

// Insert (move)
template <typename Data>
void PQHeap<Data>::Insert(Data&& dat) {
  if (size == capacity)
    Resize((capacity == 0) ? 1 : capacity * 2);

  Vector<Data>::elements_[size] = std::move(dat); 
  ++size;
  HeapifyUp(size - 1);
}

/* ************************************************************************** */

// Change (copy)
template <typename Data>
void PQHeap<Data>::Change(const unsigned long i, const Data& dat) {
  if (i >= size) throw std::out_of_range("Access out of range");
  Data old = (*this)[i];
  (*this)[i] = dat;
  (dat > old) ? HeapifyUp(i) : HeapifyDown(i);
}

/* ************************************************************************** */

// Change (move)
template <typename Data>
void PQHeap<Data>::Change(const unsigned long i, Data&& dat) {
  if (i >= size) throw std::out_of_range("Access out of range");
  Data old = (*this)[i];
  (*this)[i] = std::move(dat);
  ((*this)[i] > old) ? HeapifyUp(i) : HeapifyDown(i);
}

/* ************************************************************************** */

// Copy constructor
template <typename Data>
PQHeap<Data>::PQHeap(const PQHeap<Data>& other)
  : HeapVec<Data>(other) {
  capacity = other.capacity;
}

/* ************************************************************************** */

// Move constructor
template <typename Data>
PQHeap<Data>::PQHeap(PQHeap<Data>&& other) noexcept
  : HeapVec<Data>(std::move(other)) {
  capacity = other.capacity;
  other.capacity = 0;
}

/* ************************************************************************** */

// Copy assignment
template <typename Data>
PQHeap<Data>& PQHeap<Data>::operator=(const PQHeap<Data>& other) {
  if (this != &other) {
    HeapVec<Data>::operator=(other);
    capacity = other.capacity;
  }
  return *this;
}

/* ************************************************************************** */

// Move assignment
template <typename Data>
PQHeap<Data>& PQHeap<Data>::operator=(PQHeap<Data>&& other) noexcept {
  if (this != &other) {
    HeapVec<Data>::operator=(std::move(other));
    capacity = other.capacity;
    other.capacity = 0;
  }
  return *this;
}

/* ************************************************************************** */

// Clear
template <typename Data>
void PQHeap<Data>::Clear() {
  HeapVec<Data>::Clear();
  capacity = 0;
}

/* ************************************************************************** */

// Resize 
template <typename Data>
void PQHeap<Data>::Resize(const unsigned long newcap) {
  if (newcap == 0) {
    Vector<Data>::Clear();
    capacity = 0;
  } else {
    Data* temp = new Data[newcap];
    unsigned long min = (newcap < size) ? newcap : size;

    for (unsigned long i = 0; i < min; ++i)
      temp[i] = std::move((*this)[i]);

    delete[] Vector<Data>::elements_;
    Vector<Data>::elements_ = temp;

    if (size > newcap)
      size = newcap;

    Vector<Data>::size = size;

    capacity = newcap;
  }
}

} // namespace lasd
