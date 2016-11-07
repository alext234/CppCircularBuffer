#ifndef __CIRCULAR_BUFFER__
#define __CIRCULAR_BUFFER__

template <typename T, size_t capacity>
class CircularBuffer {

public:
    CircularBuffer(): items{new T[capacity]} {}
    ~CircularBuffer() { delete[] items;}

    size_t max_size() const {        return capacity; }
    size_t size() const {        return _size;    }
    bool empty() const {        return _size==0;    }
    
    void push_back (T&& v) {
        items[_backIdx] = std::forward<T>(v);
        if (_size==capacity) pop_front();
        if (_size < capacity) ++_size;
        _backIdx = (_backIdx+1) % capacity;
    }

    void push_back (const T& v) {
        items[_backIdx] = v;
        if (_size==capacity) pop_front();
        if (_size < capacity) ++_size;
        _backIdx = (_backIdx+1) % capacity;

    }
    

    T& front (void) {
        return items[_frontIdx];        
    }
    void pop_front(void) {
        if (_size==0) return;
        _frontIdx= (_frontIdx+1)%capacity;
    }

private:


    T *items; 
    size_t _size=0;
    size_t _backIdx=0;
    size_t _frontIdx=0;
    
    
        
};


#endif // __CIRCULAR_BUFFER__
