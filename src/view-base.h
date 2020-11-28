#ifndef VIEW_BASE_H
#define VIEW_BASE_H

class ViewBase {
 public:
  virtual ~ViewBase() = default;
  virtual void draw() = 0;
};

#endif
