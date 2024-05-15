#include "type_list.hpp"

int main() {
  static_assert(type_list_empty_v<type_list<>>);
  static_assert(!type_list_empty_v<type_list<int>>);
  static_assert(type_list_size_v<type_list<>> == 0);
  static_assert(type_list_size_v<type_list<int>> == 1);
  static_assert(type_list_size_v<type_list<int, int>> == 2);
  static_assert(type_list_size_v<type_list<bool, int, long>> == 3);
  static_assert(
      type_list_size_v<type_list<char, int, bool, long, double, unsigned>> ==
      6);

  static_assert(std::is_same_v<type_list_front_t<type_list<int>>, int>);
  static_assert(std::is_same_v<type_list_front_t<type_list<bool, int>>, bool>);
  static_assert(
      std::is_same_v<type_list_front_t<type_list<int, bool, int>>, int>);
  static_assert(
      std::is_same_v<type_list_front_t<type_list<long, int, bool, int>>, long>);

  static_assert(std::is_same_v<type_list_tail_t<type_list<int>>, int>);
  static_assert(std::is_same_v<type_list_tail_t<type_list<int, bool>>, bool>);
  static_assert(
      std::is_same_v<type_list_tail_t<type_list<int, bool, int>>, int>);
  static_assert(
      std::is_same_v<type_list_tail_t<type_list<int, bool, int, long>>, long>);

  static_assert(type_list_index_v<type_list<>, int> == -1);
  static_assert(type_list_index_v<type_list<int>, int> == 0);
  static_assert(type_list_index_v<type_list<bool>, int> == -1);
  static_assert(type_list_index_v<type_list<bool, int>, int> == 1);
  static_assert(type_list_index_v<type_list<bool, int, int>, int> == 1);
  static_assert(type_list_index_v<type_list<bool, long, int>, int> == 2);
  static_assert(type_list_index_v<type_list<bool, long, int>, char> == -1);

  static_assert(
      std::is_same_v<type_list_concat_t<type_list<>, type_list<>, type_list<>>,
                     type_list<>>);
  static_assert(
      std::is_same_v<
          type_list_concat_t<type_list<bool>, type_list<>, type_list<int>>,
          type_list<bool, int>>);
  static_assert(
      std::is_same_v<
          type_list_concat_t<type_list<bool>, type_list<char>, type_list<int>>,
          type_list<bool, char, int>>);

  static_assert(std::is_same_v<
                type_list_remove_t<type_list<bool, int, long, double>, int>,
                type_list<bool, long, double>>);
  static_assert(
      std::is_same_v<
          type_list_remove_t<type_list<bool, int, long, int, double>, int>,
          type_list<bool, long, double>>);

  static_assert(
      std::is_same_v<type_list_append_t<type_list<>, int>, type_list<int>>);
  static_assert(std::is_same_v<type_list_append_t<type_list<int>, bool>,
                               type_list<int, bool>>);

  static_assert(
      std::is_same_v<type_list_uniq_t<type_list<int, int, int, int, bool>>,
                     type_list<int, bool>>);

  static_assert(
      std::is_same_v<type_list_reverse_t<type_list<bool, char, int, double>>,
                     type_list<double, int, char, bool>>);

  static_assert(std::is_same_v<type_list_shift_t<type_list<>>, type_list<>>);
  static_assert(
      std::is_same_v<type_list_shift_t<type_list<>, 100>, type_list<>>);

  static_assert(std::is_same_v<type_list_shift_t<type_list<int>>, type_list<>>);
  static_assert(
      std::is_same_v<type_list_shift_t<type_list<int, bool>>, type_list<bool>>);
  static_assert(
      std::is_same_v<type_list_shift_t<type_list<int, bool, double>, 2>,
                     type_list<double>>);
}
