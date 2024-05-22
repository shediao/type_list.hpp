#pragma once

#include <type_traits>

template <typename...> struct type_list {};

// empty
template <typename List> struct type_list_empty : std::false_type {};
template <> struct type_list_empty<type_list<>> : std::true_type {};
template <typename List>
constexpr auto type_list_empty_v = type_list_empty<List>::value;

// size
template <typename List> struct type_list_size;
template <>
struct type_list_size<type_list<>> : std::integral_constant<int, 0> {};

template <typename Head, typename... Tail>
struct type_list_size<type_list<Head, Tail...>>
    : std::integral_constant<int,
                             1 + type_list_size<type_list<Tail...>>::value> {};

template <typename List>
constexpr auto type_list_size_v = type_list_size<List>::value;

// front
template <typename List> struct type_list_front;
template <typename Head, typename... Tail>
struct type_list_front<type_list<Head, Tail...>> {
  using type = Head;
};

template <typename List>
using type_list_front_t = typename type_list_front<List>::type;

// tail
template <typename List> struct type_list_tail;
template <typename T> struct type_list_tail<type_list<T>> {
  using type = T;
};
template <typename Head, typename... Tail>
struct type_list_tail<type_list<Head, Tail...>> {
  using type = typename type_list_tail<type_list<Tail...>>::type;
};
template <typename List>
using type_list_tail_t = typename type_list_tail<List>::type;

// concat
template <typename... List> struct type_list_concat;

template <typename... T1, typename... T2>
struct type_list_concat<type_list<T1...>, type_list<T2...>> {
  using type = type_list<T1..., T2...>;
};

template <typename T1, typename T2, typename... T3>
struct type_list_concat<T1, T2, T3...> {
  using type = typename type_list_concat<
      T1, typename type_list_concat<T2, T3...>::type>::type;
};

template <typename... List>
using type_list_concat_t = typename type_list_concat<List...>::type;

// reverse
template <typename List> struct type_list_reverse;

template <typename Head, typename... Tail>
struct type_list_reverse<type_list<Head, Tail...>> {
  using type =
      type_list_concat_t<typename type_list_reverse<type_list<Tail...>>::type,
                         type_list<Head>>;
};

template <> struct type_list_reverse<type_list<>> {
  using type = type_list<>;
};

template <typename List>
using type_list_reverse_t = typename type_list_reverse<List>::type;

// remove
template <typename List, typename T> struct type_list_remove;

template <typename T> struct type_list_remove<type_list<>, T> {
  using type = type_list<>;
};

template <typename Head, typename... Tail>
struct type_list_remove<type_list<Head, Tail...>, Head> {
  using type = typename type_list_remove<type_list<Tail...>, Head>::type;
};

template <typename Head, typename... Tail, typename T>
struct type_list_remove<type_list<Head, Tail...>, T> {
  using type = type_list_concat_t<
      type_list<Head>, typename type_list_remove<type_list<Tail...>, T>::type>;
};

template <typename List, typename T>
using type_list_remove_t = typename type_list_remove<List, T>::type;

// append
template <typename List, typename T> struct type_list_append;
template <typename... LT, typename T>
struct type_list_append<type_list<LT...>, T> {
  using type = type_list<LT..., T>;
};

template <typename List, typename T>
using type_list_append_t = typename type_list_append<List, T>::type;

// element
template <typename List, unsigned Idx>
struct type_list_element;

template <typename Head, typename... Tail>
struct type_list_element<type_list<Head, Tail...>, 0> {
  using type = Head;
};

template <typename Head, typename... Tail, unsigned Idx>
struct type_list_element<type_list<Head, Tail...>, Idx> {
  using type = typename type_list_element<type_list<Tail...>, Idx-1>::type;
};

template <typename List, unsigned Idx>
using type_list_element_t = typename type_list_element<List, Idx>::type;

// index
template <typename List, typename T>
struct type_list_index : std::integral_constant<int, -1> {};

template <typename Head, typename... Tail>
struct type_list_index<type_list<Head, Tail...>, Head>
    : std::integral_constant<int, 0> {};

template <typename Head, typename T, typename... Tail>
struct type_list_index<type_list<Head, Tail...>, T>
    : std::conditional_t<
          type_list_index<type_list<Tail...>, T>::value == -1,
          std::integral_constant<int, -1>,
          std::integral_constant<
              int, 1 + type_list_index<type_list<Tail...>, T>::value>> {};
template <typename List, typename T>
constexpr auto type_list_index_v = type_list_index<List, T>::value;

// uniq
template <typename List> struct type_list_uniq;

template <typename Head, typename... Tail>
struct type_list_uniq<type_list<Head, Tail...>> {
  using type = std::conditional_t<
      -1 == type_list_index_v<type_list<Tail...>, Head>,
      type_list_concat_t<type_list<Head>,
                         typename type_list_uniq<type_list<Tail...>>::type>,
      typename type_list_uniq<type_list<Tail...>>::type>;
};

template <> struct type_list_uniq<type_list<>> {
  using type = type_list<>;
};

template <typename List>
using type_list_uniq_t = typename type_list_uniq<List>::type;

// shift
template <typename List, unsigned = 1, typename = void> struct type_list_shift;

template <typename... T> struct type_list_shift<type_list<T...>, 0, void> {
  using type = type_list<T...>;
};

template <unsigned N>
struct type_list_shift<type_list<>, N, std::enable_if_t<(N > 0)>> {
  using type = type_list<>;
};

template <typename Head, typename... Tail, unsigned N>
struct type_list_shift<type_list<Head, Tail...>, N, std::enable_if_t<(N > 0)>> {
  using type = std::conditional_t<
      (N < type_list_size_v<type_list<Head, Tail...>>),
      typename type_list_shift<type_list<Tail...>, N - 1>::type, type_list<>>;
};

template <typename List, unsigned N = 1>
using type_list_shift_t = typename type_list_shift<List, N>::type;
