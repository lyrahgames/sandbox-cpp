#pragma once
#include <xstd/xstd.hpp>
#include <sol/sol.hpp>

namespace sandbox::lua{

struct state {
  sol::state _lua{};
  int _level = 0;
  int _max_level = 32;

  // std::filesystem::path base_path = current_path();

  // std::stop_token stop_token; // similar to std::jthread

  // bool stop_requested() noexcept { return stop_source.stop_requested(); }

  // void request_stop() noexcept {
  //   if (_level > 0) stop_source.request_stop();
  // }

  state();

  ///
  ///
  constexpr void load(std::invocable<sol::state_view> auto&& mod) {
    std::invoke(std::forward<decltype(mod)>(mod), _lua);
  }

  ///
  ///
  constexpr void load(std::invocable<sol::table> auto&& mod) {
    std::invoke(std::forward<decltype(mod)>(mod), _lua.globals());
  }

  auto level() noexcept -> int { return _level; }

  auto max_level() noexcept -> int { return _max_level; }

  void set_max_level(int level);

  void eval(std::string_view str);

  void eval_file(std::filesystem::path const& path);

  auto global_lua_module(sol::table) -> sol::table;
  auto scoped_lua_module(sol::table) -> sol::table;
};

struct live_state : state {
  using base = state;
  std::vector<std::filesystem::path> _live_paths{};

  live_state();

  void try_listen(xstd::fdm::address const& domain);
  void update();

  auto lua_module(sol::table) -> sol::table;
};

}
