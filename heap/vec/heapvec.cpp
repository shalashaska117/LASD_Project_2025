namespace lasd {

    /* ************************************************************************** */
    
    // Costruttore da TraversableContainer
    template <typename Data>
    HeapVec<Data>::HeapVec(const TraversableContainer<Data>& cont) {
      cont.Traverse([this](const Data& val) {
        this->Resize(this->size + 1);
        (*this)[this->size - 1] = val;
      });
      HeapVec<Data>::Heapify();
    }
    
    /* ************************************************************************** */
    
    // Costruttore da MappableContainer (move)
    template <typename Data>
    HeapVec<Data>::HeapVec(MappableContainer<Data>&& cont) {
      cont.Map([this](Data& val) {
        this->Resize(this->size + 1);
        (*this)[this->size - 1] = std::move(val);
      });
      HeapVec<Data>::Heapify();
    }
    
    /* ************************************************************************** */
    
    // Copy constructor
    template <typename Data>
    HeapVec<Data>::HeapVec(const HeapVec<Data>& other) : SortableVector<Data>(other) {
      // Nothing else needed: Heap is abstract, no data to copy
    }
    
    /* ************************************************************************** */
    
    // Move constructor
    template <typename Data>
    HeapVec<Data>::HeapVec(HeapVec<Data>&& other) noexcept : SortableVector<Data>(std::move(other)) {
      // Nothing else needed
    }
    
    /* ************************************************************************** */
    
    // Copy assignment
    template <typename Data>
    HeapVec<Data>& HeapVec<Data>::operator=(const HeapVec<Data>& other) {
      SortableVector<Data>::operator=(other);
      return *this;
    }
    
    /* ************************************************************************** */
    
    // Move assignment
    template <typename Data>
    HeapVec<Data>& HeapVec<Data>::operator=(HeapVec<Data>&& other) noexcept {
      SortableVector<Data>::operator=(std::move(other));
      return *this;
    }
    
    /* ************************************************************************** */
    
    // Comparison operators
    template <typename Data>
    bool HeapVec<Data>::operator==(const HeapVec<Data>& other) const noexcept {
      return SortableVector<Data>::operator==(other);
    }
    
    template <typename Data>
    bool HeapVec<Data>::operator!=(const HeapVec<Data>& other) const noexcept {
      return SortableVector<Data>::operator!=(other);
    }
    
    /* ************************************************************************** */
    
    // Controlla se il contenuto è un heap valido
    template <typename Data>
    bool HeapVec<Data>::IsHeap() const noexcept {
      for (unsigned long i = 0; i < size / 2; ++i) {
        unsigned long left = 2 * i + 1;
        unsigned long right = 2 * i + 2;
    
        if (left < size && (*this)[i] < (*this)[left])
          return false;
        if (right < size && (*this)[i] < (*this)[right])
          return false;
      }
      return true;
    }
    
    /* ************************************************************************** */
    
    // Trasforma il contenuto in un heap (bottom-up heapify)
    template <typename Data>
    void HeapVec<Data>::Heapify() noexcept {
      if (size <= 1) return;
      for (long i = (size / 2) - 1; i >= 0; --i)
        HeapifyDown(i);
    }
    
    /* ************************************************************************** */
    
    // Heapify down: sistema l'heap dal nodo i in giù

    template <typename Data>
    void HeapVec<Data>::HeapifyDown(unsigned long i) noexcept {
      while (true) {
        unsigned long largest = i;
        unsigned long left = 2 * i + 1;
        unsigned long right = 2 * i + 2;
    
        if (left < size && (*this)[left] > (*this)[largest])
          largest = left;
        if (right < size && (*this)[right] > (*this)[largest])
          largest = right;
    
        if (largest != i) {
          std::swap((*this)[i], (*this)[largest]);
          i = largest;
        } else {
          break;
        }
      }
    }

    // HeapifyUp (per ripristinare l’heap bottom-up)
    template <typename Data>
    void HeapVec<Data>::HeapifyUp(unsigned long i) noexcept {
      while (i > 0) {
        unsigned long parent = (i - 1) / 2;
        if ((*this)[i] > (*this)[parent]) {
          std::swap((*this)[i], (*this)[parent]);
          i = parent;
        } else {
          break;
        }
      }
    }
    
    /* ************************************************************************** */
    
    // Ordina i dati con heap sort (in-place, max-heap)
    template <typename Data>
    void HeapVec<Data>::Sort() noexcept {
      if (size <= 1) return; // Niente da ordinare (evita underflow di size - 1 con size == 0)

      Heapify(); // Prima costruiamo l'heap

      for (unsigned long i = size - 1; i > 0; --i) {
        std::swap((*this)[0], (*this)[i]);
        unsigned long newSize = i;
        unsigned long root = 0;
    
        while (true) {
          unsigned long largest = root;
          unsigned long left = 2 * root + 1;
          unsigned long right = 2 * root + 2;
    
          if (left < newSize && (*this)[left] > (*this)[largest])
            largest = left;
          if (right < newSize && (*this)[right] > (*this)[largest])
            largest = right;
    
          if (largest != root) {
            std::swap((*this)[root], (*this)[largest]);
            root = largest;
          } else {
            break;
          }
        }
      }
    }
    
    /* ************************************************************************** */
    
    }
    
