# cs106L-assignment1

Assignment #1 for CS106L. Check out our course website [here](http://cs106l.stanford.edu)!
这是一个简单的搜索问题，直接看答案就好了。我在这里 mark 几个我觉得有意思的点：

## std::search vs std::find

std::search: 找到序列 [source.begin(), source.end()) 在 [target.begin(), target.end()) 第一个出现的位置。
std::find: 找到元素 item 在 [target.begin(), target.end()) 第一个出现的位置。
std::find_first_of: 找到序列 [source.begin(), source.end()) 中的元素在 [target.begin(), target.end()) 第一个出现的位置。
另外 std::search 还使用了 [Boyer-Moore](https://www.youtube.com/watch?v=4Xyhb72LCX4&t=11s) 优化，这块我暂时还没看 orz。还有很多其他的可以参考[cpp17 series](https://www.cppstories.com/2018/08/searchers/)。

## 自定比较函数的优先队列

```cpp
auto cmp = [&](const vector<string>& lhs, const vector<string>& rhs) { ... };
std::priority_queue<vector<string>, vector<vector<string>>, decltype(cmp)> q(cmp);
```
