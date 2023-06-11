#include <iostream>
#include <vector>
#include <memory>

namespace test {

template<typename T, int chunkSize>
class ObjectPool {
private:
    std::vector<int> nextAvailableChunk;
    int firstAvailableChunk;
    char* memoryBegin;
public:
    ObjectPool() {
        nextAvailableChunk.resize(chunkSize);
        memoryBegin = static_cast<char*>(malloc(chunkSize * sizeof(T)));
        for(int i = 0; i < chunkSize-1; ++i) {
            nextAvailableChunk[i] = i+1;
        }
        // means there is no available chunk after this chunk
        nextAvailableChunk[chunkSize-1] = -1;
        firstAvailableChunk = 0; // head of the list
    }

    ~ObjectPool() {
        for(int i = 0; i < chunkSize; ++i) {
            if(nextAvailableChunk[i] < 0) {
                // nextAvailableChunk[i] = firstAvailableChunk;
                // firstAvailableChunk = i;
                T* p = reinterpret_cast<T*>(memoryBegin + i * sizeof(T));
                p->~T();
            }
        }
        free(memoryBegin);
    }

    template<class ...Args>
    T* make(Args&&... params) {
        if (firstAvailableChunk == -1) {
            return nullptr; 
        }
        int chunkIndex = firstAvailableChunk;
        firstAvailableChunk = nextAvailableChunk[chunkIndex];
        nextAvailableChunk[chunkIndex] = -1;

        char* objectMemory = memoryBegin + sizeof(T) * chunkIndex;
        T* objectPointer = new(objectMemory) T(std::forward<Args>(params)...);

        // std::cout << firstAvailableChunk << std::endl;
        // for(int i = 0; i < chunkSize; ++i) {
        //     std::cout << i << ",";
        // }
        // std::cout << std::endl;
        return objectPointer;
    }

    void kill(T* p) {
        if (!p) {
            return;
        }
        p->~T();
        int index = (reinterpret_cast<char*>(p) - memoryBegin) / sizeof(T);
        if (index < 0 || index >= chunkSize) {
            return;
        }
        nextAvailableChunk[index] = firstAvailableChunk;
        firstAvailableChunk = index;
    }
};

}