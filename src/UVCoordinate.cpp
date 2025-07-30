// Copyright (c) 2025 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/UVCoordinate.h"

using namespace Savitar;

UVCoordinate::UVCoordinate(float u, float v)
    : u_(u)
    , v_(v)
{
};

float UVCoordinate::getU() const
{
    return u_;
}

float UVCoordinate::getV() const
{
    return v_;
}
