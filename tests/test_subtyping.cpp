#include "gtest/gtest.h"
#include "SmrtPtr.hpp"

struct Animal {
    virtual ~Animal() = default;
    virtual int GetValue() const { return 1; }
};

struct Cat : Animal {
    int GetValue() const override { return 2; }
};

TEST(SmrtPtr, SubtypeCopyConstructor) {
    SmrtPtr<Cat> cat(new Cat());

    SmrtPtr<Animal> animal(cat);

    EXPECT_EQ(cat.Get(), animal.Get());
    EXPECT_EQ(cat.UseCount(), 2);
    EXPECT_EQ(animal.UseCount(), 2);
    EXPECT_EQ(animal->GetValue(), 2);
}

TEST(SmrtPtr, SubtypeMoveConstructor) {
    Cat* raw = new Cat();
    SmrtPtr<Cat> cat(raw);

    SmrtPtr<Animal> animal(std::move(cat));

    EXPECT_FALSE(cat);
    EXPECT_TRUE(animal);
    EXPECT_EQ(animal.Get(), raw);
    EXPECT_EQ(animal.UseCount(), 1);
}

TEST(SmrtPtr, SubtypeCopyAssignment) {
    SmrtPtr<Cat> cat(new Cat());
    SmrtPtr<Animal> animal(new Animal());

    animal = cat;

    EXPECT_EQ(cat.Get(), animal.Get());
    EXPECT_EQ(cat.UseCount(), 2);
    EXPECT_EQ(animal.UseCount(), 2);
    EXPECT_EQ(animal->GetValue(), 2);
}

TEST(SmrtPtr, SubtypeMoveAssignment) {
    Cat* raw = new Cat();

    SmrtPtr<Cat> cat(raw);
    SmrtPtr<Animal> animal(new Animal());

    animal = std::move(cat);

    EXPECT_FALSE(cat);
    EXPECT_TRUE(animal);
    EXPECT_EQ(animal.Get(), raw);
    EXPECT_EQ(animal.UseCount(), 1);
    EXPECT_EQ(animal->GetValue(), 2);
}

