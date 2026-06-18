# LASD 24/25 – Data Structures and Tests

This project was developed for the **Algorithms and Data Structures Laboratory**
(*Laboratorio di Algoritmi e Strutture Dati*) course at the University of Naples
Federico II, academic year **2024/2025**.

It is a C++ library that implements a hierarchy of generic (templated) containers
from the ground up — no STL containers are used for the data structures
themselves. Every structure is built on top of a common set of abstract
interfaces (traversable, mappable, linear, etc.), so the concrete containers
share a consistent API and can be used polymorphically.

## What's in here

The codebase is organized around an abstract container hierarchy and its concrete
implementations:

- **`container/`** – abstract base interfaces: `Container`, `TestableContainer`,
  `TraversableContainer`, `MappableContainer`, `DictionaryContainer`,
  `LinearContainer`, and the resizable/sortable/clearable refinements. These
  define the operations (traverse, map, fold, sort, …) that the concrete
  structures implement.
- **`vector/`**, **`list/`**, **`set/`**, **`heap/`**, **`pq/`** – the concrete
  data structures (see below).
- **`zlasdtest/`** – the official test suite provided by the course.
- **`zmytest/`** – additional personal tests written to cover edge cases beyond
  the official suite.

## Implemented structures

The project is split into two parts, each adding concrete data structures on top
of the shared container interfaces.

### Part 1 – Linear and set structures
- **`Vector`** – dynamic array with O(1) indexed access.
- **`List`** – singly linked list.
- **`SetVec`** – ordered set backed by a (circular) vector.
- **`SetLst`** – ordered set backed by a linked list.

### Part 2 – Heaps and Priority Queues
- **`HeapVec`** – a binary **max-heap** stored in a vector. Provides
  `Heapify`, `IsHeap`, and in-place **heap sort** (`Sort`, ascending order).
- **`PQHeap`** – a **priority queue** built on top of `HeapVec`, supporting
  `Tip` / `RemoveTip` / `TipNRemove`, `Insert`, and `Change`.

## Build

A `makefile` is provided. From the project root:

```sh
make
```

This produces an executable named `main`. The build uses `g++` with
`-std=c++20` and AddressSanitizer enabled (`-fsanitize=address`).

To clean build artifacts:

```sh
make clean
```

> **Note:** the project relies on the POSIX `uint`/`ulong` typedefs, which are
> available out of the box on Linux/glibc (the course's reference environment).
> On other toolchains they may need to be defined.

## Running the tests

Launch the executable and choose from the interactive menu:

```sh
./main
```

```
1. Official tests (zlasdtest)
2. Personal tests – Part 1 [Vector, List, Set, SetVec, SetLst]
3. Personal tests – Part 2 [Heap, HeapVec, PQ, PQHeap]
4. Personal tests – All
0. Exit
```

## Project layout

```
container/   abstract container interfaces
vector/      Vector, SortableVector
list/        List
set/         Set interface + SetVec / SetLst
heap/        Heap interface + HeapVec
pq/          PQ interface + PQHeap
zlasdtest/   official course tests
zmytest/     personal tests
main.cpp     interactive test runner
makefile     build configuration
```

---

> Project developed for academic purposes – University of Naples Federico II
