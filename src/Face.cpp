#include "Face.h"

Face::Face(int v1, int v2, int v3)
{
    vertex_1_index = v1;
    vertex_2_index = v2;
    vertex_3_index = v3;
}

Face::~Face()
{

}

int Face::getV1()
{
    return vertex_1_index;
}

int Face::getV2()
{
    return vertex_2_index;
}

int Face::getV3()
{
    return vertex_3_index;
}
