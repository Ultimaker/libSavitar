// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef SAVITAR_TYPES_H
#define SAVITAR_TYPES_H

#include <cstdint>
#include <vector>

namespace Savitar
{
// Convenience typedef so uint can be used.
using uint = unsigned int;

// Dynamic array of bytes, defined here to increase code readability.
using bytearray = std::vector<uint8_t>;
} // namespace Savitar

#endif
