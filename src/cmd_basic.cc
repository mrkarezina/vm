#include "cmd_basic.h"

using namespace std;

CmdWrite::CmdWrite(char c) : toWrite{c} {}

void CmdWrite::exec(TextModel *model) {
  int x = model->get_x();
  int y = model->get_y();
  if (model->is_write_mode()) model->add_char(toWrite, x, y);
  model->set_x(x + 1);
}

void CmdEnter::exec(TextModel *model) {
  int x = model->get_x();
  int y = model->get_y();
  if (model->is_write_mode()) {
    model->new_line(x, y);
    model->set_x(0);
    model->set_y(y + 1);
  } else {
    if (y < model->get_lines()->size() - 1) {
      model->set_x(0);
      model->set_y(y + 1);
    } else
      beep();
  }
}

void CmdBackspace::exec(TextModel *model) {
  int x = model->get_x();
  int y = model->get_y();
  if (x > 0 || y > 0) {
    if (model->is_write_mode()) {
      if (x == 0) {
        // Set x to length of prev line
        int new_x = model->get_lines()->at(y - 1).size();
        model->delete_line(y);
        model->set_x(new_x);
        model->set_y(y - 1);
      } else {
        model->delete_char(x, y);
        model->set_x(x - 1);
      }
    } else {
      if (x == 0) {
        // Move to end and up
        int new_x = model->get_lines()->at(y - 1).size();
        model->set_x(new_x == 0 ? new_x : new_x - 1);
        model->set_y(y - 1);
      } else {
        model->set_x(x - 1);
      }
    }
  } else {
    beep();
  }
}

void CmdEsc::exec(TextModel *model) {
  if (model->is_write_mode()) {
    model->toggle_mode();
    // Ensure that x is not left past length of line
    if (model->get_x() >= model->get_line_at().size()) {
      int ln_size = model->get_line_at().size();
      model->set_x(max(ln_size - 1, 0));
    }
  } else
    model->set_cmd_so_far("");
}

CmdInsert::CmdInsert(char c) : insert_type{c} {}

void CmdInsert::exec(TextModel *model) {
  switch (insert_type) {
    case 'a':
      model->set_x(model->get_x() + 1);
      break;
    case 'A':
      model->set_x(model->get_lines()->at(model->get_y()).size());
      break;
    case 'i':
      break;
    case 'I':
      model->set_x(0);
      break;
    case 'o':
      model->new_line(model->get_lines()->at(model->get_y()).size(),
                      model->get_y());
      model->set_y(model->get_y() + 1);
      model->set_x(0);
      break;
    case 'O':
      model->new_line(0, model->get_y());
      model->set_y(model->get_y());
      model->set_x(0);
      break;
    default:
      throw invalid_argument("Unrecognized insert_type: " +
                             to_string(insert_type));
      break;
  }
  model->toggle_mode();
}

void CmdJ::exec(TextModel *model) {
  if (1 < model->get_lines()->size() &&
      model->get_y() < model->get_lines()->size() - 1) {
    string ln = model->get_line_at(model->get_y() + 1);
    int x = model->get_line_at().size() - 1;
    if (ln.size() > 0) {
      model->set_line_at(model->get_line_at() + " " + ln);
      x += 1;
    }
    model->delete_line(model->get_y() + 1);
    ensure_y_in_bounds(model);
    model->set_x(x);
  }
}

void CmdStall::exec(TextModel *model) {}

CmdLineSelection::CmdLineSelection(string selection) : selection{selection} {}

void CmdLineSelection::exec(TextModel *model) {
  if (selection == "0") {
    model->set_y(0);
  } else if (selection == "$") {
    model->set_y((int)model->get_lines()->size() - 1);
  } else {
    int line = stoi(selection);
    model->set_y(min(line, (int)model->get_lines()->size() - 1));
  }
  model->set_x(0);
}

CmdsS::CmdsS(char c) : sub_type{c} {}

void CmdsS::exec(TextModel *model) {
  switch (sub_type) {
    case 's':
      model->delete_char(model->get_x() + 1, model->get_y());
      break;
    case 'S':
      model->clear_line(model->get_y());
      model->set_x(0);
      break;
    default:
      throw invalid_argument("Unrecognized sub_type: " + to_string(sub_type));
      break;
  }
  model->toggle_mode();
}

CmdPaste::CmdPaste(char c) : paste_type{c} {}

void CmdPaste::exec(TextModel *model) {
  if (model->clipboard->get_lines().size() > 0) {
    if (model->clipboard->is_paste_on_newline()) {
      for (auto l : model->clipboard->get_lines()) {
        if (paste_type == 'p') {
          model->new_line(model->get_line_at().size(), model->get_y());
          model->set_y(model->get_y() + 1);
          model->set_line_at(l);
        } else {
          model->new_line(0, model->get_y());
          model->set_line_at(l);
        }
      }
      model->set_x(0);
    } else {
      string ln = model->get_line_at();
      ln.insert(model->get_x(), model->clipboard->get_lines()[0]);
      model->set_line_at(ln);
      if (paste_type == 'P')
        model->set_x(min(model->get_x() + 1, (int)ln.size() - 1));
    }
  }
}

void CmdUndo::exec(TextModel *model) {
  // Pop state before undo command
  model->history->pop_last_text_state();
  model->history->pop_last_cursor_state();
  if (model->history->state_size() > 0) {
    model->set_lines(model->history->pop_last_text_state());
    Posn undo = model->history->pop_last_cursor_state();
    model->set_posn(undo);
  }
}
