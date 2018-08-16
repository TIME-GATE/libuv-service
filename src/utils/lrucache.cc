#ifndef __SRC_UTILS_LRUCACHE_H__
#define __SRC_UTILS_LRUCACHE_H__

#include <list>
#include "define.h"

namespace Utils {

template <typename Key, typename Value>

  class LRUCache {

    public:

      typedef Key key_type;
      typedef Value value_type;
      typedef size_t size_type;

    public:
      explicit LRUCache(size_type capacity) //explicit
          : m_Size(0),
            m_Capacity(capacity){};

      ~LRUCache() {
        this->clear();
      }

    public:
      value_type find(const key_type &key) {
        HashTableIter rc = m_Table.find(key);

        if (rc == m_Table.end()) {
          return false;
        }

        Entry * entry = rc->second;
        assert(entry != NULL && "HashTable Entry is Invalid");

        return entry->value;
      };

      bool insert(const key_type & key, const value_type & value) {
        Entry * entry = NULL;

        HashTableIter rc = m_Table.find(key);

        if (rc != m_Table.end()) {
          entry = rc->second;
          assert(entry != NULL && "HashTable Entry is Invalid");

          entry->value = value;
          m_List.erase(entry->link);
        } else {
          entry = new Entry(key, value);

          if (entry == NULL) {
            return false;
          }

          ++m_Size;
          m_Table.insert(std::make_pair(key, entry));
        }

        m_List.push_back(entry);
        entry->link = --m_List.end();

        while (m_Size > m_Capacity) {
          entry = m_List.front();
          assert(entry != NULL && "List::front() Error");

          this->remove(entry->key);
        }

        return true;
      }

      void remove(const key_type & key) {
        Entry * entry = NULL;

        HashTableIter rc = m_Table.find(key);

        if(rc != m_Table.end()) {
          --m_Size;
          entry = rc->second;
          m_Table.erase(rc);
        }

        if(entry != NULL) {
          m_List.erase(entry->link);
          delete entry;
        }

      }

      void clear() {
        for(HashTableIter it = m_Table.begin(); it != m_Table.end(); ++it) {
          delete it->second;
        }

        m_Size = 0;
        m_List.clear();
        m_Table.clear();
      }

      inline size_type size() const { return m_Size; }
      inline size_type capacity() const { return m_Capacity; }

    private:
      struct Entry;
      typedef std::list<Entry *> LRUList;
      typedef typename LRUList::iterator LRUListLink;
      typedef UnorderedMap<key_type, Entry *> HashTable;
      typedef typename HashTable::iterator HashTableIter;
      typedef typename HashTable::const_iterator HashTableConstIter;

      struct Entry {
        Entry(key_type k, value_type v)
          : key(k),
          value(v) {}
        
        key_type key;
        value_type value;
        LRUListLink link;
      };

    private:
      size_type m_Size;
      size_type m_Capacity;
      LRUList m_List;
      HashTable m_Table;
  };

}

class Demo {
  
public:
  void operator()(int id, int year);
};

#endif

int main() {
  typedef Utils::LRUCache<int, int> Cache;

  Cache cache(3);

  for(int i = 1; i <= 5; i++) {
    cache.insert(i, i * 5);
    printf("add cache is : %d\n", cache.find(i));
  }

  for (int i = 1; i <= 5; i++) {
    cache.remove(i);
    printf("remove cache and cache size is : %lu\n", cache.size());
  }

  return 1;
}