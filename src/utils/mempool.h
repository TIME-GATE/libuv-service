/* 这部分内存池实现来自于https://github.com/cacay/MemoryPool/blob/master/C-11/MemoryPool.h 
 * 此处是拿来分析
 */

#ifndef __SRC_UTILS_MEMPOLL_H__
#define __SRC_UTILS_MEMPOLL_H__

#include <climits>
#include <cstddef>

template <typename T, size_t BlockSize = 4096>
class MemoryPool {
  public:
    /* Member types */
    typedef T value_type;

    typedef T *pointer;

    typedef T &reference;

    typedef const T *const_pointer;

    typedef const T &const_reference;

    typedef size_t size_type;

    typedef ptrdiff_t difference_type;

    typedef std::false_type propagate_on_container_copy_assignment;

    typedef std::true_type propagate_on_container_move_assignment;

    typedef std::true_type propagate_on_container_swap;

    template <typename U> struct rebind {
      typedef MemoryPool<U> other;
    };

  public:
    MemoryPool() noexcept;

    MemoryPool(const MemoryPool &memoryPool) noexcept;

    MemoryPool(MemoryPool &&memoryPool) noexcept;

    template <class U> MemoryPool(const MemoryPool<U> &memoryPool) noexcept;

    ~MemoryPool() noexcept;

    MemoryPool &operator=(const MemoryPool &memoryPool) = delete;

    MemoryPool &operator=(MemoryPool &&memoryPool) noexcept;

    pointer address(reference x) const noexcept;

    const_pointer address(const_reference x) const noexcept;

    pointer allocate(size_type n = 1, const_pointer hint = 0);

    void deallocate(pointer p, size_type n = 1);

    size_type max_size() const noexcept;

    template <class U, class... Args>
    
    void construct(U *p, Args &&... args);

    template <class U> void destroy(U *p);

    template <class... Args>

    pointer newElement(Args &&... args);

    void deleteElement(pointer p);

  private:
    union Slot_ {
      value_type element;
      Slot_ * next;
    };

    typedef char * data_pointer_;
    typedef Slot_ slot_type_;
    typedef Slot_ * slot_pointer_;

    slot_pointer_ currentBlock_;
    slot_pointer_ currentSlot_;
    slot_pointer_ lastSlot_;
    slot_pointer_ freeSlots_;

    size_type padPointer(data_pointer_ p, size_type align) const noexcept;

    void allocateBlock();

    static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small.");
  };

#include "mempool.cc"

#endif