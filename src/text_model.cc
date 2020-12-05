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
  while (y >= lines->size()) {
    lines->push_back("");
  }
  if (x > lines->at(y).size()) {
    throw text_range_access();
  }
  // Add character before cursor (so right at index)
  lines->at(y).insert(x, string{c});
}

// Adds a newline splitting text line in two
void TextModel::new_line(int x, int y) {
  string left = lines->at(y).substr(0, x);
  string right = lines->at(y).substr(x);
  lines->at(y) = left;
  lines->insert(lines->begin() + y + 1, right);
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
