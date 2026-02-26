import std;
import sandbox;

using namespace std::literals;
using namespace sandbox;

int main() {
  std::jthread thread{[](std::stop_token internal) {
    while (not internal.stop_requested()) {
      std::this_thread::sleep_for(1s);
      std::println("tick");
    }
  }};
  std::stop_callback external{app::stop_token(),
                              [&] { thread.request_stop(); }};
  std::println("version = {}", sandbox::version::full);
  std::this_thread::sleep_for(5s);
  app::quit();
}
