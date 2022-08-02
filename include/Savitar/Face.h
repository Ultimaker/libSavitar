// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef FACE_H
#define FACE_H

namespace Savitar
{
class Face
{
public:
    /**
     * A face uses the index of 3 vertices to describe a triangle
     */
    Face(int v1, int v2, int v3);
    ~Face();

    int getV1();
    int getV2();
    int getV3();

protected:
    int vertex_1_index;
    int vertex_2_index;
    int vertex_3_index;
};
} // namespace Savitar

#endif // FACE_H
