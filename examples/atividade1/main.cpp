#include "window.hpp"

int main(int argc, char **argv) {
  try {
    // Create application instance
    abcg::Application app(argc, argv);

    // Create OpenGL window
    Window window;
    window.setWindowSettings({.title = "Atividade 1"});

    // Run application
    app.run(window);
  } catch (std::exception const &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}