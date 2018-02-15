
#include "texturedModel.h"

texturedModel::texturedModel() {}

texturedModel::texturedModel(rawModel& rawModel, Texture& texture)
{

	_rawModel = rawModel;
	_texture = texture;
}

texturedModel::~texturedModel() {}
