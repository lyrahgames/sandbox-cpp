module sandbox.application;

namespace sandbox::application {

// The application's global stop source is a variable with internal linkage.
// It is used for cooperative shutdowns by issuing requests to shutdown
// the application via `quit` and by checking for such requests via `done`.
static std::stop_source stop_source{};

bool done() noexcept {
  return stop_source.stop_requested();
}

void quit() noexcept {
  stop_source.request_stop();
}

auto stop_token() noexcept -> std::stop_token {
  return stop_source.get_token();
}

}  // namespace sandbox::application
