#ifndef __CIRCULAR_BUFFER__
#define __CIRCULAR_BUFFER__
#include <iterator>

template <typename T, size_t capacity>
class CircularBuffer {

public:

    template<typename T2, size_t capacity2>
    class CircularBufferIterator_base{

    public:
        CircularBufferIterator_base(CircularBuffer& cbuf,size_t idx) : _cbuf{cbuf},_idx{idx}  {    }
        T& operator*() { 
            return _cbuf.items[_idx];
        }    
    
    protected:
    
        CircularBuffer& _cbuf;
        size_t _idx;
    };

    typedef CircularBufferIterator_base<T,capacity>  CircularBufferIterator;
    typedef CircularBufferIterator_base<const T,capacity>  const_CircularBufferIterator;

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
    
        
};




#endif // __CIRCULAR_BUFFER__
