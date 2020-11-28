#include "text_model.h"

using namespace std;

TextModel::TextModel(string &filename) { renderer = make_unique<Renderer>(); }

void TextModel::run() { renderer->render(); }
