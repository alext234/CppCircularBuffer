#include "gmock/gmock.h"
#include "CircularBuffer.h"
#include <thread>
#include <chrono>

using namespace std;
using namespace testing;

class TestCircularBufThread: public Test {

public:
    enum {MAX_SIZE=10000};
    CircularBuffer<int, MAX_SIZE> cbuf; 

};

TEST_F (TestCircularBufThread, Producer1Consumer1) {


    const long START_VAL=10000;
    cbuf.push_back(START_VAL);
    bool stop=false;

    thread  producer ([&stop, this](){
        for (long i=START_VAL+1; i<START_VAL+999999; ++i) {
            cbuf.push_back (i);
            

        }
        stop=true;
    }
    );
    std::this_thread::sleep_for(100ms);
   
        
    auto it = cbuf.cbegin();
    volatile long prev_v = *it;
    volatile long v;
    volatile long v1;
    while(cbuf.empty());
    while (!stop ) {
        ++it;
        while (it==cbuf.cend());
        ++prev_v;
        v = *(it);
        EXPECT_THAT(v, Eq(prev_v));
        v1 = *(it);
        ASSERT_THAT(v1, Eq(prev_v));
        prev_v = v;
    }
    producer.join();
}
