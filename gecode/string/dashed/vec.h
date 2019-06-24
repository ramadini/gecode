#ifndef CHECK_UTILS_H__
#define CHECK_UTILS_H__
#include <limits.h>
#include <cstdlib>
#include <cassert>
#include <algorithm>

template <class T>
class vec {
public:
  static int default_sz() { return 10; }

  typedef T* iterator;
  typedef vec<T> vec_t;
  
  vec(void)
    : sz(0), maxsz(default_sz()), data((T*) malloc(sizeof(T)*maxsz))
  {
    assert(data);
  }

  vec(int _maxsz)
    : sz(0), maxsz(_maxsz), data((T*) malloc(sizeof(T)*maxsz))
  {
    assert(data);
    assert(maxsz > 0);
  }

  vec(int _sz, T elt)
    : sz(_sz), maxsz(std::max(2, sz)), data((T*) malloc(sizeof(T)*maxsz)) {
    assert(maxsz > 0);

    for(int ii = 0; ii < sz; ii++)
      new (&(data[ii])) T(elt);
  }

  vec(vec<T>&& o)
    : sz(o.sz), maxsz(o.maxsz), data(o.data) {
    o.sz = 0;
    o.maxsz = 0;
    o.data = nullptr;  
  }

  vec& operator=(vec<T>&& o) {
    if(data)
      free(data); 
    sz = o.sz; o.sz = 0;
    maxsz = o.maxsz; o.maxsz = 0;
    data = o.data; o.data = nullptr;
    return *this;
  }

  template<class V>
  vec(V& elts)
    : sz(elts.size()), maxsz(std::max(2, sz)), data((T*) malloc(sizeof(T)*maxsz))
  {
    assert(maxsz > 0);
    int ii = 0;  
    for(T& e : elts)
      new (&(data[ii++])) T(e);
//      data[ii++] = e;
  }

  T* begin(void) { return data; }
  T* end(void) { return data+sz; }
  const T* begin(void) const { return data; }
  const T* end(void) const { return data+sz; }
  int size(void) const { return sz; }
  int& _size(void) { return sz; }

  T& last(void) { return data[sz-1]; }

  void _push(void) {
    if(sz >= maxsz)
      growTo(sz+1);
    ++sz;
  }

  void push(void) {
    if(sz >= maxsz)
      careful_growTo(sz+1);
      // growTo(sz+1);
    new (&(data[sz++])) T();
  }
  void _push(const T& elt) { data[sz++] = elt; }
  void push(const T& elt) {
    if(sz >= maxsz)
      careful_growTo(sz+1); 
    // _push(elt);
    new (&(data[sz++])) T(elt);
  }
  void push(T&& elt) {
    if(sz >= maxsz)
      careful_growTo(sz+1); 
    new (&(data[sz++])) T(std::move(elt));
  }
  void pop(void) { data[--sz].~T(); }

  T& operator[](int ii) { return data[ii]; }
  const T& operator[](int ii) const { return data[ii]; }

  ~vec(void) {
    clear();
    if(data)
      free(data);
  }
  
  // WARNING: growTo allocates capacity; doesn't initialize
  void growTo(int new_max) {
    if(maxsz >= new_max) return;
    assert(maxsz >= 1);
    while(maxsz < new_max) {
      maxsz *= 2;
    }
    data = (T*) realloc(data, sizeof(T)*maxsz);
    assert(data);
  }

  void careful_growTo(int new_max) {
    if(maxsz >= new_max) return;
    assert(maxsz >= 1);
    while(maxsz < new_max) {
      maxsz *= 2;
    }
    
    T* old = data;
    
    data = (T*) malloc(sizeof(T)*maxsz);
    assert(data);
    
    T* q = data;
    for(T* p = old; p != old + sz; ++p) {
      new (q++) T(std::move(*p));
      (*p).~T();
    }
    free(old);
  }

  void dropTo(int _sz)
  {
    while(_sz < sz)
      pop();
  }
  void _dropTo(int _sz) {
    if(_sz < sz)
      sz = _sz;
  }

//  void clear(void) { sz = 0; }
  void clear(void) { dropTo(0); }
  void _clear(void) { sz = 0; }
protected:
  int sz;
  int maxsz;

  T* data;
};

#endif
