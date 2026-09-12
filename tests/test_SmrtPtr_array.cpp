#include <gtest/gtest.h>
#include "SmrtPtr.hpp"

struct TestObject {
    static int destructor_calls;

    ~TestObject() {
        destructor_calls++;
    }
};

TEST(SmrtPtrArray, ArrayCreate) {
    SmrtPtr<int> data(new int[10], 10);

    EXPECT_TRUE(data);
    EXPECT_EQ(data.UseCount(), 1);
}

TEST(SmrtPtrArray, ArrayRead) {
    SmrtPtr<int> data(new int[10], 10);

    for (size_t i = 0; i < 10; i++) {
        data[i] = i + 1;
    }

    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[1], 2);
    EXPECT_EQ(data[5], 6);
    EXPECT_EQ(data[9], 10);
    
}

TEST(SmrtPtrArray, ArrayWrite) {
    SmrtPtr<int> data(new int[10], 10);
    data[2] = 100;

    EXPECT_EQ(data[2], 100);
}

TEST(SmrtPtrArray, ArrayOutOfRange) {
    SmrtPtr<int> data(new int[10], 10);
    EXPECT_THROW(data[1000], std::out_of_range);
    EXPECT_THROW(data[10], std::out_of_range);
}

TEST(SmrtPtrArray, ArrayOperatorOnSingleThrows) {
    SmrtPtr<int> ptr(new int(10));

    EXPECT_THROW(ptr[0], std::logic_error);
}

TEST(SmrtPtrArray, DereferenceArrayThrows) {
    SmrtPtr<int> ptr(new int[5], 5);

    EXPECT_THROW(*ptr, std::logic_error);
}

TEST(SmrtPtrArray, ArrayCopyReferenceCounter) {
    SmrtPtr<int> a(new int[5], 5);
    SmrtPtr<int> b = a;

    EXPECT_EQ(a.UseCount(), 2);
    EXPECT_EQ(b.UseCount(), 2);
    EXPECT_EQ(a.Get(), b.Get());
}

TEST(SmrtPtrArray, ArrayDeleteOnce) {
    TestObject::destructor_calls = 0;

    {
        SmrtPtr<TestObject> arr(new TestObject[5], 5);

        EXPECT_EQ(TestObject::destructor_calls, 0);
    }

    EXPECT_EQ(TestObject::destructor_calls, 5);
}

TEST(SmrtPtrArray, SharedArrayDeletesOnce) {
    TestObject::destructor_calls = 0;

    {
        SmrtPtr<TestObject> a(new TestObject[5], 5);

        {
            SmrtPtr<TestObject> b = a;
            EXPECT_EQ(b.UseCount(), 2);
        }

        EXPECT_EQ(TestObject::destructor_calls, 0);
    }
    EXPECT_EQ(TestObject::destructor_calls, 5);
}