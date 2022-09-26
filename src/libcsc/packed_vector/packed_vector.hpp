#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace libcsc {

template <typename T, unsigned BITS>
class PackedVector {
 private:
  std::size_t m_size{0};
  std::size_t m_capacity{0};
  std::uint8_t* m_ptr{nullptr};

 public:
  class PackedIterator {
   private:
    PackedVector* m_vec;
    std::size_t m_ind;

   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    ~PackedIterator() = default;

    explicit PackedIterator(PackedVector* vec = nullptr, std::size_t ind = 0)
        : m_vec(vec), m_ind(ind) {}

    PackedIterator(const PackedIterator& rhs)
        : m_vec(rhs.m_vec), m_ind(rhs.m_ind) {}

    PackedIterator(PackedIterator&& rhs) noexcept
        : m_vec(rhs.m_vec), m_ind(rhs.m_ind) {
      rhs.m_vec = nullptr;
      rhs.m_ind = 0;
    }

    PackedIterator& operator=(const PackedIterator& rhs) {
      using std::swap;

      if (&rhs != this) {
        PackedIterator temp(rhs);

        swap(*this, temp);
      }

      return *this;
    }

    PackedIterator& operator=(PackedIterator&& rhs) noexcept {
      m_vec = rhs.m_vec;
      m_ind = rhs.m_ind;

      rhs.m_vec = nullptr;
      rhs.m_ind = 0;

      return *this;
    }

    T current() const { return m_vec->at(m_ind); }

    bool has_next() const { return m_ind + 1 < m_vec->size(); }

    T operator*() const { return current(); }

    PackedIterator& operator++() {
      m_ind++;

      return *this;
    }

    const PackedIterator operator++(int) {
      auto old = *this;

      ++(*this);

      return old;
    }

    friend bool operator==(
        const PackedIterator& lhs,
        const PackedIterator& rhs) {
      return (lhs.m_vec == rhs.m_vec) && (lhs.m_ind == rhs.m_ind);
    }

    friend bool operator!=(
        const PackedIterator& lhs,
        const PackedIterator& rhs) {
      return !(lhs == rhs);
    }

    friend void swap(PackedIterator& lhs, PackedIterator& rhs) {
      using std::swap;

      swap(lhs.m_vec, rhs.m_vec);
      swap(lhs.m_ind, rhs.m_ind);
    }
  };

  /* Default constructor */
  PackedVector() = default;

  explicit PackedVector(std::size_t size, T value = 0)
      : m_size(size),
        m_capacity(size),
        m_ptr(new std::uint8_t[reserved_bytes()]) {
    for (std::size_t i = 0; i < m_size; i++) {
      assign(i, value);
    }
  }

  /* Copy constructor */
  PackedVector(const PackedVector& rhs)
      : m_size(rhs.m_size),
        m_capacity(rhs.m_capacity),
        m_ptr(new std::uint8_t[reserved_bytes()]) {
    std::memcpy(m_ptr, rhs.m_ptr, used_bytes());
  }

  /* Move constructor */
  PackedVector(PackedVector&& rhs) noexcept
      : m_size(rhs.m_size), m_capacity(rhs.m_capacity), m_ptr(rhs.m_ptr) {
    rhs.m_ptr = nullptr;
    rhs.m_size = 0;
    rhs.m_capacity = 0;
  }

  /* Copy assignment */
  PackedVector& operator=(const PackedVector& rhs) {
    using std::swap;

    if (&rhs != this) {
      PackedVector temp(rhs);
      swap(*this, temp);
    }

    return *this;
  }

  /* Move assignment */
  PackedVector& operator=(PackedVector&& rhs) noexcept {
    m_ptr = rhs.m_ptr;

    m_size = rhs.m_size;
    m_capacity = rhs.m_capacity;

    rhs.m_ptr = nullptr;
    rhs.m_size = rhs.m_capacity = 0;

    return *this;
  }

  /* Destructor */
  ~PackedVector() { erase(); }

  /** Element access **/

  /* returns the value stored by index 'ind' */
  T at(std::size_t ind) const {
    if (ind >= size()) {
      throw std::out_of_range("Index out of range");
    }

    T ret = 0;

    std::uint8_t extracted_bit = 0;
    for (std::size_t bit = ind * BITS; bit < BITS * (ind + 1); bit++) {
      extracted_bit = ((*(m_ptr + bit / 8U)) >> ((7U - bit % 8U)) & 1U);

      ret |= extracted_bit << (bit % BITS);
    }

    return ret;
  }

  /* returns the value stored by index 'ind' */
  T operator[](std::size_t ind) const { return at(ind); }

  /* returns the first element (index = 0) */
  T front() const { return at(0); }

  /* returns the  last element (index = size - 1) */
  T back() const { return at(m_size - 1); }

  /* returns raw data pointer */
  const std::uint8_t* data() const { return m_ptr; }

  /* set element's value with index 'ind' to the 'value' */
  void assign(std::size_t ind, T value) {
    if (ind >= size()) {
      throw std::out_of_range("Index out of range");
    }

    std::uint8_t assign_bit = 0;
    for (std::size_t bit = ind * bits(); bit < bits() * (ind + 1); bit++) {
      assign_bit = (value >> (bit % BITS)) & 0x1;

      if (assign_bit != 0) {
        (*(m_ptr + bit / 8U)) |= (1U << (8U - 0x1U - bit % 8U));
      } else {
        (*(m_ptr + bit / 8U)) &= ~(0x1U << (8U - 0x1U - bit % 8U));
      }
    }
  }

  /** Capacity **/

  /* returns 'true' if no used elements */
  bool empty() const { return m_size == 0; }

  /* returns the number of used elements */
  std::size_t size() const { return m_size; }

  /* returns maximum number of elements that vector can store without
   * reallocation */
  std::size_t capacity() const { return m_capacity; }

  /* returns the number of bits needed to store 1 element */
  std::size_t bits() const { return BITS; }

  /* returns the number of bytes needed to store 'Size' elements */
  std::size_t used_bytes() const { return 1 + m_size * BITS / 8U; }

  /* returns the number of bytes needed to store 'Capacity' elements */
  std::size_t reserved_bytes() const { return 1 + m_capacity * BITS / 8U; }

  /* reduces the capacity to the size */
  void shrink_to_fit() {
    std::size_t used_bytes = m_size * bits() / 8U + 1;

    auto* new_ptr = new uint8_t[used_bytes];

    std::memcpy(new_ptr, m_ptr, used_bytes);

    delete[] m_ptr;
    m_ptr = new_ptr;

    m_capacity = m_size;
  }

  /** Modifiers **/

  /* set all bits of data to 0 */
  void clear() {
    for (std::size_t i = 0; i < used_bytes(); i++) {
      assign(i) = 0;
    }
  }

  /* free allocated memory */
  void erase() {
    delete[] m_ptr;
    m_ptr = nullptr;
    m_size = m_capacity = 0;
  }

  /* inserts element after element stored by index 'ind' */
  void insert(std::size_t ind, T value) {
    m_size++;

    for (std::size_t i = m_size - 1; i > ind + 1; i--) {
      assign(i, at(i - 1));
    }

    assign(ind + 1, value);
  }

  /* changes size, reallocates if needed */
  void resize(std::size_t new_size) {
    if (new_size < m_capacity) {
      m_size = new_size;

      return;
    }

    if (new_size > m_capacity) {
      std::size_t i = m_size;

      auto* new_ptr = new std::uint8_t[1 + new_size * bits() / 8U];

      if (!empty()) {
        std::memcpy(new_ptr, m_ptr, 1 + m_size * bits() / 8U);
        delete[] m_ptr;
      }

      m_ptr = new_ptr;

      m_size = new_size;
      m_capacity = new_size;

      for (; i < new_size; i++) {
        assign(i, 0);
      }
    }
  }

  /* changes capacity, reallocation is needed */
  void reserve(std::size_t new_cap) {
    if (new_cap == 0) {
      erase();
      return;
    }

    auto* new_ptr = new std::uint8_t[1 + new_cap * BITS / 8U];

    if (new_cap < m_size) {
      std::memcpy(new_ptr, m_ptr, 1 + new_cap * BITS / 8U);

      m_size = m_capacity = new_cap;
    } else {
      std::memcpy(new_ptr, m_ptr, used_bytes() + 1);

      m_capacity = new_cap;
    }

    delete[] m_ptr;
    m_ptr = new_ptr;
  }

  /* adds element after the last one */
  void push_back(T value) {
    m_size++;

    /* without reallocation */
    if (m_size < m_capacity) {
      assign(m_size - 1, value);
      return;
    }

    /* with reallocation */
    reserve(m_capacity + 2);
    assign(m_size - 1, value);
  }

  /* removes and returns the last element */
  T pop_back() {
    T ret = at(m_size - 1);

    m_size--;

    return ret;
  }

  /* returns the common array of type T */
  T* unpack() const {
    T* array = new T[size()];

    for (std::size_t i = 0; i < size(); i++) {
      array[i] = at(i);
    }

    return array;
  }

  /** Iterator **/

  /* returns the iterator that points to the first element of the vector */
  PackedIterator begin() { return PackedIterator(this, 0); }

  /* return the iterator that points to the element going ahead of the last
   * element */
  PackedIterator end() { return PackedIterator(this, size()); }

  /* swap memory of two vectors */
  friend void swap(PackedVector& lhs, PackedVector& rhs) noexcept {
    using std::swap;

    swap(lhs.m_size, rhs.m_size);
    swap(lhs.m_capacity, rhs.m_capacity);
    swap(lhs.m_ptr, rhs.m_ptr);
  }
};

}  // namespace libcsc
