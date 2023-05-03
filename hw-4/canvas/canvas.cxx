#include "canvas.hxx"

uint8_t color_format = 255;

bool operator==(const rgb_color& left, const rgb_color& right) {
  return left.red == right.red && left.green == right.green && left.blue == right.blue;
}

rgb_color color_red {255, 0, 0};
rgb_color color_green {0, 255, 0};
rgb_color color_blue {0, 0, 255};

constexpr size_t color_size = sizeof(rgb_color);
static_assert(3 == color_size, "24 bit per pixel(r,g,b)");

std::ostream& operator<<(std::ostream& out, const rgb_color& color) {
  out << "rgb_color(" << static_cast<int>(color.red) << ", " << static_cast<int>(color.green)
      << ", " << static_cast<int>(color.blue) << ")";

  return out;
}

rgb_color rgb_color::generate() {
  return {static_cast<uint8_t>(rand() % 256),
          static_cast<uint8_t>(rand() % 256),
          static_cast<uint8_t>(rand() % 256)};
}

canvas::canvas(size_t w, size_t h) {
  width = w;
  height = h;
  pixels.resize(w * h);
}

void canvas::save_image(const char* path) {
  std::FILE* file = std::fopen(path, "wb");
  if (!file)
    throw std::runtime_error("Can't open file");

  std::fprintf(file, "P6\n%d %d %d\n", width, height, color_format);

  std::fwrite(pixels.data(), sizeof(rgb_color), pixels.size(), file);
  std::fclose(file);
}

void canvas::load_image(const char* path) {
  std::FILE* file = std::fopen(path, "rb");
  if (!file)
    throw std::runtime_error("Can't open file");

  char temp;
  uint8_t color_format_temp;

  std::fscanf(file, "P6\n%d %d %d\n", &width, &height, &color_format_temp);

  if (color_format != color_format_temp)
    throw std::runtime_error("This PPM file doesnt fit the color format.");

  pixels.resize(width * height);

  std::fread(pixels.data(), sizeof(rgb_color), pixels.size(), file);
  std::fread(&temp, sizeof(char), 1, file);

  std::fclose(file);
}

void canvas::set_pixel(size_t x, size_t y, rgb_color color) {
  if (x > width || y > height) {
    throw std::runtime_error("overflow");
  }
  if (x == width) {
    x -= 1;
  }
  if (y == height) {
    y--;
  }
  rgb_color& pixel = pixels.at(y * width + x);
  pixel = color;
}

rgb_color canvas::get_pixel(size_t x, size_t y) const {
  if (x > width || y > height) {
    throw std::runtime_error("overflow");
  }
  return pixels.at(y * width + x);
}
