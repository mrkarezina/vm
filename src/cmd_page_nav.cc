#include "cmd_page_nav.h"

using namespace std;

void CmdCtrlG::exec(TextModel *model) { model->set_show_file_stats(true); }
