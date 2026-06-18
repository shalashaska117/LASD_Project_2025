#ifndef PQHEAP_HPP
#define PQHEAP_HPP

/* ************************************************************************** */

#include "../pq.hpp"
#include "../../heap/vec/heapvec.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class PQHeap : public virtual PQ<Data>,
               public HeapVec<Data> {

private:

  unsigned long capacity = 0; // physical allocated capacity

protected:

  using Container::size;              // logical size (actual elements)
  using Vector<Data>::operator[];     // element access

  using HeapVec<Data>::HeapifyDown;
  using HeapVec<Data>::HeapifyUp;

public:

  // Default constructor
  PQHeap() = default;

  /* ************************************************************************ */

  // Specific constructors
  PQHeap(const TraversableContainer<Data>&); // Builds from a container
  PQHeap(MappableContainer<Data>&&);         // Builds from an rvalue container

  /* ************************************************************************ */

  // Copy constructor
  PQHeap(const PQHeap&);

  // Move constructor
  PQHeap(PQHeap&&) noexcept;

  // Destructor
  ~PQHeap() = default;

  /* ************************************************************************ */

  // Copy assignment
  PQHeap& operator=(const PQHeap&);

  // Move assignment
  PQHeap& operator=(PQHeap&&) noexcept;

  /* ************************************************************************ */

  // Specific member functions (inherited from PQ)

  const Data& Tip() const override;       // Returns the top (error if empty)
  void RemoveTip() override;              // Removes the top (error if empty)
  Data TipNRemove() override;             // Extracts and removes the top (error if empty)

  void Insert(const Data&) override;      // Insertion (copy)
  void Insert(Data&&) override;           // Insertion (move)

  void Change(const unsigned long, const Data&) override; // Change (copy)
  void Change(const unsigned long, Data&&) override;      // Change (move)

  void Clear() override;                  // Clears the whole structure
  void Resize(const unsigned long);       // New Resize that updates only capacity, not size

};

/* ************************************************************************** */

}

#include "pqheap.cpp"

#endif


