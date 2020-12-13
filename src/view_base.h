#ifndef VIEW_BASE_H
#define VIEW_BASE_H

class TextModel;

class ViewBase {
 public:
  virtual ~ViewBase() = default;
  virtual void draw(TextModel *model) = 0;
};

#endif
