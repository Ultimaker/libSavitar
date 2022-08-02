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
    virtual ~Vertex() = default;

    [[nodiscard]] float getX() const;
    [[nodiscard]] float getY() const;
    [[nodiscard]] float getZ() const;

private:
    float x_;
    float y_;
    float z_;
};
} // namespace Savitar

#endif