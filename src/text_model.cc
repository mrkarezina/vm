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

const shared_ptr<vector<string>> TextModel::getLines() {
    return lines;
}

void TextModel::addView(unique_ptr<ViewBase> view) {
  view->draw(this);
  views.push_back(move(view));
}

void TextModel::render() {
  for (auto &v : views) {
    v->draw(this);
  }
}
