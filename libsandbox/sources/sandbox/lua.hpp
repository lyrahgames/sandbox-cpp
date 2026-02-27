#pragma once
#include <sol/sol.hpp>
#include <xstd/xstd.hpp>

namespace sandbox::lua {

struct state {
  sol::state _lua{};
  int _level     = 0;
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

  /// Returns the current recursion level of Lua execution, ie.,
  /// the number of times `eval` or `eval_file` have been called, recursively.
  ///
  auto level() noexcept -> int { return _level; }

  /// Returns the maximum recursion level allowed for Lua evaluation calls.
  ///
  auto max_level() noexcept -> int { return _max_level; }

  /// Sets the maximum recursion level allowed for Lua evaluation calls.
  /// Used to prevent stack overflows and infinite recursion for Lua eval.
  /// Already a few hundred levels can lead to a stack overflow.
  ///
  void set_max_level(int level);

  /// Evaluate the given string as Lua code.
  ///
  void eval(std::string_view str);

  /// Read and evaluate the given file as Lua code.
  ///
  void eval_file(std::filesystem::path const& path);

  auto global_lua_module(sol::table) -> sol::table;
  auto scoped_lua_module(sol::table) -> sol::table;
};

struct live_state : state {
  using base = state;
  std::vector<std::filesystem::path> _live_paths{};

  live_state();

  /// Try to receive a message for the given address/file path domain.
  /// If successful, change the current path to the address' parent path
  /// and evaluate the given message as Lua code.
  ///
  void try_listen(xstd::fdm::address const& domain);

  /// Listen to all Lua files recursively contained in the stored live paths.
  ///
  void update();

  auto lua_module(sol::table) -> sol::table;
};

}  // namespace sandbox::lua
