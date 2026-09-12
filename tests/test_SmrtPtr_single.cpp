#include <gtest/gtest.h>
#include "SmrtPtr.hpp"
#include <stdexcept>

struct TestObject {
    static int destructor_calls;

    ~TestObject() {
        destructor_calls++;
    }
};

int TestObject::destructor_calls = 0;

TEST(SmrtPtrSingle, Create) {
    SmrtPtr<int> ptr(new int(10));
    EXPECT_TRUE(ptr);
}

TEST(SmrtPtrSingle, Dereference) {
    SmrtPtr<int> ptr(new int(10));
    EXPECT_EQ(*ptr, 10);
}

TEST(SmrtPtrSingle, DereferenceWrite) {
    SmrtPtr<int> ptr(new int(10));

    *ptr = 100;

    EXPECT_EQ(*ptr, 100);
}

TEST(SmrtPtrSingle, CopyConstructor) {
    SmrtPtr<int> ptr1(new int(10));
    SmrtPtr<int> ptr2(ptr1);

    EXPECT_EQ(ptr1.Get(), ptr2.Get());
    EXPECT_EQ(*ptr2, 10);
    EXPECT_EQ(ptr1.UseCount(), 2);
    EXPECT_EQ(ptr2.UseCount(), 2);
}

TEST(SmrtPtrSingle, DefaultConstructor) {
    SmrtPtr<int> ptr;
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_TRUE(!ptr);
}

TEST(SmrtPtrSingle, ConstructFromRawPointer) {
    int *a = new int(52);
    SmrtPtr<int> ptr(a);
    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(*ptr, 52);
    EXPECT_EQ(ptr.UseCount(), 1);
}

TEST(SmrtPtrSingle, DereferenceNullThrows) {
    SmrtPtr<int> ptr;
    EXPECT_THROW(*ptr, std::logic_error);
}

TEST(SmrtPtrSingle, BoolOperatorFalse) {
    SmrtPtr<int> ptr;
    EXPECT_FALSE(ptr);
}

TEST(SmrtPtrSingle, BoolOperatorTrue) {
    SmrtPtr<int> ptr(new int(10));
    EXPECT_TRUE(ptr);
}

TEST(SmrtPtrSingle, CopyAssignment) {
    SmrtPtr<int> ptr1(new int(52));
    SmrtPtr<int> ptr2(new int(10));

    ptr1 = ptr2;

    EXPECT_EQ(*ptr1, 10);
    EXPECT_EQ(ptr1.Get(), ptr2.Get());
    EXPECT_EQ(ptr1.UseCount(), 2);
    EXPECT_EQ(ptr2.UseCount(), 2);
}

TEST(SmrtPtrSingle, SelfCopyAssignment) {
    int *a = new int(52);
    SmrtPtr<int> ptr(a);
    ptr = ptr;

    EXPECT_EQ(*ptr, 52);
    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(ptr.Get(), a);
}

TEST(SmrtPtrSingle, MoveConstructor) {
    int *a = new int(52);
    SmrtPtr<int> ptr(a);
    SmrtPtr<int> ptr2(std::move(ptr));

    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(ptr2.UseCount(), 1);
    EXPECT_EQ(ptr2.Get(), a);
}

TEST(SmrtPtrSingle, MoveAssignment) {
    int *a = new int(52);
    SmrtPtr<int> ptr(a);
    SmrtPtr<int> ptr2;

    ptr2 = std::move(ptr);

    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(ptr2.UseCount(), 1);
    EXPECT_EQ(ptr2.Get(), a);
}

TEST(SmrtPtrSingle, SelfMoveAssignment) {
    int *a = new int(10);
    SmrtPtr<int> ptr(a);

    ptr = std::move(ptr);

    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(ptr.Get(), a);
}

TEST(SmrtPtrSingle, ReferenceCounterIncreases) {
    SmrtPtr<int> a(new int(52));
    EXPECT_EQ(a.UseCount(), 1);

    SmrtPtr<int> b = a;
    EXPECT_EQ(a.UseCount(), 2);

    SmrtPtr<int> c = a;
    EXPECT_EQ(a.UseCount(), 3);
}

TEST(SmrtPtrSingle, ReferenceCounterDecreases) {
    SmrtPtr<int> a(new int(10));
    EXPECT_EQ(a.UseCount(), 1);
    {
        SmrtPtr<int> b = a;
        EXPECT_EQ(a.UseCount(), 2);

        SmrtPtr<int> c = a;
        EXPECT_EQ(a.UseCount(), 3);
    }
    EXPECT_EQ(a.UseCount(), 1);
}

TEST(SmrtPtrSingle, ObjectDeleteOnce) {
    TestObject::destructor_calls = 0;
    {
        SmrtPtr<TestObject> a(new TestObject());
        {
            SmrtPtr<TestObject> b = a;
            SmrtPtr<TestObject> c = a;

            EXPECT_EQ(TestObject::destructor_calls, 0);
        }
        EXPECT_EQ(TestObject::destructor_calls, 0);
        EXPECT_EQ(a.UseCount(), 1);
    }

    EXPECT_EQ(TestObject::destructor_calls, 1);
}

TEST(SmrtPtrSingle, CopyEmptyPointer) {
    SmrtPtr<int> a;
    SmrtPtr<int> b(new int(10));

    a = b;

    EXPECT_TRUE(b.Get());
    EXPECT_TRUE(a.Get());
    EXPECT_EQ(*a, 10);
    EXPECT_EQ(*b, 10);
    EXPECT_EQ(a.UseCount(), 2);
    EXPECT_EQ(b.UseCount(), 2);
}

TEST(SmrtPtrSingle, MoveEmptyPointer) {
    SmrtPtr<int> a;
    SmrtPtr<int> b(new int(10));

    a = std::move(b);

    EXPECT_FALSE(b);
    EXPECT_TRUE(a);
    EXPECT_EQ(*a, 10);
    EXPECT_EQ(a.UseCount(), 1);
}

TEST(SmrtPtrSingle, CopyAssignmentFromEmptyPointer) {
    SmrtPtr<int> a(new int(10));
    SmrtPtr<int> b;
    a = b;

    EXPECT_FALSE(a);
    EXPECT_THROW(*a, std::logic_error);
}

TEST(SmrtPtrSingle, MoveAssignmentFromEmptyPointer) {
    SmrtPtr<int> a(new int(10));
    SmrtPtr<int> b;
    a = std::move(b);

    EXPECT_FALSE(a);
    EXPECT_FALSE(b);
    EXPECT_THROW(*a, std::logic_error);
}

TEST(SmrtPtrSingle, MultiplePointersDeleteOnce) {
    TestObject::destructor_calls = 0;
    {
        SmrtPtr<TestObject> a(new TestObject());
        SmrtPtr<TestObject> b(a);
        SmrtPtr<TestObject> c(a);
        SmrtPtr<TestObject> d(a);
        SmrtPtr<TestObject> e(a);
        
        ///

        SmrtPtr<TestObject> f(a);
    }
    EXPECT_EQ(TestObject::destructor_calls, 1);
}

TEST(SmrtPtrSingle, CopyEmptyPointerConstructor) {
    SmrtPtr<int> a;
    SmrtPtr<int> b(a);

    EXPECT_FALSE(a);
    EXPECT_FALSE(b);
    EXPECT_THROW(*b, std::logic_error);
    EXPECT_EQ(b.UseCount(), 0);
}

TEST(SmrtPtrSingle, MoveEmptyPointerConstructor) {
    SmrtPtr<int> a;
    SmrtPtr<int> b(std::move(a));

    EXPECT_FALSE(a);
    EXPECT_FALSE(b);
    EXPECT_THROW(*b, std::logic_error);
    EXPECT_EQ(b.UseCount(), 0);
}

TEST(SmrtPtrSingle, MoveAssignmentNonEmpty) {
    TestObject::destructor_calls = 0;
    {
        SmrtPtr<TestObject> a(new TestObject());
        SmrtPtr<TestObject> b(new TestObject());

        b = std::move(a);

        EXPECT_FALSE(a);
        EXPECT_TRUE(b);
        EXPECT_EQ(b.UseCount(), 1);
    }
    EXPECT_EQ(TestObject::destructor_calls, 2);
}