// Copyright (c) 2025 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/UVCoordinatesIndices.h"

using namespace Savitar;

UVCoordinatesIndices::UVCoordinatesIndices(int group_index, int vertex_1_index, int vertex_2_index, int vertex_3_index)
    : group_index_(group_index)
    , vertex_1_index_(vertex_1_index)
    , vertex_2_index_(vertex_2_index)
    , vertex_3_index_(vertex_3_index)
{
}

int UVCoordinatesIndices::getGroupIndex() const
{
    return group_index_;
}

int UVCoordinatesIndices::getV1() const
{
    return vertex_1_index_;
}

int UVCoordinatesIndices::getV2() const
{
    return vertex_2_index_;
}

int UVCoordinatesIndices::getV3() const
{
    return vertex_3_index_;
}
