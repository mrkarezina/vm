#include "app.h"

int main(int argc, char *argv[]) {
  App app = App(argv[1]);
  app.run();
}
