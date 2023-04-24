#include <array>
#include <iostream>
#include <thread>

#include "engine.hxx"

class eye_game : public grp::game {
  public:
    eye_game() :
      index {
        0
    },
      frames {{" -----\n|     |\n|  O  |\n|     |\n -----",
               " -----\n|     |\n|  o  |\n|  -  |\n -----",
               " -----\n|     |\n|  -  |\n|  ~  |\n -----",
               " -----\n|     |\n|     |\n|  ^  |\n -----"}} {}
    // frames {{" -----\n|     |\n|  O  |\n|     |\n -----",
    //          " -----\n|     |\n|  o  |\n|     |\n -----",
    //          " -----\n|     |\n|  -  |\n|     |\n -----",
    //          " -----\n|     |\n|     |\n|     |\n -----"}} {}
    void update() override {
      index++;
      index %= frames.size();
      using namespace std::chrono;
      std::this_thread::sleep_for(milliseconds(60));
    }
    void render() const override {
      const std::string current_symbol = frames.at(index);
      std::cout << "\033[2J\033[0;0f" << current_symbol << std::flush;
    }

  private:
    uint32_t index = 0;
    const std::array<std::string, 4> frames;
};

grp::game* create_game(grp::engine* engine) {
  if (engine != nullptr) {
    return new eye_game();
  }
  return nullptr;
}
