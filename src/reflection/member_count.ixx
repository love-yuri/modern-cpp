export module struct_reflection:member_count;

import std;

/**
 * 获取struct的成员数量，实现方法: count_members_fn
 * 1、使用std::remove_cvref_t获取原始类型
 * 2、开始模板递归：尝试使用一个any_value初始化struct，如果成功则继续
 * 3、开始使用n + 1个参数初始化struct直到失败返回
 * 使用了编译器试探原理，尝试使用n个元素初始化struct，直到失败为止
 */

template <typename T>
concept can_init_any = !std::same_as<T, const char *> &&
    !std::same_as<T, std::nullptr_t> &&
    !std::is_array_v<T>;

struct any_type {
  // ReSharper disable once CppFunctionIsNotImplemented
  // ReSharper disable once CppNonExplicitConversionOperator
  template <typename T> requires can_init_any<T>
  [[maybe_unused]] constexpr operator T() const; // NOLINT(*-explicit-constructor)

  // ReSharper disable once CppNonExplicitConversionOperator
  [[maybe_unused]] constexpr operator std::string_view() const {
    return {};
  }
};

/**
 * 递归模板统计T的size
 * @tparam T 模板
 */
template <class T, class... Args>
consteval std::size_t count_members_fn() {
  using V = std::remove_cvref_t<T>;
  if constexpr (requires { V{Args{}..., any_type{}}; }) {
    return count_members_fn<V, Args..., any_type>();
  } else {
    return sizeof...(Args);
  }
}

export namespace yuri::reflection {

/**
 * 获取T的成员数量
 */
template <class T> requires(std::is_aggregate_v<std::remove_cvref_t<T>>)
inline constexpr auto count_members = [] {
  return count_members_fn<T>();
}();

}
