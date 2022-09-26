#include <packed_vector/packed_vector.hpp>

#include <gtest/gtest.h>

TEST(PackedVectorTest, Constructor) {
  libcsc::PackedVector<unsigned short, 5> vec(5, 17);

  vec.assign(1, 3);
  vec.assign(3, 42);

  /* Note:
   *     42 in binary is 101010, but we take just 5 bits,
   *     so assigned value is 10 (01010 in binary)
   */

  EXPECT_EQ(vec[0], 17);
  EXPECT_EQ(vec[1], 3);
  EXPECT_EQ(vec[2], 17);
  EXPECT_EQ(vec[3], 10);
  EXPECT_EQ(vec[4], 17);

  EXPECT_EQ(vec.capacity(), 5);
  EXPECT_EQ(vec.size(), 5);
}

TEST(PackedVectorTest, DefaultConstructor) {
  libcsc::PackedVector<unsigned short, 5> vec;

  EXPECT_EQ(vec.size(), 0);
  EXPECT_EQ(vec.capacity(), 0);

  vec.resize(3);

  vec.assign(2, 5);

  EXPECT_EQ(vec.at(0), 0);
  EXPECT_EQ(vec.at(1), 0);
  EXPECT_EQ(vec.at(2), 5);

  EXPECT_EQ(vec.size(), 3);
  EXPECT_EQ(vec.capacity(), 3);
}

TEST(PackedVectorTest, CopyConstructor) {
  libcsc::PackedVector<unsigned short, 7> vec1(3, 54);

  auto vec2 = vec1; /* Called copy constructor */

  vec1.assign(2, 17);

  EXPECT_EQ(vec1.at(2), 17);
  EXPECT_EQ(vec2.at(2), 54);
}

TEST(PackedVectorTest, CopyAssignment) {
  libcsc::PackedVector<unsigned short, 7> vec1(3, 5);
  libcsc::PackedVector<unsigned short, 7> vec2(4, 3);

  vec2 = vec1; /* Called copy assignment '=' */

  vec1.assign(0, 8);

  EXPECT_EQ(vec1.size(), 3);
  EXPECT_EQ(vec2.size(), 3);

  EXPECT_EQ(vec1.capacity(), 3);
  EXPECT_EQ(vec2.capacity(), 3);

  EXPECT_EQ(vec1.at(0), 8);
  EXPECT_EQ(vec2.at(0), 5);
}

TEST(PackedVectorTest, MoveConstructor) {
  libcsc::PackedVector<unsigned short, 7> vec1(4, 100);

  auto vec2 = std::move(vec1);

  EXPECT_EQ(vec1.size(), 0);
  EXPECT_EQ(vec2.size(), 4);

  EXPECT_EQ(vec1.capacity(), 0);
  EXPECT_EQ(vec2.capacity(), 4);

  EXPECT_EQ(vec2.at(3), 100);
}

TEST(PackedVectorTest, MoveAssignment) {
  libcsc::PackedVector<unsigned short, 7> vec1(4, 100);
  libcsc::PackedVector<unsigned short, 7> vec2(8, 50);

  vec2 = std::move(vec1);

  EXPECT_EQ(vec1.size(), 0);
  EXPECT_EQ(vec2.size(), 4);

  EXPECT_EQ(vec1.capacity(), 0);
  EXPECT_EQ(vec2.capacity(), 4);

  EXPECT_EQ(vec2.at(3), 100);
}

TEST(PackedVectorTest, ModifiersTest1) {
  libcsc::PackedVector<unsigned short, 7> vec1(4, 100);

  vec1.push_back(45);
  vec1.insert(2, 25);

  EXPECT_EQ(vec1.at(2), 100);
  EXPECT_EQ(vec1.at(3), 25);
  EXPECT_EQ(vec1.pop_back(), 45);
  EXPECT_EQ(vec1.pop_back(), 100);

  EXPECT_EQ(vec1.size(), 4);
  EXPECT_EQ(vec1.capacity(), 6);

  vec1.shrink_to_fit();

  EXPECT_EQ(vec1.size(), 4);
  EXPECT_EQ(vec1.capacity(), 4);

  EXPECT_EQ(vec1.at(0), 100);
  EXPECT_EQ(vec1.at(1), 100);
  EXPECT_EQ(vec1.at(2), 100);
  EXPECT_EQ(vec1.at(3), 25);
}
