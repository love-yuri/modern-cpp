import struct_reflection;
import yuri_log;
import std;

// 测试用例1: 简单结构体
struct simple_struct {
  int id;
  std::string name;
  double value;
};

// 测试用例2: 空结构体
struct empty_struct {
};

// 测试用例3: 单成员结构体
struct single_member {
  int only_one;
};

// 测试用例4: 包含嵌套结构体
struct nested_struct {
  struct inner_data {
    std::string inner_name;
    int inner_id;
  };

  std::string outer_name;
  inner_data nested;
  double score;
};

// 测试用例5: 多种类型混合
struct mixed_types {
  bool flag;
  char letter;
  short small_num;
  int number;
  long big_num;
  float decimal;
  double precise;
  std::string text;
};

// 测试用例6: music_item
struct music_item {
  std::string title;
  std::vector<std::string> artist;
  int duration;
  struct inner {
    std::string album;
  };
  inner metadata;
  int size;
};

// 测试用例7: 只有const成员
struct const_members {
  const int const_value = 42;
  const double pi = 3.14159;
};

// 编译时字符串比较辅助
consteval bool str_equal(std::string_view a, std::string_view b) {
  return a == b;
}

// 编译时测试成员数量
namespace count_tests {
  using namespace yuri;
  static_assert(reflection::count_members<simple_struct> == 3,
                "simple_struct 应该有 3 个成员");

  static_assert(reflection::count_members<empty_struct> == 0,
                "empty_struct 应该有 0 个成员");

  static_assert(reflection::count_members<single_member> == 1,
                "single_member 应该有 1 个成员");

  static_assert(reflection::count_members<nested_struct> == 3,
                "nested_struct 应该有 3 个成员");

  static_assert(reflection::count_members<mixed_types> == 8,
                "mixed_types 应该有 8 个成员");

  static_assert(reflection::count_members<music_item> == 5,
                "music_item 应该有 5 个成员");

  static_assert(reflection::count_members<const_members> == 2,
                "const_members 应该有 2 个成员");
}

// 编译时测试成员名称
namespace name_tests {
  using namespace yuri;
  // simple_struct 名称测试
  static_assert(reflection::get_names<simple_struct>.size() == 3);
  static_assert(str_equal(reflection::get_names<simple_struct>[0], "id"),
                "simple_struct 第1个成员应该是 'id'");
  static_assert(str_equal(reflection::get_names<simple_struct>[1], "name"),
                "simple_struct 第2个成员应该是 'name'");
  static_assert(str_equal(reflection::get_names<simple_struct>[2], "value"),
                "simple_struct 第3个成员应该是 'value'");

  // single_member 名称测试
  static_assert(reflection::get_names<single_member>.size() == 1);
  static_assert(str_equal(reflection::get_names<single_member>[0], "only_one"),
                "single_member 第1个成员应该是 'only_one'");

  // nested_struct 名称测试
  static_assert(reflection::get_names<nested_struct>.size() == 3);
  static_assert(str_equal(reflection::get_names<nested_struct>[0], "outer_name"),
                "nested_struct 第1个成员应该是 'outer_name'");
  static_assert(str_equal(reflection::get_names<nested_struct>[1], "nested"),
                "nested_struct 第2个成员应该是 'nested'");
  static_assert(str_equal(reflection::get_names<nested_struct>[2], "score"),
                "nested_struct 第3个成员应该是 'score'");

  // mixed_types 名称测试
  static_assert(reflection::get_names<mixed_types>.size() == 8);
  static_assert(str_equal(reflection::get_names<mixed_types>[0], "flag"));
  static_assert(str_equal(reflection::get_names<mixed_types>[1], "letter"));
  static_assert(str_equal(reflection::get_names<mixed_types>[2], "small_num"));
  static_assert(str_equal(reflection::get_names<mixed_types>[3], "number"));
  static_assert(str_equal(reflection::get_names<mixed_types>[4], "big_num"));
  static_assert(str_equal(reflection::get_names<mixed_types>[5], "decimal"));
  static_assert(str_equal(reflection::get_names<mixed_types>[6], "precise"));
  static_assert(str_equal(reflection::get_names<mixed_types>[7], "text"));

  // music_item 名称测试
  static_assert(reflection::get_names<music_item>.size() == 5);
  static_assert(str_equal(reflection::get_names<music_item>[0], "title"),
                "music_item 第1个成员应该是 'title'");
  static_assert(str_equal(reflection::get_names<music_item>[1], "artist"),
                "music_item 第2个成员应该是 'artist'");
  static_assert(str_equal(reflection::get_names<music_item>[2], "duration"),
                "music_item 第3个成员应该是 'duration'");
  static_assert(str_equal(reflection::get_names<music_item>[3], "metadata"),
                "music_item 第4个成员应该是 'metadata'");
  static_assert(str_equal(reflection::get_names<music_item>[4], "size"),
                "music_item 第5个成员应该是 'size'");

  // const_members 名称测试
  static_assert(reflection::get_names<const_members>.size() == 2);
  static_assert(str_equal(reflection::get_names<const_members>[0], "const_value"));
  static_assert(str_equal(reflection::get_names<const_members>[1], "pi"));
}

// 运行时打印信息
template <typename T>
void print_reflection_info(const std::string& test_name) {
  using namespace yuri;

  info("========== {} ==========", test_name);

  constexpr auto member_count = reflection::count_members<T>;
  info("成员数量: {}", member_count);

  constexpr auto names = reflection::get_names<T>;

  if constexpr (member_count > 0) {
    info("成员列表:");
    int index = 0;
    for (const auto& member_name : names) {
      info("  [{}] {}", index++, member_name);
    }
  } else {
    info("  (无成员)");
  }

  info("");
}

int main() {
  using namespace yuri;

  info("编译时反射测试 - 所有 static_assert 已通过!\n");

  // 运行时打印信息
  print_reflection_info<simple_struct>("简单结构体");
  print_reflection_info<empty_struct>("空结构体");
  print_reflection_info<single_member>("单成员结构体");
  print_reflection_info<nested_struct>("嵌套结构体");
  print_reflection_info<mixed_types>("混合类型结构体");
  print_reflection_info<music_item>("音乐项目结构体");
  print_reflection_info<const_members>("常量成员结构体");

  info("========== 测试总结 ==========");
  info("✓ 所有编译时断言测试通过!");
  info("✓ 成员数量测试: 7 个结构体");
  info("✓ 成员名称测试: 5 个结构体详细验证");

  return 0;
}
