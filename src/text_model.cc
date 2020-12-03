#include "text_model.h"

using namespace std;

class text_range_access {};

TextModel::TextModel(string &filename) {
  lines = make_shared<vector<string>>();
  controller = make_unique<Controller>();

  // TODO: tmp before reading from file
  lines->push_back("Test 1");
  lines->push_back("Test 2");
}

TextModel::~TextModel() {}

void TextModel::run() {
  while (true) {
    render();
    apply(controller->parse_input());
  }
}

const shared_ptr<vector<string>> TextModel::getLines() { return lines; }

void TextModel::writeChar(char c, int x, int y) { lines->at(y)[x] = c; }

void TextModel::addChar(char c, int x, int y) {
  if (y > lines->size()) {
    throw text_range_access();
  }
  if (y == lines->size()) {
    lines->push_back("");
  }
  
  // Add character before cursor (so right at index)
  lines->at(y).insert(x, string{c});
}

void TextModel::setX(int x) { cur_posn.x = x; }

void TextModel::setY(int y) { cur_posn.y = y; }

int TextModel::getX() { return cur_posn.x; }

int TextModel::getY() { return cur_posn.y; }

void TextModel::addView(unique_ptr<ViewBase> view) {
  views.push_back(move(view));
}

void TextModel::apply(unique_ptr<CmdBase> cmd) { cmd->exec(this); }

void TextModel::render() {
  for (auto &v : views) {
    v->draw(this);
  }
}
