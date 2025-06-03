// Copyright (c) 2025 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef UVCOORDINATESINDICES_H
#define UVCOORDINATESINDICES_H

namespace Savitar
{
/**
 * UV coordinates indices contains the UV group ID and associated indices for each point of a face. A single mesh may contain indices
 * from multiple texture.
 */
class UVCoordinatesIndices
{
public:
    UVCoordinatesIndices(int group_index, int vertex_1_index, int vertex_2_index, int vertex_3_index);
    virtual ~UVCoordinatesIndices() = default;

    [[nodiscard]] int getGroupIndex() const;
    [[nodiscard]] int getV1() const;
    [[nodiscard]] int getV2() const;
    [[nodiscard]] int getV3() const;

private:
    int group_index_{};
    int vertex_1_index_{};
    int vertex_2_index_{};
    int vertex_3_index_{};
};
} // namespace Savitar

#endif