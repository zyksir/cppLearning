#include<iostream>
#include "shared_ptr.h"
#include "unique_ptr.h"

#include <gtest/gtest.h>

class Foo {};
std::unique_ptr<int> x;

TEST(shared_ptr, lifetime) {
    // test construct function
    test::shared_ptr<Foo> x(new Foo());
    ASSERT_EQ(*x.count, 1);
    ASSERT_NE(x.ptr, nullptr);

    // test copy construct function
    test::shared_ptr<Foo> y = x;
    ASSERT_EQ(*x.count, 2);
    ASSERT_EQ(x.ptr, y.ptr);

    // test copy construct function
    test::shared_ptr<Foo> nx = std::move(x);
    ASSERT_EQ(nx.ptr, y.ptr);
    ASSERT_EQ(x.ptr, nullptr);
    ASSERT_EQ(*nx.count, 2);

    test::shared_ptr<Foo> z, nz;
    ASSERT_FALSE((bool)z);
    nz = std::move(nx);
    z = y;
    ASSERT_TRUE((bool)z);
    ASSERT_EQ(*z.count, 3);
    ASSERT_EQ(z.ptr, nz.ptr);
}

TEST(unique_ptr, lifetime) {
    // test construct function
    test::unique_ptr<Foo> x(new Foo());
    ASSERT_NE(x.get(), nullptr);

    // // test copy construct and assignment is delete
    // test::shared_ptr<Foo> y = x, z;
    // z = x;

    // test move construct function
    test::unique_ptr<Foo> nx = std::move(x);
    ASSERT_NE(nx.get(), nullptr);
    ASSERT_EQ(x.get(), nullptr);

    // test move assignment function
    test::unique_ptr<Foo> nz;
    ASSERT_FALSE((bool)nz);
    nz = std::move(nx);
    ASSERT_NE(nz.get(), nullptr);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}