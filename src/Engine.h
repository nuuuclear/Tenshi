#pragma once

#include <string>
#include <cstdint>

namespace Tenshi {

unsigned long GetTicks();
unsigned long CountFPS();
void Delay(uint32_t ticks);

}