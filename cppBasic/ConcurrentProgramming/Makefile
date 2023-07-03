VSIZE := 100

.PHONY: build configure run test clean

build: configure
	cmake --build build

configure:
	cmake -B build -DVSIZE=${VSIZE}

run:
	./build/answer_app

test:
	ctest --test-dir build -R "^calcsum."

clean:
	rm -rf build


# CXX = g++
# SOURCE_CXX = calcSum.cpp
# TARGET = calcSumTest
# LDFLAGS_COMMON = -std=c++14 -stdlib=libc++ -lgtest

# all:
# 	$(CXX) $(SOURCE_CXX) $(LDFLAGS_COMMON) -Wall -Wextra -o $(TARGET)
# clean:
# 	rm -rf *.o $(TARGET)