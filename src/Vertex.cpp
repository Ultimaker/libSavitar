// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/Vertex.h"

using namespace Savitar;

Vertex::Vertex(float x, float y, float z)
    : x_(x)
    , y_(y)
    , z_(z){

    };


float Vertex::getX() const
{
    return x_;
}

float Vertex::getY() const
{
    return y_;
}

float Vertex::getZ() const
{
    return z_;
}
