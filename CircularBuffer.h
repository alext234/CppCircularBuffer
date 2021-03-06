#ifndef __CIRCULAR_BUFFER__
#define __CIRCULAR_BUFFER__
#include <iterator>
template<typename T, size_t capacity>
class CircularBuffer;


template<typename T, size_t capacity>
class CircularBufferIterator_base: public std::iterator<std::random_access_iterator_tag, T>{

public:

    CircularBufferIterator_base(CircularBuffer<T,capacity>& cbuf,size_t idx) : _cbuf{cbuf},_idx{idx}  {    }
    T& operator*() { 
        return _cbuf.items[_idx];
    }    
    CircularBufferIterator_base& operator++()  {// prefix  
        _idx=(_idx+1) % capacity; return *this;
    }
    CircularBufferIterator_base operator++(int)  {// postfix
        CircularBufferIterator_base aCopy(*this);

        _idx=(_idx+1) % capacity; 
        return aCopy;
    }
    CircularBufferIterator_base& operator--()  {// prefix  
        if (_idx==0) _idx=capacity-1; else  _idx--;
        return *this;
    }
    CircularBufferIterator_base operator--(int)  {// postfix
        CircularBufferIterator_base aCopy(*this);
        if (_idx==0) _idx=capacity-1; else  _idx--;
        return aCopy;
    }

    template<typename T2, size_t capacity2>
    friend bool operator==(const CircularBufferIterator_base<T2,capacity2>& i1, const CircularBufferIterator_base<T2,capacity2>& i2) ;
    template<typename T2, size_t capacity2>
    friend bool operator!=(const CircularBufferIterator_base<T2,capacity2>& i1, const CircularBufferIterator_base<T2,capacity2>& i2) ;
protected:

    CircularBuffer<T,capacity>& _cbuf;
    size_t _idx;
};


template<typename T, size_t capacity>
class CircularBufferIterator_const: public std::iterator<std::random_access_iterator_tag, T>{

public:

    CircularBufferIterator_const(CircularBuffer<T,capacity>& cbuf,size_t idx) : _cbuf{cbuf},_idx{idx}  {    }
    const T& operator*() { 
        T &retVal =  _cbuf.items[_idx];
        return retVal;
    }    
    CircularBufferIterator_const& operator++()  {// prefix  
        _idx=(_idx+1) % capacity; return *this;
    }
    CircularBufferIterator_const operator++(int)  {// postfix
        CircularBufferIterator_const aCopy(*this);

        _idx=(_idx+1) % capacity; 
        return aCopy;
    }
    CircularBufferIterator_const& operator--()  {// prefix  
        if (_idx==0) _idx=capacity-1; else  _idx--;
        return *this;
    }
    CircularBufferIterator_const operator--(int)  {// postfix
        CircularBufferIterator_const aCopy(*this);
        if (_idx==0) _idx=capacity-1; else  _idx--;
        return aCopy;
    }

    template<typename T2, size_t capacity2>
    friend bool operator==(const CircularBufferIterator_const<T2,capacity2>& i1, const CircularBufferIterator_const<T2,capacity2>& i2) ;
    template<typename T2, size_t capacity2>
    friend bool operator!=(const CircularBufferIterator_const<T2,capacity2>& i1, const CircularBufferIterator_const<T2,capacity2>& i2) ;
protected:

    CircularBuffer<T,capacity>& _cbuf;
    size_t _idx;
};

template <typename T, size_t capacity>
class CircularBuffer {

public:


    typedef CircularBufferIterator_base<T,capacity>  CircularBufferIterator;
    typedef CircularBufferIterator_const<T,capacity>    const_CircularBufferIterator;

    CircularBuffer(): items{new T[capacity]} {}
    ~CircularBuffer() { delete[] items;}

    size_t max_size() const {        return capacity; }
    size_t size() const {        return _size;    }
    bool empty() const {        return _size==0;    }
    
    void push_back (T&& v) {
        items[_backIdx] = std::forward<T>(v);
        push_aux_ops();
    }

    void push_back (const T& v) {
        items[_backIdx] = v;
        push_aux_ops();

    }
    
    void push_back_reuse (void) {
        push_aux_ops();
    }

    T& front (void) {
        return items[_frontIdx];        
    }
    void pop_front(void) {
        if (_size==0) return;
        _frontIdx= (_frontIdx+1)%capacity;
        --_size;
    }
   
    CircularBufferIterator begin()  {    return CircularBufferIterator{*this,_frontIdx}; }
    CircularBufferIterator end()  {    return CircularBufferIterator{*this,_backIdx}; }
    
    const_CircularBufferIterator cbegin()  {    return const_CircularBufferIterator{*this,_frontIdx}; }
    const_CircularBufferIterator cend()  {    return const_CircularBufferIterator{*this,_backIdx}; }
private:
    inline void push_aux_ops (void) {
        if (_size==capacity) {
            _frontIdx= (_frontIdx+1)%capacity;
        } else  ++_size;

        _backIdx = (_backIdx+1) % capacity;
    }

    T *items;
    size_t _size=0;
    size_t _backIdx=0;
    size_t _frontIdx=0;
    
    friend class CircularBufferIterator_base<T,capacity>;
    friend class CircularBufferIterator_const<T,capacity>;
   
        
};


template <typename T2, size_t capacity2>
bool operator==(const CircularBufferIterator_base<T2,capacity2>& i1, const CircularBufferIterator_base<T2,capacity2>& i2) {
    return i1._idx == i2._idx;
}


template <typename T2, size_t capacity2>
bool operator!=(const CircularBufferIterator_base<T2,capacity2>& i1, const CircularBufferIterator_base<T2,capacity2>& i2) {
    return i1._idx != i2._idx;
}

template <typename T2, size_t capacity2>
bool operator==(const CircularBufferIterator_const<T2,capacity2>& i1, const CircularBufferIterator_const<T2,capacity2>& i2) {
    return i1._idx == i2._idx;
}


template <typename T2, size_t capacity2>
bool operator!=(const CircularBufferIterator_const<T2,capacity2>& i1, const CircularBufferIterator_const<T2,capacity2>& i2) {
    return i1._idx != i2._idx;
}
#endif // __CIRCULAR_BUFFER__
