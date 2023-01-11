# cs106L-assignment2

Assignment #2 for CS106L. Check out our course website [here](http://cs106l.stanford.edu)! 

这次的问题描述就是完成一个简单版本的 unordered_map。之后首先介绍大体思路，然后介绍几个比较有意思的细节。

## 大体思路

这个 map 是通过拉链法解决哈希冲突。在一个 map 里有一个 _buckets_array。每个 bucket 存一个链表，链表存 hash(key) % array.size() 是 index of bucket 的所有元素。

## MileStone1: const 的使用

这个作业的第一题是让我们去看哪些函数有 const 版本的重载。这些函数是可能修改 map 也可能不修改的，所以需要两个版本，这样 const map& 也可以调用这些函数。

```cpp
iterator find(const K& key);
const_iterator find(const K& key) const;
M& at(const K& key);
const M& at(const K& key) const;

iterator begin();
const_iterator begin() const;
iterator end();
const_iterator end() const;
```

第一个有意思的点是，如何测试某段代码不可以编译。一种朴素的办法是，编译两次，一次是有这个代码的，编译会失败；另一次删了这个代码的，编译能过。但这个办法看起来就很挫，每次还要手动改一下 test.cpp 再改回来。这里使用了模板来解决这个问题。某个函数 f 有两个重载。编译器会尝试编译第二个版本，如果失败了就不会使用这个版本。而这个版本里有一个 assert false，如果使用这个版本必报错。以此完成了关于"this code should not compile"的功能。另外 decltype 并不接受两个参数，这里其实是 decltype 在去判断一个表达式的类型。因为 "A, B" 的类型是 B 的类型，所以 decltype(A, B)获得就就是 B 的类型(如果 A 可以编译成功的话)。参考[What is decltype with two arguments?](https://stackoverflow.com/questions/16044514/what-is-decltype-with-two-arguments)。这个 trick 是专门用来检测某段代码不能编译的。

```cpp
template <typename... Ts>
void ensure_no_const_index_op(Ts...) {}

template <typename T>
auto ensure_no_const_index_op(const T& map) -> decltype((void) (map[0]), void()) {
    VERIFY_TRUE(false, __LINE__);
}
```

第二个有意思的点是 static_cast/const_cast trick。其目的是为了复用非 const 版本的代码来实现 const 版本。我们可以用 const_cast 来把 this 转换成 non-const 类型的，然后调用已经实现好的函数。当然这样要求这个函数本身不修改 class，否则就会很奇怪。



```cpp
template <typename K, typename M, typename H>
typename HashMap<K, M, H>::const_iterator HashMap<K, M, H>::end() const {
    return static_cast<const_iterator>(const_cast<HashMap<K, M, H>*>(this)->end());
}
```

## MileStone2: Move 和 Copy

copy 的核心思路就是把传入的 rhs 里的所有元素 insert 到这个 map 中；Move 则比较复杂，首先要清除自己已有的元素(否则会内存泄露)，然后把 rhs 里面的每个 attribute move 过来。我之前直接 `_buckets_array = std::move(rhs._buckets_array);`，但是这样 rhs 里的_buckets_array就是未定义的，所以还需要重新 resize 并赋值为 nullptr; 因此还不如直接使用循环 `_buckets_array[i] = std::move(rhs._buckets_array[i]); rhs._buckets_array[i] = nullptr;`。

```cpp
HashMap(const HashMap& rhs);
HashMap& operator=(const HashMap& rhs);
HashMap(HashMap&& rhs);
HashMap& operator=(HashMap&& rhs);
```

## 小问答

1. Templates and Template Classes
Q : 在 rehash() 函数中，for-each 循环有一个 auto&，这里 auto 推导出来的类型是什么？什么需要一个 &?
A: auto 推到出来的是 node*。如果不使用 &，循环中就会复制每个 curr，这样 _buckets_array 就不是 all nullptr 了。

2. HashMap Pair Type
Q : STL 容器会存储元素的 value_type, 在本次中其值是std::pair<const K, M>, 如果使用 std::pair<K, M> 会怎么样？
A : 这样我们就可以通过迭代器修改 key 的值。但是这样是不对的，因为 key 必须是常数，key 变了而对应 bucket 的 index 不变就会导致问题。

3. Find vs find
Q : 如果需要在 HashMap m 中搜索 key k, m.find(k) 和 std::find(m.begin(), m.end(), k) 哪个更好？
A : m.find(k) 更好。因为平均时间复杂度是 O(1), 而 std::find 是 O(n)。

4. RAII?
Q : This HashMap class is RAII-compliant. Explain why.
A : Because the destructor function will call the HashMap::clear() which free all the resources which HashMap acquires.

5. Privacy
Q : 为什么 HashMapIterator 的构造函数是 private 的? 那么 HashMapIterator 如何被构造呢？
A : 因为构造 HashMapIterator 的时候需要 HashMap 的私有变量. 另外，由于 HashMapIterator 是 HashMap 友元类, 所以HashMap 可以调用 HashMapIterator 的私有函数，也就可以构造HashMapIterator.

6. Did We Make A Mistake?
Q : 为什么 at() 有 const and non-const 版本, operator[] 只有 non-const 版本呢? (而 Vector 的 operator[] 就有const 版本).
A : 因为如果 key 不存在，一个 {key, value} 会被加到 map 里， 这个行为会修改 map。而 Vector 的 [] 就没有这个修改容器的逻辑。

7. Attachment Issues
Q : 为什么 HashMap 中 move/copy constructor/assignment 都需要我们自己实现，而 HashMapIterator 我们可以直接使用 default?
A : 对 HashMap，我们需要管理内存(释放已经分配的内存 或者 重新分配内存)。因此不能直接用 default。

8. Move Semantics
Q : 在我们实现的 move constructor or move assignment operator中, 为什么我们仍然使用了 std::move 来移动 rhs 的成员，即使 rhs 已经是右值了？,
A : 因为一个右值的成员并不一定是右值，这可能会导致复制而不是移动。
