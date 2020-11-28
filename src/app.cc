#include "app.h"

using namespace std;

App::App() {
  // TODO: parse command line args
  string m = "test";
  text_model = make_shared<TextModel>(m);
}

void App::run() { std::cout << "Hello word!" << std::endl; }
