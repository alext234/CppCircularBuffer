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
        inc_size();
        inc_idx(_backIdx);
        
    }
    void push_back (const T& v) {
        items[_backIdx] = v;
        inc_size();
        inc_idx(_backIdx);
    }
    

    T& front (void) {
        return items[_frontIdx];        
    }
    void pop_front(void) {
        if (_frontIdx==_backIdx) return;
        inc_idx(_frontIdx);
    }

private:

    inline void inc_size() { if (_size<capacity) ++_size;}
    inline void inc_idx(size_t& idx) {idx=(idx+1) % capacity;}

    T *items; 
    size_t _size=0;
    size_t _backIdx=0;
    size_t _frontIdx=0;
    
    
        
};


#endif // __CIRCULAR_BUFFER__
