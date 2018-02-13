#include "VAOLoader.h"
#include "SOIL/SOIL.h"
#include "rawModel.h"

VAOLoader::VAOLoader() {}

VAOLoader::~VAOLoader() {}

rawModel	*VAOLoader::loadtoVAO(const std::vector<float> &pos,
	const std::vector<float> &normals,
	const std::vector<float> &texture_coord,
	const std::vector<int> &indices)
{
	int vaoID = this->createVAO();

	this->bindIndicesBuffer(indices);

	this->storeData_attrList(0, 3, pos);
	this->storeData_attrList(1, 2, texture_coord);
	this->storeData_attrList(2, 3, normals);

	this->unbindVAO();

	// creation du model final
	return (new rawModel(vaoID, indices.size()));
}

int    	VAOLoader::createVAO(void)
{
	GLuint	vaoID;

	glGenVertexArrays(1, &vaoID);
	_vaos.push_back(vaoID);
	glBindVertexArray(vaoID);
	return (vaoID);
}

int	VAOLoader::loadTexture(const std::string &file)
{
	GLuint	textureID;
	unsigned char	*img;
	int		width, height;

	glGenTextures(1, &textureID);
	_textures.push_back(textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	img = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	if (img == NULL)
		std::cout << "could not load " << file.c_str() << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img);
	SOIL_free_image_data(img);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return (textureID);
}

void	VAOLoader::storeData_attrList(const int &attrNum,
	const int &dimension,
	const std::vector<float> &data)
{
	GLuint	vboID;

	glGenBuffers(1, &vboID);
	_vbos.push_back(vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	std::vector<float> buffer = data;

	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer[0], GL_STATIC_DRAW);

	glVertexAttribPointer(attrNum, dimension, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void	VAOLoader::bindIndicesBuffer(const std::vector<int> &indices)
{
	GLuint	vboID;

	glGenBuffers(1, &vboID);
	_vbos.push_back(vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);

	std::vector<int> buffer = indices;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer[0], GL_STATIC_DRAW);
}

void	VAOLoader::unbindVAO(void)
{
	glBindVertexArray(0);
}

void	VAOLoader::cleanUp(void)
{
	for (std::size_t i(0); i < _vaos.size(); ++i)
		glDeleteVertexArrays(1, &_vaos[i]);
	_vaos.clear();

	for (std::size_t i(0); i < _vbos.size(); ++i)
		glDeleteBuffers(1, &_vbos[i]);
	_vbos.clear();

	for (std::size_t i(0); i < _textures.size(); ++i)
		glDeleteTextures(1, &_textures[i]);
	_textures.clear();
}
