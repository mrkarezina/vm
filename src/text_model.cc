#include "text_model.h"

using namespace std;

TextModel::TextModel(string &filename) {
  lines = make_shared<vector<string>>();

  // TODO: tmp before reading from file
  lines->push_back("Test 1");
  lines->push_back("Test 2");
}

TextModel::~TextModel() {}

void TextModel::run() { render(); }

const shared_ptr<vector<string>> TextModel::getLines() { return lines; }

void TextModel::setX(int x) { cur_posn.x = x; }

void TextModel::setY(int y) { cur_posn.y = y; }

int TextModel::getX() { return cur_posn.x; }

int TextModel::getY() { return cur_posn.y; }

void TextModel::addView(unique_ptr<ViewBase> view) {
  views.push_back(move(view));
}

void TextModel::render() {
  for (auto &v : views) {
    v->draw(this);
  }
}
