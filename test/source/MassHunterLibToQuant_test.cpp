#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "MassHunterLibToQuant" ? 0 : 1;
}
