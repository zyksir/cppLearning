#include "objectPool.h"
#include <gtest/gtest.h>

class Foo {
public:
    double y;
    int x;
    Foo(int _x, double _y):y(_y),x(_x) {}
};

TEST(objectPool, init) {
    test::ObjectPool<Foo, 64> foo_pool;
    for(int i = 0; i < 64 - 1; ++i) {
        Foo* p_temp = foo_pool.make(i, -i);
        ASSERT_NE(p_temp, nullptr);
    }
    Foo* p = foo_pool.make(64, -64);
    Foo* p_null = foo_pool.make(65, -65);
    ASSERT_EQ(p_null, nullptr);
    ASSERT_EQ(p->x, 64);
    ASSERT_EQ(p->y, -64);

    foo_pool.kill(p);
    Foo* p_new = foo_pool.make(65, -65);
    ASSERT_EQ(p_new->x, 65);
    ASSERT_EQ(p->y, -65);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}