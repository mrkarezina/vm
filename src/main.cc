#include "app.h"

int main(int argc, char *argv[]) {
  std::string filename = argc > 1 ? argv[1] : "";
  App app = App(filename);
  app.run();
}
