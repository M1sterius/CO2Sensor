#include "Backend.hpp"

#include <fmt/format.h>

#include <string>
#include <iostream>

int32_t main(int32_t argc, char** argv)
{
    auto backend = CO2::PC::Backend();
    backend.Run();
}
