#include "gmock/gmock.h"
#include "CircularBuffer.h"

using namespace std;
using namespace testing;

class TestCircularBuf: public Test {

public:
    enum {MAX_SIZE=10};
    CircularBuffer<int, MAX_SIZE> cbuf; 

};

TEST_F (TestCircularBuf, Declaration) {
    ASSERT_THAT (cbuf.max_size(), Eq(MAX_SIZE));
    ASSERT_THAT (cbuf.size(), Eq(size_t( 0)));
    ASSERT_THAT (cbuf.empty(), Eq(true));
}

// should run with valgrind --leak-check=full -
TEST_F (TestCircularBuf, MemoryShouldNotLeakIfConstructorFails) {
    
    const size_t huge_size = 100000000;
    CircularBuffer<int, huge_size> hugeBuf; 
    ASSERT_THAT (cbuf.size(), Ne(huge_size)); 
}

TEST_F (TestCircularBuf, PushAndPop) {
    cbuf.push_back (1);
    int val=2;
    cbuf.push_back (val);
    cbuf.push_back (3);
    
    ASSERT_THAT (cbuf.size(), Eq(size_t( 3)));

    ASSERT_THAT (cbuf.front(), Eq( 1));
    cbuf.pop_front();
    ASSERT_THAT (cbuf.front(), Eq( 2));
    cbuf.pop_front();
    ASSERT_THAT (cbuf.front(), Eq( 3));
    cbuf.pop_front();



}

TEST_F (TestCircularBuf, PushAndPopCircular) {

    for (int v=1; v<=MAX_SIZE+5; v++) {
        cbuf.push_back (v);     // pass by reference const
    }

    ASSERT_THAT(cbuf.size(), Eq(MAX_SIZE));
    
    for (int v=1+5; v<=MAX_SIZE+5; v++) {
        ASSERT_THAT(cbuf.front(), Eq(v));
        cbuf.pop_front();    
    }

    for (int v=1; v<=MAX_SIZE+5; v++) {
        cbuf.push_back (1+v);     // pass by rvalue 
    }
    
    ASSERT_THAT(cbuf.size(), Eq(MAX_SIZE));
    
    for (int v=1+1+5; v<=MAX_SIZE+5; v++) {
        ASSERT_THAT(cbuf.front(), Eq(v));
        cbuf.pop_front();    
    }


}




int main(int argc, char *argv[])
{
	testing::InitGoogleMock (&argc, argv);
	return RUN_ALL_TESTS();
}
