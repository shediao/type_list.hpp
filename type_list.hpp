#pragma once

#include <type_traits>

template <typename...> struct type_list {};

// empty
template <typename T> struct type_list_empty : std::false_type {};
template <> struct type_list_empty<type_list<>> : std::true_type {};
template<typename T>
constexpr auto type_list_empty_v = type_list_empty<T>::value;

// size
template <typename T> struct type_list_size;
template <>
struct type_list_size<type_list<>> : std::integral_constant<int, 0> {};

template <typename T, typename... R>
struct type_list_size<type_list<T, R...>>
    : std::integral_constant<int, 1 + type_list_size<type_list<R...>>::value> {
};

template <typename T>
constexpr auto type_list_size_v = type_list_size<T>::value;

// front
template <typename T>
struct type_list_front;
template <typename T0, typename... R>
struct type_list_front<type_list<T0, R...>> {
  using type = T0;
};

template <typename T>
using type_list_front_t = typename type_list_front<T>::type;

// tail
template <typename T>
struct type_list_tail;
template <typename T>
struct type_list_tail<type_list<T>> {
  using type = T;
};
template <typename T0, typename...R>
struct type_list_tail<type_list<T0, R...>> {
  using type = typename type_list_tail<type_list<R...>>::type;
};
template <typename T>
using type_list_tail_t = typename type_list_tail<T>::type;

// concat
template <typename... T>
struct type_list_concat;

template <typename... T1, typename... T2>
struct type_list_concat<type_list<T1...>, type_list<T2...>> {
  using type = type_list<T1..., T2...>;
};

template <typename T1, typename T2, typename... T3>
struct type_list_concat<T1, T2, T3...> {
  using type = typename type_list_concat<T1, typename type_list_concat<T2, T3...>::type>::type;
};

template <typename... T>
using type_list_concat_t = type_list_concat<T...>::type;

// reverse
template <typename T>
struct type_list_reverse;

template <typename T, typename ...R>
struct type_list_reverse<type_list<T, R...>> {
  using type = type_list_concat_t<typename type_list_reverse<type_list<R...>>::type, type_list<T>>;
};

template <>
struct type_list_reverse<type_list<>> {
  using type = type_list<>;
};

template<typename T>
using type_list_reverse_t = typename type_list_reverse<T>::type;

// remove
template <typename T, typename V>
struct type_list_remove;

template <typename T>
struct type_list_remove<type_list<>, T> {
  using type = type_list<>;
};

template <typename T, typename... R>
struct type_list_remove<type_list<T, R...>, T> {
  using type = typename type_list_remove<type_list<R...>, T>::type;
};

template <typename T, typename... R, typename V>
struct type_list_remove<type_list<T, R...>, V> {
  using type = type_list_concat_t<type_list<T>, typename type_list_remove<type_list<R...>, V>::type>;
};

template <typename T, typename V>
using type_list_remove_t = typename type_list_remove<T, V>::type;

// append
template <typename T, typename V>
struct type_list_append;
template <typename... T, typename V>
struct type_list_append<type_list<T...>, V> {
  using type = type_list<T..., V>;
};

template <typename T, typename V>
using type_list_append_t = typename type_list_append<T, V>::type;

// index
template <typename T, typename V>
struct type_list_index: std::integral_constant<int, -1>{};

template <typename T, typename...R>
struct type_list_index<type_list<T,R...>, T>: std::integral_constant<int, 0>{};

template <typename T, typename V, typename...R>
struct type_list_index<type_list<T,R...>, V>: std::conditional_t< type_list_index<type_list<R...>, V>::value == -1,std::integral_constant<int, -1> , std::integral_constant<int, 1 + type_list_index<type_list<R...>, V>::value>>{};
template <typename T, typename V>
constexpr auto type_list_index_v = type_list_index<T, V>::value;

// uniq
template <typename T>
struct type_list_uniq;

template <typename T, typename... R>
struct type_list_uniq<type_list<T, R...>>{
  using type = std::conditional_t<-1 == type_list_index_v<type_list<R...>, T>, type_list_concat_t<type_list<T>, typename type_list_uniq<type_list<R...>>::type>, typename type_list_uniq<type_list<R...>>::type>;
};

template <>
struct type_list_uniq<type_list<>>{
  using type = type_list<>;
};


template <typename T>
using type_list_uniq_t = typename type_list_uniq<T>::type;


// shift
template <typename T, size_t N = 1, typename = void>
struct type_list_shift;

template <typename... T>
struct type_list_shift<type_list<T...>, 0, void> {
  using type = type_list<T...>;
};

template <size_t N>
struct type_list_shift<type_list<>, N, std::enable_if_t<(N > 0)>> {
  using type = type_list<>;
};

template <typename T, typename... R, size_t N>
struct type_list_shift<type_list<T, R...>, N, std::enable_if_t<(N > 0)>> {
  using type = std::conditional_t< (N < type_list_size_v<type_list<T, R...>>), typename type_list_shift<type_list<R...>, N-1>::type, type_list<> >;
};

template <typename T, size_t N = 1>
using type_list_shift_t = type_list_shift<T, N>::type;

// =================================

static_assert(type_list_empty_v<type_list<>>);
static_assert(!type_list_empty_v<type_list<int>>);
static_assert(type_list_size_v<type_list<>> == 0);
static_assert(type_list_size_v<type_list<int>> == 1);
static_assert(type_list_size_v<type_list<int, int>> == 2);
static_assert(type_list_size_v<type_list<bool, int, long>> == 3);
static_assert(type_list_size_v<type_list<char, int, bool, long, double, unsigned >> == 6);

static_assert(std::is_same_v<type_list_front_t<type_list<int>>, int>);
static_assert(std::is_same_v<type_list_front_t<type_list<bool, int>>, bool>);
static_assert(std::is_same_v<type_list_front_t<type_list<int, bool, int>>, int>);
static_assert(std::is_same_v<type_list_front_t<type_list<long, int, bool, int>>, long>);

static_assert(std::is_same_v<type_list_tail_t<type_list<int>>, int>);
static_assert(std::is_same_v<type_list_tail_t<type_list<int, bool>>, bool>);
static_assert(std::is_same_v<type_list_tail_t<type_list<int, bool, int>>, int>);
static_assert(std::is_same_v<type_list_tail_t<type_list<int, bool, int, long>>, long>);


static_assert(type_list_index_v<type_list<>, int> == -1);
static_assert(type_list_index_v<type_list<int>, int> == 0);
static_assert(type_list_index_v<type_list<bool>, int> == -1);
static_assert(type_list_index_v<type_list<bool, int>, int> == 1);
static_assert(type_list_index_v<type_list<bool, int, int>, int> == 1);
static_assert(type_list_index_v<type_list<bool, long, int>, int> == 2);
static_assert(type_list_index_v<type_list<bool, long, int>, char> == -1);

static_assert(std::is_same_v<type_list_concat_t<type_list<>,type_list<>,type_list<>>, type_list<>>);
static_assert(std::is_same_v<type_list_concat_t<type_list<bool>,type_list<>,type_list<int>>, type_list<bool, int>>);
static_assert(std::is_same_v<type_list_concat_t<type_list<bool>,type_list<char>,type_list<int>>, type_list<bool, char, int>>);


static_assert(std::is_same_v<type_list_remove_t<type_list<bool, int, long, double>, int>, type_list<bool, long, double>>);
static_assert(std::is_same_v<type_list_remove_t<type_list<bool, int, long, int, double>, int>, type_list<bool, long, double>>);

static_assert(std::is_same_v<type_list_append_t<type_list<>, int>, type_list<int>>);
static_assert(std::is_same_v<type_list_append_t<type_list<int>, bool>, type_list<int, bool>>);

static_assert(std::is_same_v<type_list_uniq_t<type_list<int, int, int, int, bool>>, type_list<int, bool>>);


static_assert(std::is_same_v<type_list_reverse_t<type_list<bool, char, int, double>>, type_list<double, int, char, bool>>);

static_assert(std::is_same_v<type_list_shift_t<type_list<>>,type_list<>>);
static_assert(std::is_same_v<type_list_shift_t<type_list<>, 100>,type_list<>>);

static_assert(std::is_same_v<type_list_shift_t<type_list<int>>,type_list<>>);
static_assert(std::is_same_v<type_list_shift_t<type_list<int, bool>>,type_list<bool>>);
static_assert(std::is_same_v<type_list_shift_t<type_list<int, bool, double>, 2>,type_list<double>>);

