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
    ASSERT_THAT (cbuf.size(), Eq(size_t( 2)));
    ASSERT_THAT (cbuf.front(), Eq( 2));
    cbuf.pop_front();
    ASSERT_THAT (cbuf.size(), Eq(size_t( 1)));
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

TEST_F(TestCircularBuf, TestIteratorBeginAndEnd) {
    cbuf.push_back(1);
    cbuf.push_back(2);
    cbuf.push_back(3);
   
    auto it = cbuf.begin();

    ASSERT_THAT(*it, Eq(1));
    ++it;
    ASSERT_THAT(*it, Eq(2));
    it++;
    ASSERT_THAT(*it, Eq(3));

    --it;
    ASSERT_THAT(*it, Eq(2));
    it--;
    ASSERT_THAT(*it, Eq(1));


}
TEST_F(TestCircularBuf, TestIteratorLoopSimple) {

    cbuf.push_back(1);
    cbuf.push_back(2);
    cbuf.push_back(3);
    
    int val=1;
    for (auto it = cbuf.begin(); it!=cbuf.end(); ++it) {
       ASSERT_THAT(*it, Eq(val++));
    }

    val=1;
    for (auto it = cbuf.cbegin(); it!=cbuf.cend(); ++it) {
       ASSERT_THAT(*it, Eq(val++));
    }

}
TEST_F(TestCircularBuf, TestIteratorLoopCircular) {

    
    for (int v=1; v<=MAX_SIZE+5; v++) {
        cbuf.push_back (v);     // pass by reference const
    }

    int val=1+5;
    for (auto it = cbuf.begin(); it!=cbuf.end(); ++it) {
       ASSERT_THAT(*it, Eq(val++));
    }

    val=1+5;
    for (auto it = cbuf.cbegin(); it!=cbuf.cend(); ++it) {
       ASSERT_THAT(*it, Eq(val++));      
    }

}

class A{
public:
    A()  {++_instances;}
    ~A() {--_instances;}
    A(const A& other) {++_copy_ops;}
    A& operator= (const A& other) { ++_copy_ops; return *this;}
    

    static int _instances;
    static int _copy_ops;
};
int A::_instances =0;
int A::_copy_ops = 0;

TEST_F (TestCircularBuf, PushAndPopObject) {
    ::A::_copy_ops =0;
    ::A::_instances=0;

    CircularBuffer<::A,10> bufA;

    ASSERT_THAT(A::_instances,10);

    bufA.push_back_reuse(); // just reuse the same object; don't make new one
    ASSERT_THAT(A::_instances,10);
    ASSERT_THAT(A::_copy_ops,0);
   
    ::A a;

    bufA.push_back(std::move(a));
    ASSERT_THAT(A::_copy_ops,1);

    ::A::_copy_ops =0;
    bufA.push_back(::A());
    ASSERT_THAT(A::_copy_ops,1);


}


int main(int argc, char *argv[])
{
	testing::InitGoogleMock (&argc, argv);
	return RUN_ALL_TESTS();
}
