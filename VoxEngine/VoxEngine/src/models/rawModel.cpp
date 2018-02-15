#include "RawModel.h"
#include <iostream>

rawModel::rawModel() {}

rawModel::rawModel(const std::size_t vao_id, const std::size_t vertex_count)
{
	_vao_id = vao_id;
	_vertex_count = vertex_count;
}

rawModel::~rawModel() {}