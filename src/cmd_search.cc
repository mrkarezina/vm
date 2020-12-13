#include "cmd_search.h"

using namespace std;

Posn CmdSearchBase::find_next_occurance(TextModel *model, std::string query,
                                        Posn start_pos) {
  int cur_y = start_pos.y;
  int cur_x = start_pos.x;
  string ln = model->get_line_at(start_pos.y);
  size_t pos = ln.find(query, cur_x);
  cur_x = 0;
  while (pos == string::npos) {
    cur_y += 1;
    if (cur_y == model->get_lines()->size()) cur_y = 0;
    ln = model->get_line_at(cur_y);
    pos = ln.find(query, cur_x);
    // Not found if entire text has been searched (looped back to start)
    if (cur_y == start_pos.y && pos == string::npos) return Posn{-1, -1};
  }
  return Posn{(int)pos, cur_y};
}

Posn CmdSearchBase::find_prev_occurance(TextModel *model, std::string query,
                                        Posn start_pos) {
  int cur_y = start_pos.y;
  int cur_x = start_pos.x;
  string ln = model->get_line_at(cur_y);
  size_t pos = ln.rfind(query, cur_x);
  // Correct rfind matching when start pos 0
  // Shouldn't because there are no prev characters
  if (cur_x == 0) pos = string::npos;
  while (pos == string::npos) {
    cur_y -= 1;
    if (cur_y < 0) cur_y = model->get_lines()->size() - 1;
    ln = model->get_line_at(cur_y);
    cur_x = ln.size();
    pos = ln.rfind(query, cur_x);
    // Not found if entire text has been searched
    if (cur_y == start_pos.y && pos == string::npos) return Posn{-1, -1};
  }
  return Posn{(int)pos, cur_y};
}

CmdSearchStart::CmdSearchStart(char search_type, std::string query)
    : search_type{search_type}, query{query} {}

void CmdSearchStart::exec(TextModel *model) {
  // Save state between command for:
  // - search direction
  // - query
  // - lastest hit location x and y
  model->state_int["cmd_search_is_forward_search_direction"] =
      (search_type == '/');
  model->state_str["cmd_search_query"] = query;

  // Move cursor to begining of occurance of
  Posn new_pos;
  if (search_type == '/')
    new_pos =
        find_next_occurance(model, query, Posn{model->get_x(), model->get_y()});
  else
    new_pos =
        find_prev_occurance(model, query, Posn{model->get_x(), model->get_y()});
  if (new_pos.x != -1) {
    model->set_x(new_pos.x);
    model->set_y(new_pos.y);
  } else {
    beep();
  }

  model->state_int["cmd_search_prev_x"] = model->get_x();
  model->state_int["cmd_search_prev_y"] = model->get_y();
}

CmdSearchNav::CmdSearchNav(char c) : nav_type{c} {}

void CmdSearchNav::exec(TextModel *model) {
  try {
    bool is_forward_search =
        model->state_int["cmd_search_is_forward_search_direction"];
    string query = model->state_str["cmd_search_query"];
    int x = model->state_int["cmd_search_prev_x"];
    int y = model->state_int["cmd_search_prev_y"];

    Posn new_pos;
    if ((is_forward_search && nav_type == 'n') ||
        (!is_forward_search && nav_type == 'N')) {
      new_pos = find_next_occurance(model, query, Posn{x + 1, y});
    }
    if ((!is_forward_search && nav_type == 'n') ||
        (is_forward_search && nav_type == 'N')) {
      new_pos = find_prev_occurance(model, query, Posn{x != 0 ? x - 1 : 0, y});
    }
    model->state_int["cmd_search_prev_x"] = new_pos.x;
    model->state_int["cmd_search_prev_y"] = new_pos.y;
    model->set_x(new_pos.x);
    model->set_y(new_pos.y);
  } catch (std::out_of_range const e) {
    // If no search query provided
    beep();
  }
}
