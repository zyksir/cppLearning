# C++面试题：ObjectPool 的实现

## 问题描述

我们经常会遇到需要生成新对象的问题。但是每次生成一个新的对象，会需要重新分配内存；对象被析构之后还要回收内存。这一部分分配/回收内存的时间是完全可以省去的。具体的方法就是实现一个 ObjectPool。ObjectPool 通过提前申请内存的方式，每次收到请求的时候，从已经分配好的内存里快速构造对象。

## 基本思路

首先我们明确一些 ObjectPool 类的接口。在面试过程中，最优先需要确认的就是这些 high level 的 design idea。

1. 对外暴露的接口是：make 在已经分配好的内存上构造一个对象，返回该对象; kill 将某个对象析构，保留内存以便之后分配给其他对象。
2. 构造函数里，它需要完成的是分配内存，同时也要想办法维护一个数据结构来判断哪些内存是可以用的；在析构函数里，要析构其中的所有对象，然后把内存释放。

```c++
template<typename T, int chunkSize>
class ObjectPool {
private:
    void* memoryBegin;
public:
    ObjectPool();
    ~ObjectPool();
    template<class ...Args>
    T& make(Args&&... params);
    void kill(T& p);
};
```

## 构造函数 & 析构函数

在设计构造函数的时候，成员也应该一并设计好。首先我们需要一个 void*的指针，指向被分配的内存的首地址。然后，为了知道哪一个 chunk 的地址，我们维护一个代表可用的 chunk 的 list，每次去头部元素即可。我们用 vector 来实现这个 list，value 的含义是下一个空闲的Chunk，-1表示当前 chunk 已经被占用了。

```c++
ObjectPool() {
  	memoryBegin = malloc(chunkSize * sizeof(T));
    nextAvailableChunk.resize(chunkSize);
    for(int i = 0; i < chunkSize-1; ++i) {
      nextAvailableChunk[i] = i+1;
    }
    // means there is no available chunk after this chunk
    nextAvailableChunk[chunkSize-1] = -1;
    firstAvailableChunk = 0; // head of the list
}
```

在析构函数中，我们只需要遍历vector，找到占用的 object，析构掉这些 object，然后释放最开始申请的内存即可。对于第 i 个 chunk，我们通过memoryBegin + i * sizeof(T)可以拿到对应对象的地址的第一个字节，然后使用reinterpret_cast<T*>拿到对应的 T\*地址。

```c++
~ObjectPool() {
    for(int i = 0; i < chunkSize; ++i) {
        if(nextAvailableChunk[i] < 0) {
            T* p = reinterpret_cast<T*>(memoryBegin + i * sizeof(T));
            p->~T();
        }
    }
    free(memoryBegin);
}
```

- 问：如果不调用对象的析构函数，会有什么样的问题？答：会造成内存泄露。可能这个对象在自己构造函数里调用了malloc，在析构函数里调用了 free，如果忘了调用构造函数，会导致这部分内存泄露。
- [stackoverflow: static_cast, dynamic_cast, const_cast, reinterpret_cast介绍](https://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-const-cast-and-reinterpret-cast-be-used)。这里使用 static_cast 还是用 reinterpret_cast 我有些犹豫，我在 github 上看到另一个人是用了reinterpret_cast，但就我的理解可能static_cast 也可以，具体行不行还是交给测试来判断。
  - static_cast 适用于基本类型之间(e.g int 到 float)的转换、指针到 `void*`、具有继承关系的类。注意，如果在有虚函数的类里这么做是很危险的，因为static_cast 不会做任何事情；另外转到一个没有继承关系的类是未定义的行为。
  - const_cast 是修改变量 const 属性的(也是唯一一个可以做到这件事情的)；dynamic_cast 是专门用来处理多态的的，这里都没关系
  - reinterpret_cast 是最危险的，可以进行任意的转换

## 核心接口

```c++
template<class ...Args>
T* make(Args&&... params) {
    if (nextAvailableChunk == -1) {
        return nullptr; 
    }
    int chunkIndex = firstAvailableChunk;
    firstAvailableChunk = nextAvailableChunk[firstAvailableChunk];
    nextAvailableChunk[firstAvailableChunk] = -1;

    void* objectMemory = memoryBegin + sizeof(T) * chunkIndex;
    T* objectPointer = new(objectMemory) T(std::forward<Args>(params)...);
    return objectPointer;
}
```

首先从链表头部取出一个 block，然后使用其对应的内存构造 object。这里使用了[placement `new`()](https://isocpp.org/wiki/faq/dtors#placement-new)，它可以使用已经分配好的内存。(refer from [here](https://stackoverflow.com/questions/1554774/create-new-c-object-at-specific-memory-address)).

- 问：如果不使用完美转发 forward 会怎么样。(注：这里的 Args&&是 universal references 而不是 rvalue references) 答：会导致将右值和左值搞反，严重的话会导致程序崩溃。 (具体可以详细参考 [Effective-Modern C++ item25](https://github.com/CnTransGroup/EffectiveModernCppChinese/blob/master/src/5.RRefMovSemPerfForw/item25.md))
- 关于 args&&, 这个在 effective-modern C++里也多次提到了。这个可以实现任意数量参数的输入。



另外，kill 函数就比较简单，只需要做一些简单的检查即可。这里最令我没想到的是，p->~T()这个用法。

```c++
void kill(T* p) {
    if (!p) {
        return;
    }
    p->~T();
    int index = (reinterpret_cast<void*>(p) - memoryBegin) / sizeof(T);
    if (index < 0 || index >= chunkSize) {
        return;
    }
    nextAvailableChunk[index] = firstAvailableChunk;
    firstAvailableChunk = index;
}
```

