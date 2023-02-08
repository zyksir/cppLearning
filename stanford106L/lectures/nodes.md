# standord 106L 笔记整理

## Lec2

这节课首先强调了 Cpp 是一个重视 type 的语言，任何变量都有 type；因此很多问题都可以在编译时解决。
另外关于 type 提到了 Struct, pair 和 auto。Struct是一个拥有很多个不同类型 type 变量的集合体；pair 可以快速整个两个不同类型的变量；auto 则是C++11之后的特性，可以完成 type 的自动推导。

```cpp
/* an example of code using pair */
std::pair<bool, std::pair<double, double> > quadratic(double a, double b, double c){
    double radical = pow(b, 2) - (4*a*c);

    if(radical < 0){
        return std::make_pair(false, std::make_pair(-1, -1));
    } else {
        double root1 = ( -1*b + sqrt(radical) ) / (2*a);
        double root2 = ( -1*b - sqrt(radical) ) / (2*a);
    
        return std::make_pair(true, std::make_pair(root1, root2));
    }
}
```
