#include "view-text.h"

using namespace std;

void ViewText::draw(TextModel *model) {
    const shared_ptr<vector<string>> lines = model->getLines();
    for (auto l = lines->begin(); l != lines->end(); ++l) {
        cout << *l << endl;
    }
}
