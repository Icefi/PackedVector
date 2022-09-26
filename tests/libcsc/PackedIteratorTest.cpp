#include <packed_vector/packed_vector.hpp>

#include <gtest/gtest.h>

#include <algorithm>

TEST(PackedIteratorTest, FindTest) {
  libcsc::PackedVector<unsigned short, 5> vec(5, 17);
  vec.assign(1, 17);
  vec.assign(2, 19);
  vec.assign(3, 20);
  vec.assign(4, 21);

  auto it1 = std::find(vec.begin(), vec.end(), 19);
  auto it2 = std::find_if(
      vec.begin(), vec.end(), [](unsigned short i) { return i % 2 == 0; });

  EXPECT_EQ(*it1, 19);
  EXPECT_EQ(*it2, 20);
}

TEST(PackedIteratorTest, IncludesTest) {
  libcsc::PackedVector<unsigned short, 5> vec1(5, 17);
  vec1.assign(1, 17);
  vec1.assign(2, 19);
  vec1.assign(3, 22);
  vec1.assign(4, 21);

  libcsc::PackedVector<unsigned short, 5> vec2(2, 0);
  vec2.assign(0, 19);
  vec2.assign(1, 22);

  auto ret = std::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());

  EXPECT_EQ(ret, true);
}

TEST(PackedIteratorTest, EqualTest) {
  libcsc::PackedVector<unsigned short, 5> vec1(5, 17);
  vec1.assign(1, 17);
  vec1.assign(2, 19);
  vec1.assign(3, 22);
  vec1.assign(4, 21);

  libcsc::PackedVector<unsigned short, 5> vec2(3, 0);
  vec2.assign(0, 19);
  vec2.assign(1, 22);
  vec2.assign(2, 21);

  auto ret = std::equal(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());

  EXPECT_EQ(ret, false);
}

TEST(PackedIteratorTest, AllAnyNoneOfTest) {
  libcsc::PackedVector<unsigned short, 5> vec(5, 17);
  vec.assign(1, 17);
  vec.assign(2, 19);
  vec.assign(3, 22);
  vec.assign(4, 23);

  auto all_res = std::all_of(
      vec.begin(), vec.end(), [](unsigned short i) { return i > 15; });
  auto any_res = std::any_of(
      vec.begin(), vec.end(), [](unsigned short i) { return i % 2 == 0; });
  auto none_res = std::none_of(
      vec.begin(), vec.end(), [](unsigned short i) { return i % 3 == 0; });

  EXPECT_EQ(all_res, true);
  EXPECT_EQ(any_res, true);
  EXPECT_EQ(none_res, true);
}

TEST(PackedIteratorTest, CountTest) {
  libcsc::PackedVector<unsigned short, 5> vec(5, 17);
  vec.assign(1, 17);
  vec.assign(2, 19);
  vec.assign(3, 22);
  vec.assign(4, 23);

  auto res1 = std::count(vec.begin(), vec.end(), 17);
  auto res2 = std::count_if(
      vec.begin(), vec.end(), [](unsigned short i) { return i % 2 == 0; });

  EXPECT_EQ(res1, 2);
  EXPECT_EQ(res2, 1);
}
