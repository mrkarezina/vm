#ifndef POSN_H
#define POSN_H

struct Posn {
  int x;
  int y;
  Posn() = default;
  Posn(int x, int y) : x{x}, y{y} {}
  bool operator==(const Posn &other) const {
    return x == other.x && y == other.y;
  }
  bool operator!=(const Posn &other) const {
    return x != other.x || y != other.y;
  }
};

#endif
