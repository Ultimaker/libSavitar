// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef VERTEX_H
#define VERTEX_H

namespace Savitar
{
class Vertex
{
public:
    /**
     * A vertex is a point in 3D space.
     */
    Vertex(float x, float y, float z);
    virtual ~Vertex();

    float getX();
    float getY();
    float getZ();

protected:
    float x;
    float y;
    float z;
};
} // namespace Savitar

#endif