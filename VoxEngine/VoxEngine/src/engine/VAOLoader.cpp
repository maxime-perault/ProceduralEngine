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
	std::vector<GLuint> vbos;
	int vaoID = this->createVAO();

	vbos.push_back(this->bindIndicesBuffer(indices));
	vbos.push_back(this->storeData_attrList(0, 3, pos));
	vbos.push_back(this->storeData_attrList(1, 2, texture_coord));
	vbos.push_back(this->storeData_attrList(2, 3, normals));

	_vbos.push_back(vbos);

	this->unbindVAO();

	return (new rawModel(vaoID, indices.size()));
}

rawModel	*VAOLoader::loadtoVAO(const std::vector<float> &pos, const std::vector<float> &texture_coord, const std::vector<int> &indices)
{
	std::vector<GLuint> vbos;
	int vaoID = this->createVAO();

	vbos.push_back(this->bindIndicesBuffer(indices));
	vbos.push_back(this->storeData_attrList(0, 2, pos));
	vbos.push_back(this->storeData_attrList(1, 2, texture_coord));

	_vbos.push_back(vbos);

	this->unbindVAO();

	return (new rawModel(vaoID, pos.size()));
}

rawModel	*VAOLoader::loadtoVAO(const std::vector<float> &pos,
	const std::vector<int> &indices)
{
	std::vector<GLuint> vbos;
	int vaoID = this->createVAO();

	vbos.push_back(this->bindIndicesBuffer(indices));
	vbos.push_back(this->storeData_attrList(0, 3, pos));

	_vbos.push_back(vbos);

	this->unbindVAO();

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
	int		width, height, channels;

	glGenTextures(1, &textureID);
	_textures.push_back(textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	img = SOIL_load_image(file.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
	if (img == NULL)
		std::cout << "could not load " << file.c_str() << std::endl;

	if (channels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, img);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img);

	SOIL_free_image_data(img);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4);

	return (textureID);
}

GLuint	VAOLoader::storeData_attrList(const int &attrNum,
	const int &dimension,
	const std::vector<float> &data)
{
	GLuint	vboID;

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	std::vector<float> buffer = data;

	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer[0], GL_STATIC_DRAW);

	glVertexAttribPointer(attrNum, dimension, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vboID;
}

GLuint	VAOLoader::bindIndicesBuffer(const std::vector<int> &indices)
{
	GLuint	vboID;

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);

	std::vector<int> buffer = indices;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(float), &buffer[0], GL_STATIC_DRAW);
	return vboID;
}

void	VAOLoader::unbindVAO(void)
{
	glBindVertexArray(0);
}

void	VAOLoader::deleteVAO(const GLuint vao)
{
	std::vector<GLuint>::iterator				vaoIT;
	std::vector<std::vector<GLuint>>::iterator	vbosIT;

	vbosIT = _vbos.begin();
	for (vaoIT = _vaos.begin(); vaoIT != _vaos.end(); ++vaoIT)
	{
		if (*vaoIT == vao)
		{
			for (std::size_t i = 0; i < (*vbosIT).size(); ++i)
				glDeleteBuffers(1, &(*vbosIT)[i]);
			glDeleteVertexArrays(1, &(*vaoIT));

			_vaos.erase(vaoIT);
			_vbos.erase(vbosIT);

			return;
		}
		++vbosIT;
	}
}

void	VAOLoader::cleanUp(void)
{
	for (std::size_t i(0); i < _vaos.size(); ++i)
		glDeleteVertexArrays(1, &_vaos[i]);
	_vaos.clear();

	for (std::size_t y(0); y < _vbos.size(); ++y)
	{
		for (std::size_t x(0); x < _vbos[y].size(); ++x)
		{
			glDeleteBuffers(1, &_vbos[y][x]);
		}
		_vbos[y].clear();
	}
	_vbos.clear();

	for (std::size_t i(0); i < _textures.size(); ++i)
		glDeleteTextures(1, &_textures[i]);
	_textures.clear();
}
