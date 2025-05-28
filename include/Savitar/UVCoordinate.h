// Copyright (c) 2025 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef UVCOORDINATE_H
#define UVCOORDINATE_H

namespace Savitar
{
class UVCoordinate
{
public:
    /**
     * A UV coordinate represents the position of the point on a texture image.
     */
    UVCoordinate(float u, float v);
    virtual ~UVCoordinate() = default;

    [[nodiscard]] float getU() const;
    [[nodiscard]] float getV() const;

private:
    float u_;
    float v_;
};
} // namespace Savitar

#endif