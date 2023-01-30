//
// Created by chenb on 2023/1/30.
//

// https://godbolt.org/z/P1GPboK8E

#ifndef CLION_RUST_MACRO_RULE_H
#define CLION_RUST_MACRO_RULE_H

#include <tuple>
#include <string_view>
#include <optional>
#include <array>

#include "compile_parse.h"

namespace meta::parse {
struct Identifier {
  size_t id;
};

template<typename Arg0, typename Arg1>
struct Some {
  Arg0 arg0;
  Arg1 arg1;
};

template<typename Arg0, typename Arg1>
Some(Arg0, Arg1) -> Some<Arg0, Arg1>;

struct None {};

template <typename... T>
struct List {
  std::tuple<T...> elements;
};

template <typename... T>
List(std::tuple<T...>) -> List<T...>;

template <typename... T>
struct Group {
  std::tuple<T...> elements;
};

template <typename... T>
Group(std::tuple<T...>) -> Group<T...>;

template<enum TokenType type>
class token {
 public:
  template<TokenStream stream>
  consteval static auto compile() {
    if constexpr ((*stream).type == type) {
      return Some{*stream, stream.next()};
    } else {
      return None{};
    }
  }
};

template<uint32_t id>
struct ident {
  template<TokenStream stream>
  static consteval auto compile() {
    if constexpr ((*stream).type == TokenType::Identifier && (*stream).id == id) {
      return Some{Identifier{(*stream).id}, stream.next()};
    } else {
      return None{};
    }
  }
};

consteval auto __optional(auto const&) {
  return false;
}

template<typename T>
consteval auto __optional(std::optional<T> const&) {
  return true;
}

template<TokenStream stream, typename Head, typename... Tail>
consteval auto __group(auto... args) {
  constexpr auto opt = Head::template compile<stream>();
  if constexpr (__optional(opt)) {
    constexpr auto r = opt.value();
    if constexpr (std::same_as<std::decay_t<decltype(r)>, None>) {
      return __group<stream, Tail...>(std::forward<decltype(args)>(args)...);
    } else if constexpr (sizeof...(Tail) > 0) {
      return __group<r.arg1, Tail...>(std::forward<decltype(args)>(args)..., r.arg0);
    } else {
      return Some{Group{std::tuple{std::forward<decltype(args)>(args)..., r.arg0}}, r.arg1};
    }
  } else {
    constexpr auto r = opt;
    if constexpr (std::same_as<std::decay_t<decltype(r)>, None>) {
      return None{};
    } else if constexpr (sizeof...(Tail) > 0) {
      return __group<r.arg1, Tail...>(std::forward<decltype(args)>(args)..., r.arg0);
    } else {
      return Some{Group{std::tuple{std::forward<decltype(args)>(args)..., r.arg0}}, r.arg1};
    }
  }
}

template<TokenStream stream, typename T>
consteval auto __list(auto... args) {
  constexpr auto r = T::template compile<stream>();
  if constexpr (std::same_as<std::decay_t<decltype(r)>, None>) {
    return Some{List{std::tuple{std::forward<decltype(args)>(args)...}}, stream};
  } else {
    return __list<r.arg1, T>(std::forward<decltype(args)>(args)..., r.arg0);
  }
}

template<TokenStream stream, typename Head, typename... Tail>
consteval auto __one_of() {
  constexpr auto r = Head::template compile<stream>();
  if constexpr (std::same_as<std::decay_t<decltype(r)>, None>) {
    if constexpr (sizeof...(Tail) > 0) {
      return __one_of<stream, Tail...>();
    } else {
      return None{};
    }
  } else {
    return r;
  }
}

template<TokenStream stream, typename First, typename Next>
constexpr auto __chain() {
  constexpr auto lhs = First::template compile<stream>();
  if constexpr (std::same_as<std::decay_t<decltype(lhs)>, None>) {
    return None{};
  } else {
    constexpr auto rhs = __chain<lhs.arg1, Next, Next>();
    if constexpr (std::same_as<std::decay_t<decltype(rhs)>, None>) {
      return lhs;
    } else {
      return Some{Some{lhs.arg0, rhs.arg0}, rhs.arg1};
    }
  }
}

template<typename... T>
class group {
 public:
  using base = group;

  template<TokenStream stream>
  consteval static auto compile() {
    return __group<stream, T...>();
  }
};

template<typename First, typename Next>
class chain {
 public:
  template<TokenStream stream>
  consteval static auto compile() {
    return __chain<stream, First, Next>();
  }
};

template<typename T>
class optional {
 public:
  template<TokenStream stream>
  consteval static auto compile() {
    return std::make_optional(T::template compile<stream>());
  }
};

template<typename T>
class list {
 public:
  template<TokenStream stream>
  consteval static auto compile() {
    return __list<stream, T>();
  }
};

template<typename>
class reference {};

template<typename... T>
class one_of {
 public:
  template<TokenStream stream>
  consteval static auto compile() {
    return __one_of<stream, T...>();
  }
};

enum class Result {
  Success,
  Error
};

template<typename T, const_string chars>
consteval auto compile() {
  constexpr auto r = T::template compile<TokenStream::of(chars.str())>();
  constexpr auto none = std::same_as<std::decay_t<decltype(r)>, None>;
  if constexpr (none) {
    return None{};
  } else {
    constexpr auto eos = (*r.arg1).type == TokenType::End;
    if constexpr (eos) {
      return r.arg0;
    } else {
      return None{};
    }
  }
}
}

#define apply_rules(rules, ...) decltype(meta::parse::compile<rules, const_string{#__VA_ARGS__}>())
#define try_apply_rules(rules, ...) !std::same_as<apply_rules(rules, __VA_ARGS__), meta::parse::None>

#define if_some(r) if constexpr(!std::same_as<std::decay_t<decltype(r)>, meta::parse::None>)

class unary;
class primitive;
class expression;

struct primitive : meta::parse::one_of<
                       meta::parse::group<
                           meta::parse::token<TokenType::LeftParen>,
                           expression,
                           meta::parse::token<TokenType::RightParen>
                           >,
                       meta::parse::token<TokenType::Identifier>,
                       meta::parse::token<TokenType::Number>
                       > {};

struct unary : meta::parse::one_of<
                   meta::parse::group<meta::parse::token<TokenType::Plus>, unary>,
                   meta::parse::group<meta::parse::token<TokenType::Minus>, unary>,
                   meta::parse::group<meta::parse::token<TokenType::Not>, unary>,
                   meta::parse::group<meta::parse::token<TokenType::Tilde>, unary>,
                   primitive
                   > {};

struct multiplication : meta::parse::chain<
                            unary,
                            meta::parse::one_of<
                                meta::parse::group<meta::parse::token<TokenType::Asterisk>, unary>,
                                meta::parse::group<meta::parse::token<TokenType::Slash>, unary>
                                >
                            > {};

struct addition : meta::parse::chain<
                      multiplication,
                      meta::parse::one_of<
                          meta::parse::group<meta::parse::token<TokenType::Plus>, multiplication>,
                          meta::parse::group<meta::parse::token<TokenType::Minus>, multiplication>
                          >
                      > {};

struct comparison : meta::parse::chain<
                        addition,
                        meta::parse::one_of<
                            meta::parse::group<meta::parse::token<TokenType::LessThan>, addition>,
                            meta::parse::group<meta::parse::token<TokenType::LessEqual>, addition>,
                            meta::parse::group<meta::parse::token<TokenType::GreaterThan>, addition>,
                            meta::parse::group<meta::parse::token<TokenType::GreaterEqual>, addition>,
                            meta::parse::group<meta::parse::token<TokenType::GreaterEqual>, addition>
                            >
                        > {};
struct expression : comparison {};

struct macro_rules {
  struct pattern;

  struct identifier {
    template<TokenStream stream>
    static consteval auto compile() {
      if constexpr ((*stream).type == TokenType::Identifier) {
        return meta::parse::Some{meta::parse::ident<(*stream).id>{}, stream.next()};
      } else {
        return meta::parse::None{};
      }
    }
  };

  template<enum TokenType type>
  struct token : meta::parse::token<type> {
    template<TokenStream stream>
    consteval static auto compile() {
      constexpr auto r = meta::parse::token<type>::template compile<stream>();
      if_some(r) {
        return meta::parse::Some{meta::parse::token<type>{}, r.arg1};
      } else {
        return meta::parse::None{};
      }
    }
  };

  struct ident : meta::parse::group<
                     meta::parse::token<TokenType::Dollar>,
                     identifier,
                     meta::parse::token<TokenType::Colon>,
                     meta::parse::ident<SourceStream::fnv1a("ident")>
                     > {
    template<TokenStream stream>
    consteval static auto compile() {
      constexpr auto r = base::compile<stream>();
      if_some(r) {
        // todo: bind to name
        return meta::parse::Some{meta::parse::token<TokenType::Identifier>{}, r.arg1};
      } else {
        return meta::parse::None{};
      }
    }
  };

  struct expr : meta::parse::group<
                    meta::parse::token<TokenType::Dollar>,
                    identifier,
                    meta::parse::token<TokenType::Colon>,
                    meta::parse::ident<SourceStream::fnv1a("expr")>
                    > {
    template<TokenStream stream>
    consteval static auto compile() {
      constexpr auto r = base::compile<stream>();
      if_some(r) {
        // todo: bind to name
        return meta::parse::Some{expression{}, r.arg1};
      } else {
        return meta::parse::None{};
      }
    }
  };

  struct group_rep : meta::parse::group<
                         meta::parse::token<TokenType::Dollar>,
                         meta::parse::token<TokenType::LeftParen>,
                         pattern,
                         meta::parse::token<TokenType::RightParen>,
                         meta::parse::token<TokenType::Asterisk>
                         > {
    template<TokenStream stream>
    consteval static auto compile() {
      constexpr auto r = base::compile<stream>();
      if_some(r) {
        using T = std::decay_t<decltype(std::get<2>(r.arg0.elements))>;
        return meta::parse::Some{meta::parse::optional<meta::parse::list<T>>{}, r.arg1};
      } else {
        return meta::parse::None{};
      }
    }
  };

  template<typename T>
  struct list : meta::parse::list<T> {
    template<TokenStream stream>
    consteval static auto compile() {
      constexpr auto r = meta::parse::list<T>::template compile<stream>();
      if_some(r) {
        return meta::parse::Some{decompose(r.arg0), r.arg1};
      } else {
        return meta::parse::None{};
      }
    }

    template<typename First, typename... U>
    consteval static auto decompose(const meta::parse::List<First, U...>& list) {
      if constexpr (sizeof...(U) == 0) {
        return First{};
      } else {
        return meta::parse::group<First, U...>{};
      }
    }
  };

  template<typename... T>
  struct sequence : meta::parse::group<T...> {
    template<TokenStream stream>
    consteval static auto compile() {
      constexpr auto r = meta::parse::group<T...>::template compile<stream>();
      if_some(r) {
        return meta::parse::Some{decompose(r.arg0), r.arg1};
      } else {
        return meta::parse::None{};
      }
    }

    template<typename... U>
    consteval static auto decompose(const meta::parse::Group<U...>& list) {
      return meta::parse::group<U...>{};
    }
  };

  struct pattern : list<meta::parse::one_of<
                       group_rep,
                       ident,
                       identifier,
                       expr,
                       sequence<
                           token<TokenType::LeftCurly>,
                           pattern,
                           token<TokenType::RightCurly>
                           >,
                       sequence<
                           token<TokenType::LeftParen>,
                           pattern,
                           token<TokenType::RightParen>
                           >,
                       // todo: any_token ?
                       token<TokenType::Colon>,
                       token<TokenType::Comma>,
                       token<TokenType::Semicolon>,
                       token<TokenType::Arrow>,
                       token<TokenType::Plus>,
                       token<TokenType::Minus>,
                       token<TokenType::Asterisk>,
                       token<TokenType::Slash>,
                       token<TokenType::Tilde>,
                       token<TokenType::Not>,
                       token<TokenType::Equal>,
                       token<TokenType::LessThan>,
                       token<TokenType::GreaterThan>,
                       token<TokenType::LessEqual>,
                       token<TokenType::GreaterEqual>
                       >> {};

  template<TokenStream stream>
  consteval static auto compile() {
    return pattern::compile<stream>();
  }
};

using struct_parser = apply_rules(macro_rules,
    struct $struct_name:ident {
      $($field_name:ident : $field_type:ident;)*
    }
);

static_assert(std::same_as<struct_parser, meta::parse::group<
                                              meta::parse::ident<SourceStream::fnv1a("struct")>,
                                              meta::parse::token<TokenType::Identifier>,
                                              meta::parse::group<
                                                  meta::parse::token<TokenType::LeftCurly>,
                                                  meta::parse::optional<meta::parse::list<meta::parse::group<
                                                      meta::parse::token<TokenType::Identifier>,
                                                      meta::parse::token<TokenType::Colon>,
                                                      meta::parse::token<TokenType::Identifier>,
                                                      meta::parse::token<TokenType::Semicolon>
                                                      >>>,
                                                  meta::parse::token<TokenType::RightCurly>
                                                  >
                                              >>);

static_assert(!try_apply_rules(struct_parser,
    struct name { name: ; }
    ));

static_assert(try_apply_rules(struct_parser,
    struct name { name: i32; name: i32; }
    ));

using function_parser = apply_rules(macro_rules,
                                    ($($struct_name:ident)*) -> $expr:expr
);

static_assert(std::same_as<function_parser,
                           meta::parse::group<
                               meta::parse::group<
                                   meta::parse::token<TokenType::LeftParen>,
                                   meta::parse::optional<
                                       meta::parse::list<
                                           meta::parse::token<TokenType::Identifier>
                                           >
                                       >,
                                   meta::parse::token<TokenType::RightParen>
                                   >,
                               meta::parse::token<TokenType::Arrow>,
                               expression
                               >
                           >);

static_assert(try_apply_rules(function_parser,
                              (a b c d) -> d - ((a - b) + c)
                                  ));

static_assert(!try_apply_rules(function_parser,
                               (a b c d) ->
                               ));



#endif  // CLION_RUST_MACRO_RULE_H
