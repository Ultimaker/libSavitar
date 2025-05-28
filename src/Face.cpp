// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/Face.h"

using namespace Savitar;

Face::Face(int v1, int v2, int v3, const std::optional<UVCoordinatesIndices>& uv_coordinates)
    : vertex_1_index_{ v1 }
    , vertex_2_index_{ v2 }
    , vertex_3_index_{ v3 }
    , uv_coordinates_(uv_coordinates)
{
}

int Face::getV1() const
{
    return vertex_1_index_;
}

int Face::getV2() const
{
    return vertex_2_index_;
}

int Face::getV3() const
{
    return vertex_3_index_;
}

const std::optional<UVCoordinatesIndices>& Face::getUVCoordinates() const
{
    return uv_coordinates_;
}
