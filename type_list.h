#ifndef TYPE_LIST_H_
#define TYPE_LIST_H_

#include <concepts>
#include <type_traits>

template <typename T>
struct Return {
  using type = T;
};

template <typename... Ts>
struct TypeList {
  struct IsTypeList {};                          // 标记，用于定义concept
  using type = TypeList;                         // 约定type输出类型
  constexpr static size_t size = sizeof...(Ts);  // 列表长度

  template <typename... T>
  using append = TypeList<Ts..., T...>;
  template <typename... T>
  using prepend = TypeList<T..., Ts...>;
  template <template <typename...> typename T>
  using to = T<Ts...>;
};

template <typename TypeList>
concept TL = requires {
               typename TypeList::IsTypeList;
               typename TypeList::type;
             };

// define map function
template <TL In, template <typename> class F>
struct Map;

template <template <typename> class F, typename... Ts>
struct Map<TypeList<Ts...>, F> : TypeList<typename F<Ts>::type...> {};

template <TL In, template <typename> class F>
using Map_t = typename Map<In, F>::type;

// define filter function
template <TL In, template <typename> class P, TL Out = TypeList<>>
struct Filter : Out {};

template <template <typename> class P, TL Out, typename H, typename... Ts>
struct Filter<TypeList<H, Ts...>, P, Out>
    : std::conditional_t<
          P<H>::value,
          Filter<TypeList<Ts...>, P, typename Out::template append<H>>,
          Filter<TypeList<Ts...>, P, Out>> {};

template <TL In, template <typename> class P>
using Filter_t = typename Filter<In, P>::type;

// define fold function
template <TL In, typename Init, template <typename, typename> class Op>
struct Fold : Return<Init> {};

template <typename Acc,
          template <typename, typename>
          class Op,
          typename H,
          typename... Ts>
struct Fold<TypeList<H, Ts...>, Acc, Op>
    : Fold<TypeList<Ts...>, typename Op<Acc, H>::type, Op> {};

template <TL In, typename Init, template <typename, typename> class Op>
using Fold_t = typename Fold<In, Init, Op>::type;

// define algorithm Concat
template <TL... In>
struct Concat;

template <TL... In>
using Concat_t = typename Concat<In...>::type;

template <>
struct Concat<> : TypeList<> {};

template <TL In>
struct Concat<In> : In {};

template <TL In, TL In2, TL... Rest>
struct Concat<In, In2, Rest...> : Concat<Concat_t<In, In2>, Rest...> {};

template <typename... Ts, typename... Ts2>
struct Concat<TypeList<Ts...>, TypeList<Ts2...>> : TypeList<Ts..., Ts2...> {};

// define algorithm Elem
template <TL In, typename E>
struct Elem : std::false_type {};

template <typename... Ts, typename E>
struct Elem<TypeList<Ts...>, E>
    : std::bool_constant<(false || ... || std::is_same_v<E, Ts>)> {};

template <TL In, typename E>
inline constexpr bool Elem_v = Elem<In, E>::value;

// define algorithm Unique
template <TL In>
class Unique {
  template <TL Acc, typename E>
  using Append =
      std::conditional_t<Elem_v<Acc, E>, Acc, typename Acc::template append<E>>;

 public:
  using type = Fold_t<In, TypeList<>, Append>;
};

// define algorithm Partition
template <TL In, template <typename> typename P>
class Partition {
  template <typename Arg>
  using NotP = std::bool_constant<!P<Arg>::value>;

 public:
  struct type {
    using Satisfied = Filter_t<In, P>;
    using Rest = Filter_t<In, NotP>;
  };
};

template <TL In, template <typename> typename P>
using Partition_t = typename Partition<In, P>::type;

// define algorithm Sort
template <TL In, template <typename, typename> class Cmp>
struct Sort : TypeList<> {};

template <template <typename, typename> class Cmp,
          typename Pivot,
          typename... Ts>
class Sort<TypeList<Pivot, Ts...>, Cmp> {
  template <typename E>
  using LT = Cmp<E, Pivot>;

  using P = Partition_t<TypeList<Ts...>, LT>;

  using SmallerSorted = typename Sort<typename P::Satisfied, Cmp>::type;
  using BiggerSorted = typename Sort<typename P::Rest, Cmp>::type;

 public:
  using type =
      Concat_t<typename SmallerSorted::template append<Pivot>, BiggerSorted>;
};

template <TL In, template <typename, typename> class Cmp>
using Sort_t = typename Sort<In, Cmp>::type;

#endif  // TYPE_LIST_H_