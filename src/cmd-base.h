#ifndef CMD_BASE_H
#define CMD_BASE_H

class TextModel;

class CmdBase {
 public:
  virtual ~CmdBase() = default;
  virtual void exec(TextModel *model) = 0;
};

#endif
