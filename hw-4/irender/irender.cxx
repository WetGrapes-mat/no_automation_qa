#include "irender.hxx"

irender::~irender() {}

double position::length() {
  return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

position operator-(const position& left, const position& right) {
  return {left.x - right.x, left.y - right.y};
}

bool operator==(const position& left, const position& right) {
  return left.x == right.x && left.y == right.y;
}

position position::generate(const int width, const int height) {
  return position {rand() % width, rand() % height};
}

std::tuple<position, position> position::generate(const int width,
                                                  const int height,
                                                  const int amount,
                                                  const int serial) {
  int center_width = width / 2;
  int center_height = height / 2;
  int angle = 360 / amount * serial;

  int x1 = center_width + center_height * std::cos(angle);
  int y1 = center_height + center_height * std::sin(angle);

  int x2 = center_width - center_height * std::cos(angle);
  int y2 = center_height - center_height * std::sin(angle);
  return std::make_tuple(position {x1, y1}, position {x2, y2});
}
