
#include <concepts>
#include <initializer_list>
#include <new>
#include <tuple>
#include <type_traits>
#include <utility>

#include <iostream>
#include <sstream>

namespace pro
{
enum class constraint_level : int
{
  none,
  nontrivial,
  nothrow,
  trivial
};

template<class T, auto CPO = static_cast<auto>(nullptr)>
struct dispatch;
/* First instantiated from: insights.cpp:525 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct dispatch<void (std::basic_ostream<char> &), nullptr>
{
  using return_type = void;
  using argument_types = std::tuple<std::basic_ostream<char> &>;
  template<class T>
    requires (std::is_invocable_v<std::nullptr_t &, T, std::basic_ostream<char> &>)
  inline constexpr decltype(auto) operator()(T && value, std::basic_ostream<char> & args) const;
};

#endif
/* First instantiated from: insights.cpp:529 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct dispatch<double (), nullptr>
{
  using return_type = double;
  using argument_types = std::tuple<>;
  template<class T>
    requires (std::is_invocable_v<std::nullptr_t &, T>)
  inline constexpr decltype(auto) operator()(T && value) const;
};

#endif
template<class R, class ... Args, auto CPO>
struct dispatch<R (Args...), CPO>
{
  using return_type = R;
  using argument_types = std::tuple<Args...>;
  template<class T>
    requires (std::is_invocable_v<decltype(CPO) &, T, Args...>)
  inline constexpr decltype(auto) operator()(T && value, Args &&... args) const
  {
    return CPO(std::forward<T>(value), std::forward<Args>(args)... );
  }
};

template<class ... Ds>
struct facade
{
  using dispatch_types = std::tuple<Ds...>;
  using reflection_type = void;
  inline static constexpr const std::size_t maximum_size = sizeof(void *) * 2U;
  inline static constexpr const std::size_t maximum_alignment = alignof(void *);
  inline static constexpr const constraint_level minimum_copyability = constraint_level::none;
  inline static constexpr const constraint_level minimum_relocatability = constraint_level::nothrow;
  inline static constexpr const constraint_level minimum_destructibility = constraint_level::nothrow;
  // inline facade() = delete;
};

/* First instantiated from: insights.cpp:533 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade<Draw, Area>
{
  using dispatch_types = std::tuple<Draw, Area>;
  using reflection_type = void;
  inline static constexpr const std::size_t maximum_size = sizeof(void *) * 2U;
  inline static constexpr const std::size_t maximum_alignment = alignof(void *);
  inline static constexpr const constraint_level minimum_copyability = constraint_level::none;
  inline static constexpr const constraint_level minimum_relocatability = constraint_level::nothrow;
  inline static constexpr const constraint_level minimum_destructibility = constraint_level::nothrow;
  // inline facade() = delete;
};

#endif
namespace details
{
struct applicable_traits
{
  inline static constexpr const bool applicable = true;
};

struct inapplicable_traits
{
  inline static constexpr const bool applicable = false;
};

template<class T>
inline consteval bool has_copyability(constraint_level level)
{
  switch(level) {
    case constraint_level::trivial: return std::is_trivially_copy_constructible_v<T>;
    case constraint_level::nothrow: return std::is_nothrow_copy_constructible_v<T>;
    case constraint_level::nontrivial: return std::is_copy_constructible_v<T>;
    case constraint_level::none: return true;
    default: return false;
  }
}

/* First instantiated from: insights.cpp:236 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
inline consteval bool has_copyability<details::sbo_ptr<Rectangle> >(constraint_level level)
{
  switch(level) {
    case constraint_level::trivial: return std::is_trivially_copy_constructible_v<details::sbo_ptr<Rectangle> >;
    case constraint_level::nothrow: return std::is_nothrow_copy_constructible_v<details::sbo_ptr<Rectangle> >;
    case constraint_level::nontrivial: return std::is_copy_constructible_v<details::sbo_ptr<Rectangle> >;
    case constraint_level::none: return true;
    default: return false;
  }
}
#endif

template<class T>
inline consteval bool has_relocatability(constraint_level level)
{
  switch(level) {
    case constraint_level::trivial: return std::is_trivially_move_constructible_v<T> && std::is_trivially_destructible_v<T>;
    case constraint_level::nothrow: return std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>;
    case constraint_level::nontrivial: return std::is_move_constructible_v<T> && std::is_destructible_v<T>;
    case constraint_level::none: return true;
    default: return false;
  }
}

/* First instantiated from: insights.cpp:237 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
inline consteval bool has_relocatability<details::sbo_ptr<Rectangle> >(constraint_level level)
{
  switch(level) {
    case constraint_level::trivial: return std::is_trivially_move_constructible_v<details::sbo_ptr<Rectangle> > && std::is_trivially_destructible_v<details::sbo_ptr<Rectangle> >;
    case constraint_level::nothrow: return std::is_nothrow_move_constructible_v<details::sbo_ptr<Rectangle> > && std::is_nothrow_destructible_v<details::sbo_ptr<Rectangle> >;
    case constraint_level::nontrivial: return std::is_move_constructible_v<details::sbo_ptr<Rectangle> > && std::is_destructible_v<details::sbo_ptr<Rectangle> >;
    case constraint_level::none: return true;
    default: return false;
  }
}
#endif

template<class T>
inline consteval bool has_destructibility(constraint_level level)
{
  switch(level) {
    case constraint_level::trivial: return std::is_trivially_destructible_v<T>;
    case constraint_level::nothrow: return std::is_nothrow_destructible_v<T>;
    case constraint_level::nontrivial: return std::is_destructible_v<T>;
    case constraint_level::none: return true;
    default: return false;
  }
}

/* First instantiated from: insights.cpp:238 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
inline consteval bool has_destructibility<details::sbo_ptr<Rectangle> >(constraint_level level)
{
  switch(level) {
    case constraint_level::trivial: return std::is_trivially_destructible_v<details::sbo_ptr<Rectangle> >;
    case constraint_level::nothrow: return std::is_nothrow_destructible_v<details::sbo_ptr<Rectangle> >;
    case constraint_level::nontrivial: return std::is_destructible_v<details::sbo_ptr<Rectangle> >;
    case constraint_level::none: return true;
    default: return false;
  }
}
#endif

template<class P>
struct pointer_traits : public inapplicable_traits
{
};

/* First instantiated from: insights.cpp:255 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct pointer_traits<sbo_ptr<Rectangle> > : public applicable_traits
{
  using value_type = Rectangle &;
};

#endif
template<class P>
  requires (requires(const P & ptr) {
             {*ptr};
           })
struct pointer_traits<P> : public applicable_traits
{
  using value_type = decltype(*std::declval<const P &>());
};

template<class T, class ... Us>
struct contains_traits : public inapplicable_traits
{
};

/* First instantiated from: insights.cpp:91 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct contains_traits<Draw> : public inapplicable_traits
{
};

#endif
/* First instantiated from: insights.cpp:91 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct contains_traits<Draw, Draw> : public applicable_traits
{
};

#endif
/* First instantiated from: insights.cpp:185 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct contains_traits<Area> : public inapplicable_traits
{
};

#endif
/* First instantiated from: insights.cpp:185 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct contains_traits<Draw, Area> : public contains_traits<Draw>
{
};

#endif
/* First instantiated from: insights.cpp:209 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct contains_traits<Draw, Area, Draw> : public contains_traits<Draw, Draw>
{
};

#endif
/* First instantiated from: insights.cpp:209 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct contains_traits<Area, Area, Draw> : public applicable_traits
{
};

#endif
template<class T, class ... Us>
struct contains_traits<T, T, Us...> : public applicable_traits
{
};

template<class T, class U, class ... Us>
struct contains_traits<T, U, Us...> : public contains_traits<T, Us...>
{
};

template<class ... U>
struct default_traits
{
  using type = void;
};

/* First instantiated from: insights.cpp:206 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct default_traits<Area, Draw>
{
  using type = void;
};

#endif
template<class T>
struct default_traits<T>
{
  using type = T;
};

template<class D, class Args>
struct dispatch_traits_impl : public inapplicable_traits
{
};

/* First instantiated from: insights.cpp:117 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct dispatch_traits_impl<Draw, std::tuple<std::basic_ostream<char> &> > : public applicable_traits
{
  using dispatcher_type = typename Draw::return_type (*)(const char *, std::basic_ostream<char> &);
  template<class P>
  static inline typename Draw::return_type dispatcher(const char * p, std::basic_ostream<char> & __args1);

/* First instantiated from: insights.cpp:123 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  static inline typename Draw::return_type dispatcher<sbo_ptr<Rectangle> >(const char * p, std::basic_ostream<char> & __args1)
  {
    return Draw{{}}.operator()((*reinterpret_cast<const sbo_ptr<Rectangle> *>(p)).operator*(), std::forward<std::basic_ostream<char> &>(__args1));
  }
#endif

  template<>
  inline static constexpr const bool applicable_operand<Rectangle &> = requires(Rectangle & operand, std::basic_ostream<char> & __args1) {
                                                                        {Draw{}.operator()(std::forward<Rectangle &>(operand), std::forward<std::basic_ostream<char> &>(args))};
                                                                      };
};

#endif
/* First instantiated from: insights.cpp:117 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct dispatch_traits_impl<Area, std::tuple<> > : public applicable_traits
{
  using dispatcher_type = typename Area::return_type (*)(const char *);
  template<class P>
  static inline typename Area::return_type dispatcher(const char * p);

/* First instantiated from: insights.cpp:123 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  static inline typename Area::return_type dispatcher<sbo_ptr<Rectangle> >(const char * p)
  {
    return Area{{}}.operator()((*reinterpret_cast<const sbo_ptr<Rectangle> *>(p)).operator*());
  }
#endif

  template<>
  inline static constexpr const bool applicable_operand<Rectangle &> = requires(Rectangle & operand) {
                                                                        {Area{}.operator()(std::forward<Rectangle &>(operand))};
                                                                      };
};

#endif
template<class D, class ... Args>
struct dispatch_traits_impl<D, std::tuple<Args...> > : public applicable_traits
{
  using dispatcher_type = typename D::return_type (*)(const char *, Args...);
  template<class T>
  inline static constexpr const bool applicable_operand = requires(T operand, Args... args) {
                                                            {D{}(std::forward<T>(operand), std::forward<Args>(args)... )};
                                                          };
  template<class P>
  static inline typename D::return_type dispatcher(const char * p, Args... args)
  {
    return D{}(**reinterpret_cast<const P *>(p), std::forward<Args>(args)... );
  }
};

template<class D>
struct dispatch_traits : public inapplicable_traits
{
};

/* First instantiated from: insights.cpp:228 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct dispatch_traits<Draw> : public dispatch_traits_impl<Draw, std::tuple<std::basic_ostream<char> &> >
{
};

#endif
/* First instantiated from: insights.cpp:228 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct dispatch_traits<Area> : public dispatch_traits_impl<Area, std::tuple<> >
{
};

#endif
template<class D>
  requires (requires{
             typename D::return_type;
             typename D::argument_types;
             {D{}};
           })
struct dispatch_traits<D> : public dispatch_traits_impl<D, typename D::argument_types>
{
};

template<class D>
struct dispatch_meta
{
  template<class P>
  inline explicit constexpr dispatch_meta(std::in_place_type_t<P>)
      : dispatcher{dispatch_traits<D>::dispatcher}
  {
  }

  typename dispatch_traits<D>::dispatcher_type dispatcher;
};

/* First instantiated from: insights.cpp:153 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct dispatch_meta<Area>
{
  template<class P>
  inline explicit constexpr dispatch_meta(std::in_place_type_t<P>);


/* First instantiated from: insights.cpp:156 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline explicit constexpr dispatch_meta<sbo_ptr<Rectangle> >(std::in_place_type_t<sbo_ptr<Rectangle> >)
      : dispatcher{dispatch_traits_impl<Area, std::tuple<> >::dispatcher<sbo_ptr<Rectangle> >}
  {
  }
#endif


  typename dispatch_traits<Area>::dispatcher_type dispatcher;
};

#endif
/* First instantiated from: insights.cpp:153 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct dispatch_meta<Draw>
{
  template<class P>
  inline explicit constexpr dispatch_meta(std::in_place_type_t<P>);


/* First instantiated from: insights.cpp:156 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline explicit constexpr dispatch_meta<sbo_ptr<Rectangle> >(std::in_place_type_t<sbo_ptr<Rectangle> >)
      : dispatcher{dispatch_traits_impl<Draw, std::tuple<std::basic_ostream<char> &> >::dispatcher<sbo_ptr<Rectangle> >}
  {
  }
#endif


  typename dispatch_traits<Draw>::dispatcher_type dispatcher;
};

#endif
struct copy_meta
{

  class __lambda_130_15
  {
   public:
    inline auto operator()(char * self, const char * rhs) const
    {
      new (self)P{*reinterpret_cast<const P *>(rhs)};
    }

    using retType_130_15 = auto (*)(char *, const char *);
    inline constexpr operator retType_130_15 () const noexcept
    {
      return __invoke;
    };

   private:
    static inline auto __invoke(char * self, const char * rhs)
    {
      return __lambda_130_15{}.operator()(self, rhs);
    }


   public:
    // /*constexpr */ __lambda_130_15() = default;

  } __lambda_130_15{};

  template<class P>
  inline explicit constexpr copy_meta(std::in_place_type_t<P>)
      : clone{__lambda_130_15}
  {
  }

  void (*clone)(char *, const char *);
};

struct relocation_meta
{

  class __lambda_138_18
  {
   public:
    inline auto operator()(char * self, char * rhs) const
    {
      new (self)P{std::move(*reinterpret_cast<P *>(rhs))};
      /* INSIGHTS-TODO: CodeGenerator.cpp:3628 stmt: CXXPseudoDestructorExpr */();
    }

    using retType_138_18 = auto (*)(char *, char *);
    inline constexpr operator retType_138_18 () const noexcept
    {
      return __invoke;
    };

   private:
    static inline auto __invoke(char * self, char * rhs)
    {
      return __lambda_138_18{}.operator()(self, rhs);
    }


   public:
    // /*constexpr */ __lambda_138_18() = default;

  } __lambda_138_18{};

  template<class P>
  inline explicit constexpr relocation_meta(std::in_place_type_t<P>)
      : relocate{__lambda_138_18}
  {
  }



  class __lambda_138_18
  {
   public:
    inline /*constexpr */ void operator()(char * self, char * rhs) const
    {
      new (reinterpret_cast<void *>(self))pro::details::sbo_ptr<Rectangle>(std::move(*reinterpret_cast<pro::details::sbo_ptr<Rectangle> *>(rhs)));
      reinterpret_cast<pro::details::sbo_ptr<Rectangle> *>(rhs)->~sbo_ptr();
    }

    using retType_138_18 = void (*)(char *, char *);
    inline constexpr operator retType_138_18 () const noexcept
    {
      return __invoke;
    }

   private:
    static inline /*constexpr */ void __invoke(char * self, char * rhs)
    {
      __lambda_138_18{}.operator()(self, rhs);
    }


   public:
    // /*constexpr */ __lambda_138_18() = default;

  };

/* First instantiated from: insights.cpp:156 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline explicit constexpr relocation_meta<sbo_ptr<Rectangle> >(std::in_place_type_t<sbo_ptr<Rectangle> >)
      : relocate{static_cast<void (*)(char *, char *)>(__lambda_138_18{}.operator __lambda_138_18::retType_138_18())}
  {
  }
#endif


  void (*relocate)(char *, char *);
};

struct destruction_meta
{

  class __lambda_148_17
  {
   public:
    inline auto operator()(char * self) const
    {
      /* INSIGHTS-TODO: CodeGenerator.cpp:3628 stmt: CXXPseudoDestructorExpr */();
    }

    using retType_148_17 = auto (*)(char *);
    inline constexpr operator retType_148_17 () const noexcept
    {
      return __invoke;
    };

   private:
    static inline auto __invoke(char * self)
    {
      return __lambda_148_17{}.operator()(self);
    }


   public:
    // /*constexpr */ __lambda_148_17() = default;

  } __lambda_148_17{};

  template<class P>
  inline explicit constexpr destruction_meta(std::in_place_type_t<P>)
      : destroy{__lambda_148_17}
  {
  }



  class __lambda_148_17
  {
   public:
    inline /*constexpr */ void operator()(char * self) const
    {
      reinterpret_cast<pro::details::sbo_ptr<Rectangle> *>(self)->~sbo_ptr();
    }

    using retType_148_17 = void (*)(char *);
    inline constexpr operator retType_148_17 () const noexcept
    {
      return __invoke;
    }

   private:
    static inline /*constexpr */ void __invoke(char * self)
    {
      __lambda_148_17{}.operator()(self);
    }


   public:
    // /*constexpr */ __lambda_148_17() = default;

  };

/* First instantiated from: insights.cpp:156 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline explicit constexpr destruction_meta<sbo_ptr<Rectangle> >(std::in_place_type_t<sbo_ptr<Rectangle> >)
      : destroy{static_cast<void (*)(char *)>(__lambda_148_17{}.operator __lambda_148_17::retType_148_17())}
  {
  }
#endif


  void (*destroy)(char *);
};

template<class ... Ms>
struct facade_meta : public Ms...
{
  template<class P>
  inline explicit constexpr facade_meta(std::in_place_type_t<P>)
      : Ms{std::in_place_type<P>}
  {
  }

};

/* First instantiated from: insights.cpp:153 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_meta<relocation_meta, destruction_meta> : public relocation_meta, public destruction_meta
{
  template<class P>
  inline explicit constexpr facade_meta(std::in_place_type_t<P>);


/* First instantiated from: insights.cpp:156 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline explicit constexpr facade_meta<sbo_ptr<Rectangle> >(std::in_place_type_t<sbo_ptr<Rectangle> >)
      : relocation_meta(std::in_place_type_t<sbo_ptr<Rectangle> >(std::in_place_type<sbo_ptr<Rectangle> >))
        , destruction_meta(std::in_place_type_t<sbo_ptr<Rectangle> >(std::in_place_type<sbo_ptr<Rectangle> >))
  {
  }
#endif


};

#endif
/* First instantiated from: insights.cpp:243 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_meta<facade_meta<relocation_meta, destruction_meta>, dispatch_meta<Area>, dispatch_meta<Draw> > : public facade_meta<relocation_meta, destruction_meta>, public dispatch_meta<Area>, public dispatch_meta<Draw>
{
  template<class P>
  inline explicit constexpr facade_meta(std::in_place_type_t<P>);


/* First instantiated from: insights.cpp:243 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline explicit constexpr facade_meta<sbo_ptr<Rectangle> >(std::in_place_type_t<sbo_ptr<Rectangle> >)
      : facade_meta<relocation_meta, destruction_meta>(std::in_place_type_t<sbo_ptr<Rectangle> >(std::in_place_type<sbo_ptr<Rectangle> >))
        , dispatch_meta<Area>(std::in_place_type_t<sbo_ptr<Rectangle> >(std::in_place_type<sbo_ptr<Rectangle> >))
        , dispatch_meta<Draw>(std::in_place_type_t<sbo_ptr<Rectangle> >(std::in_place_type<sbo_ptr<Rectangle> >))
  {
  }
#endif


};

#endif
template<constraint_level C, class M>
struct conditional_meta_tag
{
};

template<class M, class Ms>
struct facade_meta_traits_impl;
/* First instantiated from: insights.cpp:174 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_meta_traits_impl<conditional_meta_tag<0, void>, facade_meta<> >
{
  using type = facade_meta<>;
};

#endif
/* First instantiated from: insights.cpp:174 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_meta_traits_impl<conditional_meta_tag<2, destruction_meta>, facade_meta<> >
{
  using type = facade_meta<destruction_meta>;
};

#endif
/* First instantiated from: insights.cpp:174 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_meta_traits_impl<conditional_meta_tag<2, relocation_meta>, facade_meta<destruction_meta> >
{
  using type = facade_meta<relocation_meta, destruction_meta>;
};

#endif
/* First instantiated from: insights.cpp:174 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_meta_traits_impl<conditional_meta_tag<0, copy_meta>, facade_meta<relocation_meta, destruction_meta> >
{
  using type = facade_meta<relocation_meta, destruction_meta>;
};

#endif
template<class M, class ... Ms>
struct facade_meta_traits_impl<M, facade_meta<Ms...> >
{
  using type = facade_meta<M, Ms...>;
};

template<pro::constraint_level C, class M, class ... Ms>
  requires ((C > pro::constraint_level::none) && (C < pro::constraint_level::trivial))
struct facade_meta_traits_impl<conditional_meta_tag<C, M>, facade_meta<Ms...> >
{
  using type = facade_meta<M, Ms...>;
};

template<pro::constraint_level C, class M, class ... Ms>
  requires ((C < pro::constraint_level::nontrivial) || (C > pro::constraint_level::nothrow))
struct facade_meta_traits_impl<conditional_meta_tag<C, M>, facade_meta<Ms...> >
{
  using type = facade_meta<Ms...>;
};

template<class ... Ms>
struct facade_meta_traits;
/* First instantiated from: insights.cpp:175 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_meta_traits<conditional_meta_tag<2, relocation_meta>, conditional_meta_tag<2, destruction_meta>, conditional_meta_tag<0, void> > : public facade_meta_traits_impl<conditional_meta_tag<2, relocation_meta>, facade_meta<destruction_meta> >
{
};

#endif
/* First instantiated from: insights.cpp:175 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_meta_traits<conditional_meta_tag<2, destruction_meta>, conditional_meta_tag<0, void> > : public facade_meta_traits_impl<conditional_meta_tag<2, destruction_meta>, facade_meta<> >
{
};

#endif
/* First instantiated from: insights.cpp:175 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_meta_traits<conditional_meta_tag<0, void> > : public facade_meta_traits_impl<conditional_meta_tag<0, void>, facade_meta<> >
{
};

#endif
/* First instantiated from: insights.cpp:199 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_meta_traits<conditional_meta_tag<0, copy_meta>, conditional_meta_tag<2, relocation_meta>, conditional_meta_tag<2, destruction_meta>, conditional_meta_tag<0, void> > : public facade_meta_traits_impl<conditional_meta_tag<0, copy_meta>, facade_meta<relocation_meta, destruction_meta> >
{
};

#endif
template<class M, class ... Ms>
struct facade_meta_traits<M, Ms...> : public facade_meta_traits_impl<M, typename facade_meta_traits<Ms...>::type>
{
};

template<>
struct facade_meta_traits<>
{
  using type = facade_meta<>;
};

template<class T, class U>
struct flattening_traits_impl;
/* First instantiated from: insights.cpp:187 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct flattening_traits_impl<std::tuple<>, std::tuple<Area> >
{
  using type = std::tuple<Area>;
};

#endif
/* First instantiated from: insights.cpp:187 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct flattening_traits_impl<std::tuple<>, std::tuple<Area, Draw> >
{
  using type = std::tuple<Area, Draw>;
};

#endif
/* First instantiated from: insights.cpp:192 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct flattening_traits_impl<std::tuple<Area>, std::tuple<> > : public flattening_traits_impl<std::tuple<>, std::tuple<Area> >
{
};

#endif
/* First instantiated from: insights.cpp:192 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct flattening_traits_impl<std::tuple<Draw>, std::tuple<Area> > : public flattening_traits_impl<std::tuple<>, std::tuple<Area, Draw> >
{
};

#endif
template<class T>
struct flattening_traits_impl<std::tuple<>, T>
{
  using type = T;
};

template<class T, class ... Ts, class U>
struct flattening_traits_impl<std::tuple<T, Ts...>, U> : public flattening_traits_impl<std::tuple<Ts...>, U>
{
};

template<class T, class ... Ts, class ... Us>
  requires (!contains_traits<T, Us...>::applicable)
struct flattening_traits_impl<std::tuple<T, Ts...>, std::tuple<Us...> > : public flattening_traits_impl<std::tuple<Ts...>, std::tuple<Us..., T> >
{
};

template<class T>
struct flattening_traits
{
  using type = std::tuple<T>;
};

/* First instantiated from: insights.cpp:193 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct flattening_traits<Draw>
{
  using type = std::tuple<Draw>;
};

#endif
/* First instantiated from: insights.cpp:193 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct flattening_traits<Area>
{
  using type = std::tuple<Area>;
};

#endif
/* First instantiated from: insights.cpp:194 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct flattening_traits<std::tuple<Area> > : public flattening_traits_impl<std::tuple<Area>, std::tuple<> >
{
};

#endif
/* First instantiated from: insights.cpp:224 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct flattening_traits<std::tuple<Draw, Area> > : public flattening_traits_impl<std::tuple<Draw>, std::tuple<Area> >
{
};

#endif
template<>
struct flattening_traits<std::tuple<> >
{
  using type = std::tuple<>;
};

template<class T, class ... Ts>
struct flattening_traits<std::tuple<T, Ts...> > : public flattening_traits_impl<typename flattening_traits<T>::type, typename flattening_traits<std::tuple<Ts...> >::type>
{
};

template<class F, class Ds>
struct basic_facade_traits_impl;
/* First instantiated from: insights.cpp:223 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct basic_facade_traits_impl<DrawableFacade, std::tuple<Area, Draw> > : public applicable_traits
{
  using meta_type = facade_meta_traits<conditional_meta_tag<0, copy_meta>, conditional_meta_tag<2, relocation_meta>, conditional_meta_tag<2, destruction_meta>, conditional_meta_tag<0, void> >::facade_meta<relocation_meta, destruction_meta>;
  using default_dispatch = typename default_traits<Area, Draw>::type;
  template<>
  inline static constexpr const bool has_dispatch<Draw> = applicable_traits::applicable;
  template<>
  inline static constexpr const bool has_dispatch<Area> = applicable_traits::applicable;
};

#endif
template<class F, class ... Ds>
struct basic_facade_traits_impl<F, std::tuple<Ds...> > : public applicable_traits
{
  using meta_type = typename facade_meta_traits<conditional_meta_tag<F::minimum_copyability, copy_meta>, conditional_meta_tag<F::minimum_relocatability, relocation_meta>, conditional_meta_tag<F::minimum_destructibility, destruction_meta>, conditional_meta_tag<std::is_void_v<typename F::reflection_type> ? constraint_level::none : constraint_level::nothrow, typename F::reflection_type> >::type;
  using default_dispatch = typename default_traits<Ds...>::type;
  template<class D>
  inline static constexpr const bool has_dispatch = contains_traits<D, Ds...>::applicable;
};

template<class F>
struct basic_facade_traits : public inapplicable_traits
{
};

/* First instantiated from: insights.cpp:260 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct basic_facade_traits<DrawableFacade> : public basic_facade_traits_impl<DrawableFacade, std::tuple<Area, Draw> >
{
};

#endif
template<class F>
  requires (requires{
             typename F::dispatch_types;
             typename F::reflection_type;
             typename std::integral_constant<std::size_t, F::maximum_size>;
             typename std::integral_constant<std::size_t, F::maximum_alignment>;
             typename std::integral_constant<constraint_level, F::minimum_copyability>;
             typename std::integral_constant<constraint_level, F::minimum_relocatability>;
             typename std::integral_constant<constraint_level, F::minimum_destructibility>;
           })
struct basic_facade_traits<F> : public basic_facade_traits_impl<F, typename flattening_traits<typename F::dispatch_types>::type>
{
};

template<class F, class Ds>
struct facade_traits_impl : public inapplicable_traits
{
};

/* First instantiated from: insights.cpp:245 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_traits_impl<DrawableFacade, std::tuple<Area, Draw> > : public applicable_traits
{
  using meta_type = facade_meta<facade_meta<relocation_meta, destruction_meta>, dispatch_meta<Area>, dispatch_meta<Draw> >;
  template<>
  inline static constexpr const bool applicable_pointer<sbo_ptr<Rectangle> > = ((((((sizeof(sbo_ptr<Rectangle>) <= pro::facade<Draw, Area>::maximum_size) && (alignof(sbo_ptr<Rectangle>) <= pro::facade<Draw, Area>::maximum_alignment)) && has_copyability<sbo_ptr<Rectangle> >(pro::facade<Draw, Area>::minimum_copyability)) && has_relocatability<sbo_ptr<Rectangle> >(pro::facade<Draw, Area>::minimum_relocatability)) && has_destructibility<sbo_ptr<Rectangle> >(pro::facade<Draw, Area>::minimum_destructibility)) && (dispatch_traits_impl<Area, std::tuple<> >::applicable_operand<Rectangle &> && dispatch_traits_impl<Draw, std::tuple<std::basic_ostream<char> &> >::applicable_operand<Rectangle &>)) && (std::is_void_v<void> || std::is_constructible_v<void, std::in_place_type_t<sbo_ptr<Rectangle> > >);
  template<>
  inline static constexpr const facade_meta<facade_meta<relocation_meta, destruction_meta>, dispatch_meta<Area>, dispatch_meta<Draw> > meta<sbo_ptr<Rectangle> > = facade_meta<facade_meta<relocation_meta, destruction_meta>, dispatch_meta<Area>, dispatch_meta<Draw> >{std::in_place_type_t<sbo_ptr<Rectangle> >(std::in_place_type<sbo_ptr<Rectangle> >)};
};

#endif
template<class F, class ... Ds>
  requires (dispatch_traits<Ds>::applicable && ...)
struct facade_traits_impl<F, std::tuple<Ds...> > : public applicable_traits
{
  using meta_type = facade_meta<typename basic_facade_traits<F>::meta_type, dispatch_meta<Ds>...>;
  template<class P>
  inline static constexpr const bool applicable_pointer = ((((((sizeof(P) <= F::maximum_size) && (alignof(P) <= F::maximum_alignment)) && has_copyability<P>(F::minimum_copyability)) && has_relocatability<P>(F::minimum_relocatability)) && has_destructibility<P>(F::minimum_destructibility)) && (dispatch_traits<Ds>::template applicable_operand<typename pointer_traits<P>::value_type> && ...)) && (std::is_void_v<typename F::reflection_type> || std::is_constructible_v<typename F::reflection_type, std::in_place_type_t<P> >);
  template<class P>
  inline static constexpr const meta_type meta = {std::in_place_type<P>};
};

template<class F>
struct facade_traits : public facade_traits_impl<F, typename flattening_traits<typename F::dispatch_types>::type>
{
};

/* First instantiated from: insights.cpp:445 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct facade_traits<DrawableFacade> : public facade_traits_impl<DrawableFacade, std::tuple<Area, Draw> >
{
};

#endif
template<class T, class U>
struct dependent_traits
{
  using type = T;
};

/* First instantiated from: insights.cpp:250 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct dependent_traits<facade_traits<DrawableFacade>, Draw>
{
  using type = facade_traits<DrawableFacade>;
};

#endif
/* First instantiated from: insights.cpp:250 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct dependent_traits<facade_traits<DrawableFacade>, Area>
{
  using type = facade_traits<DrawableFacade>;
};

#endif
template<class T, class U>
using dependent_t = typename dependent_traits<T, U>::type;

}template<class P, class F>
concept proxiable = ((details::pointer_traits<P>::applicable && details::basic_facade_traits<F>::applicable) && details::facade_traits<F>::applicable) && details::facade_traits<F>::template applicable_pointer<P>;

template<class F>
  requires (details::basic_facade_traits<F>::applicable)
class proxy
{
  using BasicTraits = details::basic_facade_traits<F>;
  using Traits = details::facade_traits<F>;
  template<class P, class ... Args>
  inline static constexpr const bool HasNothrowPolyConstructor = std::conditional_t<proxiable<P, F>, std::is_nothrow_constructible<P, Args...>, std::false_type>::value;
  template<class P, class ... Args>
  inline static constexpr const bool HasPolyConstructor = std::conditional_t<proxiable<P, F>, std::is_constructible<P, Args...>, std::false_type>::value;
  inline static constexpr const bool HasTrivialCopyConstructor = F::minimum_copyability == constraint_level::trivial;
  inline static constexpr const bool HasNothrowCopyConstructor = F::minimum_copyability >= constraint_level::nothrow;
  inline static constexpr const bool HasCopyConstructor = F::minimum_copyability >= constraint_level::nontrivial;
  inline static constexpr const bool HasNothrowMoveConstructor = F::minimum_relocatability >= constraint_level::nothrow;
  inline static constexpr const bool HasMoveConstructor = F::minimum_relocatability >= constraint_level::nontrivial;
  inline static constexpr const bool HasTrivialDestructor = F::minimum_destructibility == constraint_level::trivial;
  inline static constexpr const bool HasNothrowDestructor = F::minimum_destructibility >= constraint_level::nothrow;
  inline static constexpr const bool HasDestructor = F::minimum_destructibility >= constraint_level::nontrivial;
  template<class P, class ... Args>
  inline static constexpr const bool HasNothrowPolyAssignment = HasNothrowPolyConstructor<P, Args...> && HasNothrowDestructor;
  template<class P, class ... Args>
  inline static constexpr const bool HasPolyAssignment = HasPolyConstructor<P, Args...> && HasDestructor;
  inline static constexpr const bool HasTrivialCopyAssignment = HasTrivialCopyConstructor && HasTrivialDestructor;
  inline static constexpr const bool HasNothrowCopyAssignment = HasNothrowCopyConstructor && HasNothrowDestructor;
  inline static constexpr const bool HasCopyAssignment = HasNothrowCopyAssignment || ((HasCopyConstructor && HasMoveConstructor) && HasDestructor);
  inline static constexpr const bool HasNothrowMoveAssignment = HasNothrowMoveConstructor && HasNothrowDestructor;
  inline static constexpr const bool HasMoveAssignment = HasMoveConstructor && HasDestructor;

 public:
  inline proxy() noexcept
  {
    this->meta_ = nullptr;
  }

  inline proxy(std::nullptr_t) noexcept
      : proxy<F>{}
  {
  }

  inline proxy(const proxy<F> & rhs) noexcept(false) requires (!HasTrivialCopyConstructor && HasCopyConstructor)
  {
    if(rhs.meta_ != nullptr) {
      rhs.meta_->clone(this->ptr_, rhs.ptr_);
      this->meta_ = rhs.meta_;
    } else {
      this->meta_ = nullptr;
    }

  }

  inline proxy(const proxy<F> &) noexcept requires (HasTrivialCopyConstructor) = default;
  // inline proxy(const proxy<F> &) requires (!HasCopyConstructor) = delete;
  inline proxy(proxy<F> && rhs) noexcept(false) requires (HasMoveConstructor)
  {
    if(rhs.meta_ != nullptr) {
      if constexpr(F::minimum_relocatability == constraint_level::trivial) {
        memcpy(this->ptr_, rhs.ptr_, F::maximum_size);
      } else /* constexpr */ {
        rhs.meta_->relocate(this->ptr_, rhs.ptr_);
      }

      this->meta_ = rhs.meta_;
      rhs.meta_ = nullptr;
    } else {
      this->meta_ = nullptr;
    }

  }

  // inline proxy(proxy<F> &&) requires (!HasMoveConstructor) = delete;
  template<class P>
  inline proxy(P && ptr) noexcept(false) requires (HasPolyConstructor<std::decay_t<P>, P>)
  {
    initialize<std::decay_t<P> >(std::forward<P>(ptr));
  }

  template<class P, class ... Args>
  inline explicit proxy(std::in_place_type_t<P>, Args &&... args) noexcept(false) requires (HasPolyConstructor<P, Args...>)
  {
    initialize<P>(std::forward<Args>(args)... );
  }

  template<class P, class U, class ... Args>
  inline explicit proxy(std::in_place_type_t<P>, std::initializer_list<U> il, Args &&... args) noexcept(false) requires (HasPolyConstructor<P, std::initializer_list<U> &, Args...>)
  {
    initialize<P>(il, std::forward<Args>(args)... );
  }

  inline proxy<F> & operator=(std::nullptr_t) noexcept(false) requires (HasDestructor)
  {
    /* INSIGHTS-TODO: CodeGenerator.cpp:3628 stmt: CXXPseudoDestructorExpr */();
    new (this)proxy<F>{};
    return *this;
  }

  inline proxy<F> & operator=(const proxy<F> & rhs) requires (!HasNothrowCopyAssignment && HasCopyAssignment)
  {
    return *this = proxy<F>{{rhs}};
  }

  inline proxy<F> & operator=(const proxy<F> & rhs) noexcept requires (!HasTrivialCopyAssignment && HasNothrowCopyAssignment)
  {
    if(this != &rhs) {
      /* INSIGHTS-TODO: CodeGenerator.cpp:3628 stmt: CXXPseudoDestructorExpr */();
      new (this)proxy<F>{rhs};
    }

    return *this;
  }

  inline proxy<F> & operator=(const proxy<F> &) noexcept requires (HasTrivialCopyAssignment) = default;
  // inline proxy<F> & operator=(const proxy<F> &) requires (!HasCopyAssignment) = delete;
  inline proxy<F> & operator=(proxy<F> && rhs) noexcept(false) requires (HasMoveAssignment)
  {
    if constexpr(HasNothrowMoveAssignment) {
      /* INSIGHTS-TODO: CodeGenerator.cpp:3628 stmt: CXXPseudoDestructorExpr */();
    } else /* constexpr */ {
      this->reset();
    }

    new (this)proxy<F>{std::move(rhs)};
    return *this;
  }

  // inline proxy<F> & operator=(proxy<F> &&) requires (!HasMoveAssignment) = delete;
  template<class P>
  inline proxy<F> & operator=(P && ptr) noexcept requires (HasNothrowPolyAssignment<std::decay_t<P>, P>)
  {
    /* INSIGHTS-TODO: CodeGenerator.cpp:3628 stmt: CXXPseudoDestructorExpr */();
    initialize<std::decay_t<P> >(std::forward<P>(ptr));
    return *this;
  }
  template<class P>
  inline proxy<F> & operator=(P && ptr) requires (!HasNothrowPolyAssignment<std::decay_t<P>, P> && HasPolyAssignment<std::decay_t<P>, P>)
  {
    return *this = proxy<F>{{std::forward<P>(ptr)}};
  }
  inline ~proxy() noexcept(false) requires (!HasTrivialDestructor && HasDestructor)
  {
    if(this->meta_ != nullptr) {
      this->meta_->destroy(this->ptr_);
    }

  }

  inline ~proxy() requires (HasTrivialDestructor) = default;
  // inline ~proxy() requires (!HasDestructor) = delete;
  inline bool has_value() const noexcept
  {
    return this->meta_ != nullptr;
  }

  inline decltype(auto) reflect() const noexcept requires (!std::is_void_v<typename F::reflection_type>)
  {
    return static_cast<const typename F::reflection_type &>(*this->meta_);
  }

  inline void reset() noexcept(false) requires (HasDestructor)
  {
    /* INSIGHTS-TODO: CodeGenerator.cpp:3628 stmt: CXXPseudoDestructorExpr */();
    this->meta_ = nullptr;
  }

  inline void swap(proxy<F> & rhs) noexcept(false) requires (HasMoveConstructor)
  {
    if constexpr(F::minimum_relocatability == constraint_level::trivial) {
      std::swap(this->meta_, rhs.meta_);
      std::swap(this->ptr_, rhs.ptr);
    } else /* constexpr */ {
      if(this->meta_ != nullptr) {
        if(rhs.meta_ != nullptr) {
          proxy<F> temp = std::move(*this);
          new (this)proxy<F>{std::move(rhs)};
          new (&rhs)proxy<F>{std::move(temp)};
        } else {
          new (&rhs)proxy<F>{std::move(*this)};
        }

      } else {
        if(rhs.meta_ != nullptr) {
          new (this)proxy<F>{std::move(rhs)};
        }

      }

    }

  }

  friend inline void swap(proxy<F> & a, proxy<F> & b) noexcept(false)
  {
    a.swap(b);
  }
  template<class P, class ... Args>
  inline P & emplace(Args &&... args) noexcept(false) requires (HasPolyAssignment<P, Args...>)
  {
    this->reset();
    initialize<P>(std::forward<Args>(args)... );
    return *reinterpret_cast<P *>(this->ptr_);
  }
  template<class P, class U, class ... Args>
  inline P & emplace(std::initializer_list<U> il, Args &&... args) noexcept(false) requires (HasPolyAssignment<P, std::initializer_list<U> &, Args...>)
  {
    this->reset();
    initialize<P>(il, std::forward<Args>(args)... );
    return *reinterpret_cast<P *>(this->ptr_);
  }
  template<class D = typename BasicTraits::default_dispatch, class ... Args>
  inline decltype(auto) invoke(Args &&... args) const requires ((details::dependent_t<Traits, D>::applicable && BasicTraits::template has_dispatch<D>) && std::is_convertible_v<std::tuple<Args...>, typename D::argument_types>)
  {
    return static_cast<const typename Traits::meta_type *>(this->meta_)->dispatcher(this->ptr_, std::forward<Args>(args)... );
  }

 private:
  template<class P, class ... Args>
  inline void initialize(Args &&... args)
  {
    new (this->ptr_)P{std::forward<Args>(args)... };
    this->meta_ = &Traits::template meta<P>;
  }
  const typename BasicTraits::meta_type * meta_;
  alignas(F::maximum_alignment) char ptr_[F::maximum_size];
};

/* First instantiated from: insights.cpp:550 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
class proxy<DrawableFacade>
{
  using BasicTraits = details::basic_facade_traits<DrawableFacade>;
  using Traits = details::facade_traits<DrawableFacade>;
  static constexpr const bool HasTrivialCopyConstructor;
  static constexpr const bool HasNothrowCopyConstructor;
  static constexpr const bool HasCopyConstructor;
  static constexpr const bool HasNothrowMoveConstructor;
  static constexpr const bool HasMoveConstructor;
  inline static constexpr const bool HasTrivialDestructor = facade<Draw, Area>::minimum_destructibility == constraint_level::trivial;
  inline static constexpr const bool HasNothrowDestructor = facade<Draw, Area>::minimum_destructibility >= constraint_level::nothrow;
  inline static constexpr const bool HasDestructor = facade<Draw, Area>::minimum_destructibility >= constraint_level::nontrivial;
  static constexpr const bool HasTrivialCopyAssignment;
  static constexpr const bool HasNothrowCopyAssignment;
  static constexpr const bool HasCopyAssignment;
  static constexpr const bool HasNothrowMoveAssignment;
  static constexpr const bool HasMoveAssignment;

 public:
  inline proxy() noexcept;

  inline proxy(std::nullptr_t) noexcept;

  inline proxy(const proxy<DrawableFacade> & rhs) /* noexcept */ requires (!HasTrivialCopyConstructor && HasCopyConstructor);

  inline constexpr proxy(const proxy<DrawableFacade> &) noexcept requires (HasTrivialCopyConstructor) = default;
  // inline proxy(const proxy<DrawableFacade> &) requires (!HasCopyConstructor) = delete;
  inline proxy(proxy<DrawableFacade> && rhs) /* noexcept */ requires (HasMoveConstructor);

  // inline proxy(proxy<DrawableFacade> &&) requires (!HasMoveConstructor) = delete;
  template<class P>
  inline proxy(P && ptr) /* noexcept */ requires (HasPolyConstructor<std::decay_t<P>, P>);

  template<class P, class ... Args>
  inline explicit proxy(std::in_place_type_t<P>, Args &&... args) /* noexcept */ requires (HasPolyConstructor<P, Args...>);


/* First instantiated from: insights.cpp:504 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline explicit proxy<details::sbo_ptr<Rectangle>, Rectangle &>(std::in_place_type_t<details::sbo_ptr<Rectangle> >, Rectangle & args) noexcept(true) requires (HasPolyConstructor<details::sbo_ptr<Rectangle>, Rectangle &>)
  {
    this->initialize<details::sbo_ptr<Rectangle>, Rectangle &>(std::forward<Rectangle &>(args));
  }
#endif


  template<class P, class U, class ... Args>
  inline explicit proxy(std::in_place_type_t<P>, std::initializer_list<U> il, Args &&... args) /* noexcept */ requires (HasPolyConstructor<P, std::initializer_list<U> &, Args...>);

  inline proxy<DrawableFacade> & operator=(std::nullptr_t) /* noexcept */ requires (HasDestructor);

  inline proxy<DrawableFacade> & operator=(const proxy<DrawableFacade> & rhs) requires (!HasNothrowCopyAssignment && HasCopyAssignment);

  inline proxy<DrawableFacade> & operator=(const proxy<DrawableFacade> & rhs) noexcept requires (!HasTrivialCopyAssignment && HasNothrowCopyAssignment);

  inline proxy<DrawableFacade> & operator=(const proxy<DrawableFacade> &) noexcept requires (HasTrivialCopyAssignment) = default;
  // inline proxy<DrawableFacade> & operator=(const proxy<DrawableFacade> &) requires (!HasCopyAssignment) = delete;
  inline proxy<DrawableFacade> & operator=(proxy<DrawableFacade> && rhs) /* noexcept */ requires (HasMoveAssignment);

  // inline proxy<DrawableFacade> & operator=(proxy<DrawableFacade> &&) requires (!HasMoveAssignment) = delete;
  template<class P>
  inline proxy<DrawableFacade> & operator=(P && ptr) noexcept requires (HasNothrowPolyAssignment<std::decay_t<P>, P>);
  template<class P>
  inline proxy<DrawableFacade> & operator=(P && ptr) requires (!HasNothrowPolyAssignment<std::decay_t<P>, P> && HasPolyAssignment<std::decay_t<P>, P>);
  inline ~proxy() noexcept(true) requires (!HasTrivialDestructor && HasDestructor)
  {
    if(this->meta_ != nullptr) {
      static_cast<details::destruction_meta *>(this->meta_)->destroy(this->ptr_);
    }

  }

  inline ~proxy() /* noexcept */ requires (HasTrivialDestructor) = default;
  // inline ~proxy() /* noexcept */ requires (!HasDestructor) = delete;
  inline bool has_value() const noexcept;

  inline decltype(auto) reflect() const noexcept requires (!std::is_void_v<void>);

  inline void reset() /* noexcept */ requires (HasDestructor);

  inline void swap(proxy<DrawableFacade> & rhs) /* noexcept */ requires (HasMoveConstructor);

  friend inline void swap(proxy<DrawableFacade> & a, proxy<DrawableFacade> & b) /* noexcept */;
  template<class P, class ... Args>
  inline P & emplace(Args &&... args) /* noexcept */ requires (HasPolyAssignment<P, Args...>);
  template<class P, class U, class ... Args>
  inline P & emplace(std::initializer_list<U> il, Args &&... args) /* noexcept */ requires (HasPolyAssignment<P, std::initializer_list<U> &, Args...>);
  template<class D = typename BasicTraits::default_dispatch, class ... Args>
  inline decltype(auto) invoke(Args &&... args) const requires ((details::dependent_t<Traits, D>::applicable && details::basic_facade_traits<DrawableFacade>::template has_dispatch<D>) && std::is_convertible_v<std::tuple<Args...>, typename D::argument_types>);

#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline typename Draw::return_type invoke<Draw, std::basic_stringstream<char> &>(std::basic_stringstream<char> & args) const requires ((details::applicable_traits::applicable && details::basic_facade_traits_impl<DrawableFacade, std::tuple<Area, Draw> >::has_dispatch<Draw>) && std::is_convertible_v<std::tuple<std::basic_stringstream<char> &>, std::tuple<std::basic_ostream<char> &> >)
  {
    return static_cast<details::dispatch_meta<Draw> *>(static_cast<const details::facade_meta<details::facade_meta<details::relocation_meta, details::destruction_meta>, details::dispatch_meta<Area>, details::dispatch_meta<Draw> > *>(this->meta_))->dispatcher(this->ptr_, static_cast<std::basic_ostream<char>&>(std::forward<std::basic_stringstream<char> &>(args)));
  }
#endif


#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline typename Area::return_type invoke<Area>() const requires ((details::applicable_traits::applicable && details::basic_facade_traits_impl<DrawableFacade, std::tuple<Area, Draw> >::has_dispatch<Area>) && std::is_convertible_v<std::tuple<>, std::tuple<> >)
  {
    return static_cast<details::dispatch_meta<Area> *>(static_cast<const details::facade_meta<details::facade_meta<details::relocation_meta, details::destruction_meta>, details::dispatch_meta<Area>, details::dispatch_meta<Draw> > *>(this->meta_))->dispatcher(this->ptr_);
  }
#endif


 private:
  template<class P, class ... Args>
  inline void initialize(Args &&... args);

/* First instantiated from: insights.cpp:342 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline void initialize<details::sbo_ptr<Rectangle>, Rectangle &>(Rectangle & args)
  {
    new (reinterpret_cast<void *>(this->ptr_))details::sbo_ptr<Rectangle>(std::forward<Rectangle &>(args));
    this->meta_ = static_cast<const details::facade_meta<details::relocation_meta, details::destruction_meta> *>(&details::facade_traits_impl<DrawableFacade, std::tuple<Area, Draw> >::meta<details::sbo_ptr<Rectangle> >);
  }
#endif

  const details::facade_meta<details::relocation_meta, details::destruction_meta> * meta_;
  alignas(struct DrawableFacade::maximum_alignment) char ptr_[16];
  template<>
  inline static constexpr const bool HasPolyConstructor<details::sbo_ptr<Rectangle>, Rectangle &> = std::integral_constant<bool, true>::value;
  template<>
  inline static constexpr const bool HasNothrowPolyConstructor<details::sbo_ptr<Rectangle>, Rectangle &> = std::integral_constant<bool, true>::value;
};

#endif
namespace details
{
template<class T>
class sbo_ptr
{

 public:
  template<class ... Args>
  inline sbo_ptr(Args &&... args) noexcept(false) requires (std::is_constructible_v<T, Args...>)
      : value_{std::forward<Args>(args)... }
  {
  }

  inline sbo_ptr(const sbo_ptr<T> &) noexcept(false) = default;
  inline sbo_ptr(sbo_ptr<T> &&) noexcept(false) = default;
  inline T & operator*() const
  {
    return this->value_;
  }


 private:
  mutable T value_;
};

/* First instantiated from: insights.cpp:83 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
class sbo_ptr<Rectangle>
{

 public:
  template<class ... Args>
  inline sbo_ptr(Args &&... args) /* noexcept */ requires (std::is_constructible_v<Rectangle, Args...>);


#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline sbo_ptr<const sbo_ptr<Rectangle> &>(const sbo_ptr<Rectangle> & args) /* noexcept */ requires (std::is_constructible_v<Rectangle, const sbo_ptr<Rectangle> &>);
#endif



#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline sbo_ptr<sbo_ptr<Rectangle> >(sbo_ptr<Rectangle> && __args0) /* noexcept */ requires (std::is_constructible_v<Rectangle, sbo_ptr<Rectangle> >);
#endif



/* First instantiated from: insights.cpp:457 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline sbo_ptr<Rectangle &>(Rectangle & args) noexcept(true) requires (std::is_constructible_v<Rectangle, Rectangle &>)
      : value_{Rectangle(std::forward<Rectangle &>(args))}
  {
  }
#endif


  inline constexpr sbo_ptr(const sbo_ptr<Rectangle> &) noexcept(true) = default;
  inline constexpr sbo_ptr(sbo_ptr<Rectangle> &&) noexcept(true) = default;
  inline Rectangle & operator*() const
  {
    return this->value_;
  }


 private:
  mutable Rectangle value_;
 public:
  // inline sbo_ptr<Rectangle> & operator=(const sbo_ptr<Rectangle> &) /* noexcept */ = delete;
};

#endif
template<class T>
class deep_ptr
{

 public:
  template<class ... Args>
  inline deep_ptr(Args &&... args) requires (std::is_constructible_v<T, Args...>)
      : ptr_{new T{std::forward<Args>(args)... }}
  {
  }

  inline deep_ptr(const deep_ptr<T> & rhs) requires (std::is_copy_constructible_v<T>)
      : ptr_{rhs.ptr_ == nullptr ? nullptr : new T{*rhs}}
  {
  }

  inline deep_ptr(deep_ptr<T> && rhs) noexcept
      : ptr_{rhs.ptr_}
  {
    rhs.ptr_ = nullptr;
  }

  inline ~deep_ptr() noexcept
  {
    delete this->ptr_;
  }

  inline T & operator*() const
  {
    return *this->ptr_;
  }


 private:
  T * ptr_;
};

template<class F, class T, class ... Args>
proxy<F> make_proxy_impl(Args &&... args)
{
  return proxy<F>{{std::in_place_type<std::conditional_t<proxiable<sbo_ptr<T>, F>, sbo_ptr<T>, deep_ptr<T> > >, std::forward<Args>(args)... }};
}

/* First instantiated from: insights.cpp:519 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
proxy<DrawableFacade> make_proxy_impl<DrawableFacade, Rectangle, Rectangle &>(Rectangle & args)
{
  return proxy<DrawableFacade>{std::in_place_type_t<details::sbo_ptr<Rectangle> >(std::in_place_type<details::sbo_ptr<Rectangle> >), std::forward<Rectangle &>(args)};
}
#endif


}template<class F, class T, class ... Args>
proxy<F> make_proxy(Args &&... args)
{
  return details::make_proxy_impl<F, T>(std::forward<Args>(args)... );
}
template<class F, class T, class U, class ... Args>
proxy<F> make_proxy(std::initializer_list<U> il, Args &&... args)
{
  return details::make_proxy_impl<F, T>(il, std::forward<Args>(args)... );
}
template<class F, class T>
proxy<F> make_proxy(T && value)
{
  return details::make_proxy_impl<F, std::decay_t<T> >(std::forward<T>(value));
}

/* First instantiated from: insights.cpp:563 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
proxy<DrawableFacade> make_proxy<DrawableFacade, Rectangle &>(Rectangle & value)
{
  return details::make_proxy_impl<DrawableFacade, std::decay_t<Rectangle &> >(std::forward<Rectangle &>(value));
}
#endif


}  // namespace pro

// Abstraction
struct Draw : public pro::dispatch<void (std::basic_ostream<char> &), nullptr>
{
  template<class T>
  inline void operator()(const T & self, std::basic_ostream<char> & out)
  {
    self.Draw(out);
  }

/* First instantiated from: insights.cpp:107 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline void operator()<Rectangle>(const Rectangle & self, std::basic_ostream<char> & out)
  {
    self.Draw(out);
  }
#endif

};


struct Area : public pro::dispatch<double (), nullptr>
{
  template<class T>
  inline double operator()(const T & self)
  {
    return self.Area();
  }

/* First instantiated from: insights.cpp:107 */
#ifdef INSIGHTS_USE_TEMPLATE
  template<>
  inline double operator()<Rectangle>(const Rectangle & self)
  {
    return self.Area();
  }
#endif

};


struct DrawableFacade : public pro::facade<Draw, Area>
{
};



// Implementation
class Rectangle
{

 public:
  inline void Draw(std::basic_ostream<char> & out) const
  {
    std::operator<<(std::operator<<(std::operator<<(out, "{Rectangle: width = ").operator<<(this->width_), ", height = ").operator<<(this->height_), "}");
  }

  inline void SetWidth(double width)
  {
    this->width_ = width;
  }

  inline void SetHeight(double height)
  {
    this->height_ = height;
  }

  inline double Area() const
  {
    return this->width_ * this->height_;
  }


 private:
  double width_;
  double height_;
 public:
  // inline constexpr Rectangle() noexcept = default;
  // inline constexpr Rectangle(const Rectangle &) noexcept = default;
  // inline constexpr Rectangle(Rectangle &&) noexcept = default;
};



// Client - Consumer
std::basic_string<char, std::char_traits<char>, std::allocator<char> > PrintDrawableToString(pro::proxy<DrawableFacade> p)
{
  std::basic_stringstream<char> result = std::basic_stringstream<char>();
  std::operator<<(static_cast<std::basic_ostream<char>&>(result), "shape = ");
  p.invoke<Draw, std::basic_stringstream<char> &>(result);
  std::operator<<(static_cast<std::basic_ostream<char>&>(result), ", area = ").operator<<(p.invoke<Area>());
  return std::move(result).str();
}


// Client - Producer
pro::proxy<DrawableFacade> CreateRectangleAsDrawable(int width, int height)
{
  class Rectangle rect = class Rectangle();
  rect.SetWidth(static_cast<double>(width));
  rect.SetHeight(static_cast<double>(height));
  return pro::make_proxy<DrawableFacade>(rect);
}


//int main()
//{
//  PrintDrawableToString(CreateRectangleAsDrawable(5, 10));
//  return 0;
//}







