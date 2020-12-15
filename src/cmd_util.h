#ifndef CMD_UTIL_H
#define CMD_UTIL_H

class TextModel;

void ensure_y_in_bounds(TextModel *model);

int line_size_to_x_cord(int y, TextModel *model);

#endif
