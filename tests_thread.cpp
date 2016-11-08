#include "gmock/gmock.h"
#include "CircularBuffer.h"
#include <thread>

using namespace std;
using namespace testing;

class TestCircularBufThread: public Test {

public:
    enum {MAX_SIZE=100000};
    CircularBuffer<int, MAX_SIZE> cbuf; 

};

TEST_F (TestCircularBufThread, Producer1Consumer1) {


    cbuf.push_back(0);
    bool stop=false;

    thread  producer ([&stop, this](){
        for (long i=1; i<100000000; ++i) {
            cbuf.push_back (i);
            for (long j=1; j<5; ++j); // small delay
            

        }
        stop=true;
    }
    );
    thread  consumer ([&stop, this](){
        
        auto it = cbuf.cbegin();
        long prev_v = *it;
        long v;
        while (!stop && it!=cbuf.cend()) {
            v = *(++it);
            ASSERT_THAT(v, Eq(prev_v+1));
            prev_v = v;
        }
    }
    );
    producer.join();
    consumer.join();
}
