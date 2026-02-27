#include <sandbox/lua.hpp>
//
#include <experimental/scope>

import sandbox.application;

namespace sandbox::lua{

state::state() {
  _lua.open_libraries(      //
      sol::lib::base,       //
      sol::lib::package,    //
      sol::lib::coroutine,  //
      sol::lib::string,     //
      sol::lib::os,         //
      sol::lib::math,       //
      sol::lib::table,      //
      sol::lib::io,         //
      sol::lib::debug,      //
      sol::lib::bit32,      //
      sol::lib::utf8);
  // load(application::basic_lua_module);
  _lua.set_function("done", application::done);
  _lua.set_function("quit", application::quit);
  load([this](sol::table t) { return scoped_lua_module(t); });
}

void state::set_max_level(int level) {
  _max_level = level;
}

void state::eval(std::string_view str) {
  if (level() >= max_level()) {
    std::println("ERROR: Maximum level ({}) of Lua recursion reached.",
                 max_level());
    return;
  }
  ++_level;
  auto guard = std::experimental::scope_exit{[&] { --_level; }};
  const auto result = _lua.safe_script(str, sol::script_pass_on_error);
  // --_level;
  if (not result.valid())
    std::println("ERROR:\n{}\n", sol::error{result}.what());
}

void state::eval_file(std::filesystem::path const& path) {
  if (level() >= max_level()) {
    std::println("ERROR: Maximum level ({}) of Lua recursion reached.",
                 max_level());
    return;
  }

  // Build absolute path first to prevent empty strings that lead to exceptions.
  const auto p = absolute(path);
  xstd::scoped_chdir _{p.parent_path()};

  ++_level;
  const auto result = _lua.safe_script_file(p, sol::script_pass_on_error);
  --_level;

  if (not result.valid())
    std::println("ERROR:\n{}\n", sol::error{result}.what());
}

auto state::global_lua_module(sol::table table) -> sol::table {
  table["level"] = [this] { return level(); };
  table["max_level"] = [this] { return max_level(); };
  table["set_max_level"] = [this](int x) { set_max_level(x); };
  // table["stop_requested"] = stop_requested;
  // table["request_stop"] = request_stop;
  table["eval"] = [this](std::string_view str) { eval(str); };
  table["eval_file"] = [this](std::string_view str) { eval_file(str); };
  return table;
}

auto state::scoped_lua_module(sol::table table) -> sol::table {
  return global_lua_module(table["lua"].get_or_create<sol::table>());
}

live_state::live_state() : state{} {
  load([this](sol::table t) {
    return lua_module(t["lua"].get_or_create<sol::table>());
  });
}

void live_state::try_listen(xstd::fdm::address const& domain) {
  auto msg = xstd::fdm::recv(domain);
  if (not msg) return;
  std::println("\n{}:\n---\n{}---", proximate(domain).string(), msg.value());
  xstd::scoped_chdir _{domain.parent_path()};
  eval(std::move(msg).value());
}

void live_state::update() {
  for (auto const& path : _live_paths) {
    if (not is_directory(path)) {
      try_listen(path);
      continue;
    }
    for (auto const& entry :
         std::filesystem::recursive_directory_iterator(path)) {
      if (not entry.is_regular_file()) continue;
      if (entry.path().extension() != ".lua") continue;
      try_listen(entry.path());
    }
  }
}

auto live_state::lua_module(sol::table table) -> sol::table {
  auto live = table["live"].get_or_create<sol::table>();
  auto paths = live["paths"].get_or_create<sol::table>();

  paths["assign"] = [this](sol::table const& table) {
    _live_paths.clear();
    for (auto const& [k, v] : table) {
      _live_paths.push_back(
          weakly_canonical(std::filesystem::path(v.as<std::string_view>())));
    }
  };

  paths["append"] = [this](sol::table const& table) {
    for (auto const& [k, v] : table) {
      _live_paths.push_back(
          weakly_canonical(std::filesystem::path(v.as<std::string_view>())));
    }
  };

  paths["clear"] = [this] { _live_paths.clear(); };

  paths["print"] = [this] {
    for (auto const& path : _live_paths) std::println("{}", path.string());
  };

  return live;
}

}
