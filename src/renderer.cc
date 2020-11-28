#include "renderer.h"

using namespace std;

Renderer::Renderer() { addView(unique_ptr<ViewText>(new ViewText())); }

void Renderer::addView(unique_ptr<ViewBase> view) {
  view->draw();
  views.push_back(move(view));
}

void Renderer::render() {
  for (auto &v : views) {
    v->draw();
  }
}
