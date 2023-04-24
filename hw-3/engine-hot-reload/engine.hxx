#include <iostream>

namespace grp {

enum class event {
  a_pressed,
  a_released,
  d_pressed,
  d_released,
  w_pressed,
  w_released,
  s_pressed,
  s_released,
  space_pressed,
  space_released,
  escape_pressed,
  escape_released,
  turn_off
};

class engine;

engine* create_engine();
void destroy_engine(engine* e);

std::ostream& operator<<(std::ostream& out, event e);

class engine {
  public:
    void init();
    void uninit();
    bool input_event(event& e);
    void render();
};

class game {
  public:
    virtual void update() = 0;
    virtual void render() const = 0;
};

} // namespace grp

extern "C" grp::game* create_game(grp::engine*);