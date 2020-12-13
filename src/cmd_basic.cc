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
  if (model->is_write_mode())
    model->toggle_mode();
  else
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
  switch (paste_type) {
    case 'p': {
      // Paste below cur line
      model->new_line(model->get_line_at().size(), model->get_y());
      model->set_y(model->get_y() + 1);
      try {
        model->set_line_at(model->state_str["copied_data"]);
      } catch (std::out_of_range const e) {
      }
      break;
    }
    case 'P': {
      // Paste above cur line
      model->new_line(0, model->get_y());
      try {
        model->set_line_at(model->state_str["copied_data"]);
      } catch (std::out_of_range const e) {
      }
      break;
    }
    default:
      throw invalid_argument("Unrecognized paste_type: " +
                             to_string(paste_type));
      break;
  }
  model->set_x(0);
}
