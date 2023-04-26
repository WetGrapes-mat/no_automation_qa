#pragma once

#include "../irender/irender.hxx"

struct line_render : irender {
  public:
    line_render(canvas& buffer_);

    void clear(rgb_color) override;
    void set_pixel(position, rgb_color) override;
    pixels pixels_positions(position start, position end) override;
    void draw_line(position start, position end, rgb_color);

  private:
    canvas& buffer;
};