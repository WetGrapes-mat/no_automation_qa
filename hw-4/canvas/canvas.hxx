#pragma once
#include <cstdio>
#include <iomanip>
#include <vector>

extern uint8_t color_format;

#pragma pack(push, 1)
struct rgb_color {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    friend bool operator==(const rgb_color& left, const rgb_color& right);

    friend std::ostream& operator<<(std::ostream& out, const rgb_color& color);

    static rgb_color generate();
};
#pragma pack(pop)

extern rgb_color color_red;
extern rgb_color color_green;
extern rgb_color color_blue;

class canvas {
  public:
    canvas(size_t w, size_t h);

    void save_image(const char* path);
    void load_image(const char* path);

    void set_pixel(size_t x, size_t y, rgb_color color);
    rgb_color get_pixel(size_t x, size_t y) const;

    auto begin() {
      return pixels.begin();
    }
    auto end() {
      return pixels.end();
    }
    std::vector<rgb_color>& get_pixels() {
      return pixels;
    }

    bool operator==(const canvas& other) const;
    bool operator!=(const canvas& other) const;

  private:
    size_t width = 0;
    size_t height = 0;
    std::vector<rgb_color> pixels;
};
