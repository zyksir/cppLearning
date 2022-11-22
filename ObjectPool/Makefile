CXX = clang++

SOURCE_CXX = objectPoolTest.cpp

TARGET = objectPoolTest
LDFLAGS_COMMON = -std=c++14 -stdlib=libc++ -lgtest

all:
	$(CXX) $(SOURCE_CXX) $(LDFLAGS_COMMON) -Wall -Wextra -o $(TARGET)
clean:
	rm -rf *.o $(TARGET)