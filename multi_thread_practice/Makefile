CXX = g++

SOURCE_CXX = calcSum.cpp

TARGET = calcSumTest
LDFLAGS_COMMON = -std=c++14 -stdlib=libc++ -lgtest

all:
	$(CXX) $(SOURCE_CXX) $(LDFLAGS_COMMON) -Wall -Wextra -o $(TARGET)
clean:
	rm -rf *.o $(TARGET)