#include "VAOLoader.h"
#include "SOIL/SOIL.h"
#include "rawModel.h"

void	printError(void)
{
	GLenum err(glGetError());

	if (err != GL_NO_ERROR)
	{
		std::string error;

		switch (err)
		{
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		std::cout << error.c_str() << std::endl;
	}
}


VAOLoader::VAOLoader() {}

VAOLoader::~VAOLoader() {}

rawModel	VAOLoader::loadtoVAO(const std::vector<float> &pos,
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

	return (rawModel(vaoID, indices.size()));
}

void	VAOLoader::stockFrags(	const std::vector<float> &pos,
								const std::vector<float> &normals,
								const std::vector<float> &texture_coord,
								const std::vector<int> &indices, GLuint vao)
{
	static s_frag tmp;

	tmp.pos = pos;
	tmp.normals = normals;
	tmp.texture_coord = texture_coord;
	tmp.indices = indices;
	tmp.vao = vao;

	_tmpFrags.push_back(tmp);
}

void	VAOLoader::loadFrags(std::size_t count)
{
	static std::size_t max;

	if (_tmpFrags.empty() == true)
		return;

	if (_tmpFrags.size() > count)
		max = count;
	else
		max = _tmpFrags.size();
		
	for (std::size_t i(0); i < max; ++i)
	{
		this->updateVAO(_tmpFrags.back().pos, _tmpFrags.back().normals, _tmpFrags.back().texture_coord, _tmpFrags.back().indices, _tmpFrags.back().vao);
		_tmpFrags.pop_back();
	}
}

rawModel	VAOLoader::updateVAO(const std::vector<float> &pos,
	const std::vector<float> &normals,
	const std::vector<float> &texture_coord,
	const std::vector<int> &indices,
	GLuint vao)
{
	static GLint current_vao;

	for (std::size_t vaoIT(0); vaoIT < _vaos.size(); ++vaoIT)
	{
		if (_vaos[vaoIT] == vao)
		{
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
			while (current_vao != 0)
			{
				glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
			}

			glBindVertexArray(vao);
			this->updateIndicesBuffer(indices, _vbos[vaoIT][0]);

			this->updateData_attrList(0, 3, pos, _vbos[vaoIT][1]);
			this->updateData_attrList(1, 2, texture_coord, _vbos[vaoIT][2]);
			this->updateData_attrList(2, 3, normals, _vbos[vaoIT][3]);

			this->unbindVAO();
			return (rawModel(vao, indices.size()));
		}
	}
	std::cout << "can't update this VAO" << std::endl;
}



rawModel	VAOLoader::loadtoVAO(const std::vector<float> &pos, const std::vector<float> &texture_coord, const std::vector<int> &indices)
{
	std::vector<GLuint> vbos;
	int vaoID = this->createVAO();

	vbos.push_back(this->bindIndicesBuffer(indices));
	vbos.push_back(this->storeData_attrList(0, 2, pos));
	vbos.push_back(this->storeData_attrList(1, 2, texture_coord));

	_vbos.push_back(vbos);

	this->unbindVAO();

	return (rawModel(vaoID, pos.size()));
}

rawModel	VAOLoader::loadtoVAO(const std::vector<float> &pos,
	const std::vector<int> &indices)
{
	std::vector<GLuint> vbos;
	int vaoID = this->createVAO();

	vbos.push_back(this->bindIndicesBuffer(indices));
	vbos.push_back(this->storeData_attrList(0, 3, pos));

	_vbos.push_back(vbos);

	this->unbindVAO();

	return (rawModel(vaoID, indices.size()));
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

	if (strncmp(file.c_str(), "assets/Fonts/", strlen("assets/Fonts/")) == 0)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4);
	}
	else
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.6);
	}
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

void	VAOLoader::updateData_attrList(const int &attrNum,
										const int &dimension,
										const std::vector<float> &data, GLuint vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	std::vector<float> buffer = data;

	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, buffer.size() * sizeof(float), &buffer[0]);
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

void	VAOLoader::updateIndicesBuffer(const std::vector<int> &indices, GLuint vbo)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);

	std::vector<int> buffer = indices;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.size() * sizeof(float), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, buffer.size() * sizeof(float), &buffer[0]);
}

void	VAOLoader::unbindVAO(void)
{
	glBindVertexArray(0);
}

void	VAOLoader::deleteVAO(const GLuint vao)
{
	for (std::size_t  vaoIT(0); vaoIT < _vaos.size(); ++vaoIT)
	{
		if (_vaos[vaoIT] == vao)
		{
			for (std::size_t i = 0; i < _vbos[vaoIT].size(); ++i)
			{
				glDeleteBuffers(1, &(_vbos[vaoIT][i]));
			}
			_vbos.erase(_vbos.begin() + vaoIT);
			glDeleteVertexArrays(1, &(_vaos[vaoIT]));
			_vaos.erase(_vaos.begin() + vaoIT);

			return;
		}
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
