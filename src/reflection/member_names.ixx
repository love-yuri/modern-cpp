// ReSharper disable CppTooWideScopeInitStatement
export module struct_reflection:member_names;

import std;
import :member_count;
import :tie;

/**
 * 计算出struct成员的name，实现关键为member_name函数
 * 他的实现原理是通过各编译器提供的函数名展开，提取成员名
 * 1、通过count_members_fn<T>提取struct的成员数量 -> count_members_fn
 * 2、提取到成员数量后通过tie 将struct返回成结构化绑定 -> tie
 * 3、对返回的结构化绑定成员提取成员指针 -> get_ptr
 * 4、传递给一个 <auto T>的函数，通过编译器扩展拿到函数完整签名从而计算 -> FUNCTION_SIGNATURE
 * 5、拿到一个名称后即可对N进行展开，计算所有成员的名称 -> member_names
 */

/**
 * 获取完整签名
 */
#if defined(_MSC_VER)
#define FUNCTION_SIGNATURE __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#else
#warning("暂未支持非msvc、gcc、clang以外的编译器")
#endif

/**
 * 用于保存一个任意类型的指针
 * @tparam T
 */
template <typename T>
struct ptr_t final {
  const T *ptr;
};

/**
 * 获取T 的第n个参数的类型指针，这里为纯编译期计算，&&t传入仅做类型推导
 * @tparam N 序号
 * @tparam Total 成员总数，提前传入，减少计算
 * @tparam T 模板类型
 * @param t 右值实例，这里实际应用为模板类型推导
 * @return
 */
template <std::size_t N, std::size_t Total, typename T>
consteval auto get_ptr(T &&t) noexcept {
  auto &p = std::get<N>(tie<T, Total>(t));
  return ptr_t<std::remove_cvref_t<decltype(p)>>{&p};
}

/* string_view n_pos */
constexpr auto n_pos = std::string_view::npos;

/**
 * 获取成员名称: mscv实现
 * @tparam T 成员函数指针
 * @return 成员名称
 */
template <auto T>
consteval std::string_view member_name_msvc() {
  constexpr std::string_view full_name = FUNCTION_SIGNATURE;

  constexpr auto start_index = full_name.rfind("->");
  constexpr auto end_index = full_name.rfind('}');
  if constexpr (start_index != n_pos && end_index != n_pos) {
    return full_name.substr(start_index + 2, end_index - start_index - 2);
  }

  return full_name;
}

/**
 * 获取成员名称: clang实现
 * @tparam T 成员函数指针
 * @return 成员名称
 */
template <auto T>
consteval std::string_view member_name_clang() {
  constexpr std::string_view full_name = FUNCTION_SIGNATURE;

  constexpr auto start_index = full_name.rfind('.');
  constexpr auto end_index = full_name.rfind('}');
  if constexpr (start_index != n_pos && end_index != n_pos) {
    return full_name.substr(start_index + 1, end_index - start_index - 1);
  }

  return full_name;
}

/**
 * 获取成员名称
 * @tparam T 成员函数指针
 * @return 成员名称
 */
template <auto T>
consteval std::string_view member_name() {
#if defined(_MSC_VER)
  return member_name_msvc<T>();
#elif defined(__GNUC__) || defined(__clang__)
  return member_name_clang<T>();
#endif
}

/**
 * external使用，用于给编译器提供一个类型已提取成员指针
 * clang: 在clang下extern constexpr T external;也能够编译但是warning
 */
template <typename T>
#if defined(_MSC_VER)
extern constexpr T external;
#elif defined(__GNUC__) || defined(__clang__)
inline constexpr T external{};
#endif

/**
 * 提取所有成员名称
 * @tparam T struct类型
 * @return 包含所有成员的array数组
 */
template <typename T, std::size_t N = count_members_fn<T>()>
consteval std::array<std::string_view, N> member_names() {
  using TType = std::remove_volatile_t<T>;
  return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
    return std::array<std::string_view, N> {
      member_name<get_ptr<Is, N>(external<TType>)>()...
    };
  }(std::make_index_sequence<N>{});
}


/**
 * 导出成员
 */
export namespace yuri::reflection {

/**
 * 获取指针序号的成员名称
 * @tparam T struct类型
 * @tparam N 成员序号
 * @return 成员名称
 */
template <auto N, typename T>
constexpr std::string_view get_name = [] {
  return member_name<get_ptr<N>(external<std::remove_volatile_t<T>>)>();
}();

/**
 * 提取所有成员名称
 * @tparam T struct类型
 * @return 包含所有成员的array数组
 */
template <typename T>
constexpr auto get_names = [] {
  return member_names<T>();
}();

}
