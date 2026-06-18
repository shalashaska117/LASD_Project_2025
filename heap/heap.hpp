#ifndef HEAP_HPP
#define HEAP_HPP

/* ************************************************************************** */

#include "../container/linear.hpp"
#include "../container/container.hpp" // For ClearableContainer

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

template <typename Data>
class Heap : public virtual SortableLinearContainer<Data>,
             public virtual ClearableContainer {

private:

protected:

public:

  // Destructor
  virtual ~Heap() = default;

  /* ************************************************************************ */

  // Copy assignment
  Heap<Data>& operator=(const Heap<Data>&) = delete; // Copy assignment of abstract types is not possible.

  // Move assignment
  Heap<Data>& operator=(Heap<Data>&&) noexcept = delete; // Move assignment of abstract types is not possible.

  /* ************************************************************************ */

  // Specific member functions

  virtual bool IsHeap() const noexcept = 0;

  virtual void Heapify() noexcept = 0;

};

/* ************************************************************************** */

}

#endif
