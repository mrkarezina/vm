#include "view-text.h"

using namespace std;

ViewText::ViewText() { win = make_unique<Window>(1, 0.75, 0, 0); }

void ViewText::draw(TextModel *model) {
  const shared_ptr<vector<string>> lines = model->getLines();
  for (size_t i = 0; i < lines->size(); i++) {
    string l = lines->at(i);
    for (size_t j = 0; j < l.size(); j++) {
      win->writeChar(l[j], model->getX(), model->getY());
      model->setX(model->getX() + 1);
    }
    model->setY(model->getY() + 1);
  }
  win->refresh();
}
