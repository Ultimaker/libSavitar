// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef FACE_H
#define FACE_H

#include "Savitar/UVCoordinatesIndices.h"

#include <optional>

namespace Savitar
{
class Face
{
public:
    /**
     * A face uses the index of 3 vertices to describe a triangle
     */
    Face(int v1, int v2, int v3, const std::optional<UVCoordinatesIndices>& uv_coordinates = std::nullopt);
    ~Face() = default;

    [[nodiscard]] int getV1() const;
    [[nodiscard]] int getV2() const;
    [[nodiscard]] int getV3() const;
    [[nodiscard]] const std::optional<UVCoordinatesIndices>& getUVCoordinates() const;

private:
    int vertex_1_index_;
    int vertex_2_index_;
    int vertex_3_index_;
    std::optional<UVCoordinatesIndices> uv_coordinates_;
};
} // namespace Savitar

#endif // FACE_H
