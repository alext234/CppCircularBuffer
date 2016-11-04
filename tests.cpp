#include "gmock/gmock.h"
#include "CircularBuffer.h"

using namespace std;
using namespace testing;

int main(int argc, char *argv[])
{
	testing::InitGoogleMock (&argc, argv);
	return RUN_ALL_TESTS();
}
