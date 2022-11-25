#ifndef TYPE_LIST_TEST_H_
#define TYPE_LIST_TEST_H_

#include "type_list.h"

template <typename T>
using SizeLess4 = std::bool_constant<(sizeof(T) < 4)>;

template <typename L, typename R>
using SizeCmp = std::bool_constant<(sizeof(L) < sizeof(R))>;

template <typename Acc, typename E>
using TypeSizeAcc = std::integral_constant<size_t, Acc::value + sizeof(E)>;

void TypelistTest() {
  using LongList = TypeList<char, float, double, int, char>;
  static_assert(std::is_same_v<Map_t<LongList, std::add_pointer>,
                               TypeList<char*, float*, double*, int*, char*>>);

  static_assert(
      std::is_same_v<Filter_t<LongList, SizeLess4>, TypeList<char, char>>);

  static_assert(
      Fold_t<LongList, std::integral_constant<size_t, 0>, TypeSizeAcc>::value ==
      18);

  static_assert(std::is_same_v<Sort_t<LongList, SizeCmp>,
                               TypeList<char, char, float, int, double>>);
}

#endif  // TYPE_LIST_TEST_H_