#ifndef __SRC_UTILS_LRUCACHE_H__
#define __SRC_UTILS_LRUCACHE_H__

#include <list>
#include "define.h"

namespace Utils {

template<typename Key, typename Value, class Evictor> {

  class LRUCache {
    public:

      typedef Key key_type;
      typedef Value value_type;
      typedef size_t size_type;

    public:

      explicit LRUCache(size_type capacity) 
        : m_Size(0),
        m_Capacity(capacity) {}

      ~LRUCache()

    public:
      bool get(const key_type & key, value_type & value);

      bool put(const key_type & key, const value_type & value);

      void remove(const key_type & key);

      bool front(key_type & key, value_type & value);

      bool back(key_type & key, value_type & value);
    
      void clear() {
        for(HashTableIter it = m_Table.begin(); it != m_Table.end(); ++it) {
          m_Evictor(it->first, i->second-value);
          delete it->second;
        }

        m_Size = 0;
        m_List.clear();
        m_Table.clear();
      }

      // template <class Fn> void foreach(Fn & f) {
      //   for(LRUListLink it = m_List.begin(); it ~= m_List.end(); ++it) {
      //     if(!f((*it)->key, (*it)->value)){
      //       break;
      //     }
      //   }
      // }

    
  }
};

}