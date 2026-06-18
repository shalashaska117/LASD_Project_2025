namespace lasd {

    /* ************************************************************************** */
    /* Constructors */
    
    template <typename Data>
    SetLst<Data>::SetLst(const TraversableContainer<Data>& container) {
      container.Traverse([this](const Data& dat) {
        Insert(dat);
      });
    }
    
    template <typename Data>
    SetLst<Data>::SetLst(MappableContainer<Data>&& container) {
      container.Map([this](Data& dat) {
        Insert(std::move(dat));
      });
    }
    
    template <typename Data>
    SetLst<Data>::SetLst(const SetLst& other) : List<Data>(other) {}
    
    template <typename Data>
    SetLst<Data>::SetLst(SetLst&& other) noexcept : List<Data>(std::move(other)) {}
    
    /* ************************************************************************** */
    /* Assignment operators */
    
    template <typename Data>
    SetLst<Data>& SetLst<Data>::operator=(const SetLst& other) {
      List<Data>::operator=(other);
      return *this;
    }
    
    template <typename Data>
    SetLst<Data>& SetLst<Data>::operator=(SetLst&& other) noexcept {
      List<Data>::operator=(std::move(other));
      return *this;
    }
    
    /* ************************************************************************** */
    /* Comparison operators */
    
    template <typename Data>
    bool SetLst<Data>::operator==(const SetLst& other) const {
      return List<Data>::operator==(other);
    }
    
    template <typename Data>
    bool SetLst<Data>::operator!=(const SetLst& other) const {
      return !(*this == other);
    }
    
    /* ************************************************************************** */
    /* OrderedDictionary functions */
    
    template <typename Data>
    const Data& SetLst<Data>::Min() const {
      if (this->Empty()) throw std::length_error("Set is empty");
      const Data* minElem = &this->Front();
      this->Traverse([&](const Data& dat) {
        if (dat < *minElem) minElem = &dat;
      });
      return *minElem;
    }
    
    template <typename Data>
    Data SetLst<Data>::MinNRemove() {
      const Data& minElem = Min();
      Data ret = minElem;
      Remove(minElem);
      return ret;
    }
    
    template <typename Data>
    void SetLst<Data>::RemoveMin() {
      const Data& minElem = Min();
      Remove(minElem);
    }
    
    template <typename Data>
    const Data& SetLst<Data>::Max() const {
      if (this->Empty()) throw std::length_error("Set is empty");
      const Data* maxElem = &this->Front();
      this->Traverse([&](const Data& dat) {
        if (dat > *maxElem) maxElem = &dat;
      });
      return *maxElem;
    }
    
    template <typename Data>
    Data SetLst<Data>::MaxNRemove() {
      const Data& maxElem = Max();
      Data ret = maxElem;
      Remove(maxElem);
      return ret;
    }
    
    template <typename Data>
    void SetLst<Data>::RemoveMax() {
      const Data& maxElem = Max();
      Remove(maxElem);
    }
    
    template <typename Data>
    const Data& SetLst<Data>::Predecessor(const Data& dat) const {
      if (this->Empty()) throw std::length_error("Set is empty");
      const Data* pred = nullptr;
      this->PreOrderTraverse([&](const Data& curr) {
        if (curr < dat && (pred == nullptr || curr > *pred)) {
          pred = &curr;
        }
      });
      if (pred == nullptr) throw std::length_error("No predecessor found");
      return *pred;
    }
    
    template <typename Data>
    Data SetLst<Data>::PredecessorNRemove(const Data& dat) {
      const Data& pred = Predecessor(dat);
      Data ret = pred;
      Remove(pred);
      return ret;
    }
    
    template <typename Data>
    void SetLst<Data>::RemovePredecessor(const Data& dat) {
      const Data& pred = Predecessor(dat);
      Remove(pred);
    }
    
    template <typename Data>
    const Data& SetLst<Data>::Successor(const Data& dat) const {
      if (this->Empty()) throw std::length_error("Set is empty");
      const Data* succ = nullptr;
      this->PreOrderTraverse([&](const Data& curr) {
        if (curr > dat && (succ == nullptr || curr < *succ)) {
          succ = &curr;
        }
      });
      if (succ == nullptr) throw std::length_error("No successor found");
      return *succ;
    }
    
    template <typename Data>
    Data SetLst<Data>::SuccessorNRemove(const Data& dat) {
      const Data& succ = Successor(dat);
      Data ret = succ;
      Remove(succ);
      return ret;
    }
    
    template <typename Data>
    void SetLst<Data>::RemoveSuccessor(const Data& dat) {
      const Data& succ = Successor(dat);
      Remove(succ);
    }
    
    /* ************************************************************************** */
    /* Dictionary functions */
    
    template <typename Data>
    bool SetLst<Data>::Insert(const Data& dat) {
      typename List<Data>::Node* prev = nullptr;
      typename List<Data>::Node* curr = this->head;
    
      while (curr != nullptr && curr->element < dat) {
        prev = curr;
        curr = curr->next;
      }
    
      if (curr != nullptr && curr->element == dat) return false;
    
      typename List<Data>::Node* newNode = new typename List<Data>::Node(dat);

      newNode->next = curr;
    
      if (prev == nullptr) {
        this->head = newNode;
        if (this->size == 0) this->tail = newNode;
      } else {
        prev->next = newNode;
        if (curr == nullptr) this->tail = newNode;
      }
    
      ++this->size;
      return true;
    }    
    
    template <typename Data>
    bool SetLst<Data>::Insert(Data&& dat) {
      typename List<Data>::Node* prev = nullptr;
      typename List<Data>::Node* curr = this->head;
    
      while (curr != nullptr && curr->element < dat) {
        prev = curr;
        curr = curr->next;
      }
    
      if (curr != nullptr && curr->element == dat) return false;
    
      typename List<Data>::Node* newNode = new typename List<Data>::Node(std::move(dat));

      newNode->next = curr;
    
      if (prev == nullptr) {
        this->head = newNode;
        if (this->size == 0) this->tail = newNode;
      } else {
        prev->next = newNode;
        if (curr == nullptr) this->tail = newNode;
      }
    
      ++this->size;
      return true;
    }    
    
    template <typename Data>
    bool SetLst<Data>::Remove(const Data& dat) {
      typename List<Data>::Node* prev = nullptr;
      typename List<Data>::Node* curr = this->head;
    
      while (curr != nullptr && curr->element < dat) {
        prev = curr;
        curr = curr->next;
      }
    
      if (curr != nullptr && curr->element == dat) {
        if (prev == nullptr) {
          this->head = curr->next;
          if (this->tail == curr) this->tail = nullptr;
        } else {
          prev->next = curr->next;
          if (this->tail == curr) this->tail = prev;
        }
    
        delete curr;
        --this->size;
        return true;
      }
    
      return false;
    }    
    
    /* ************************************************************************** */
    /* LinearContainer functions */
    
    template <typename Data>
    const Data& SetLst<Data>::operator[](unsigned long index) const {
      return List<Data>::operator[](index);
    }
    
    /* ************************************************************************** */
    /* TestableContainer function */
    
    template <typename Data>
    bool SetLst<Data>::Exists(const Data& dat) const noexcept {
      typename List<Data>::Node* curr = this->head;
    
      // Stop once we go past the value being searched for
      while (curr != nullptr && curr->element < dat) {
        curr = curr->next;
      }
    
      return (curr != nullptr && curr->element == dat);
    }
    
    /* ************************************************************************** */
    /* ClearableContainer function */
    
    template <typename Data>
    void SetLst<Data>::Clear() {
      List<Data>::Clear();
    }
    
    /* ************************************************************************** */
    /* ResizableContainer function */

    template <typename Data>
    void SetLst<Data>::Resize(unsigned long newSize) {
      while (this->size > newSize) {
        this->RemoveFromBack();
      }
    }

    /* ************************************************************************** */

    // Auxiliary functions
  }
    
