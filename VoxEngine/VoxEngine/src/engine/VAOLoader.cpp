#include "VAOLoader.h"
#include "SOIL/SOIL.h"
#include "rawModel.h"

/*
** TOOLS
*/
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

int gltIsExtSupported(const char *extension)
{
	GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	where = (GLubyte *)strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;

	extensions = (GLubyte *)glGetString(GL_EXTENSIONS);

	start = extensions;
	for (;;)
	{
		where = (GLubyte *)strstr((const char *)start, extension);

		if (!where)
			break;

		terminator = where + strlen(extension);

		if (where == start || *(where - 1) == ' ')
		{
			if (*terminator == ' ' || *terminator == '\0')
				return 1;
		}
		start = terminator;
	}
}
/*
** !TOOLS
*/

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

	printError();

	return (rawModel(vaoID, indices.size()));
}

void	VAOLoader::stockFrags(	const std::vector<float> &pos,
								const std::vector<float> &normals,
								const std::vector<float> &texture_coord,
								const std::vector<int> &indices, GLuint vao, bool update)
{
	static s_frag tmp;

	if (update == true)
	{
		for (std::size_t i(0); i < _tmpFrags.size(); ++i)
		{
			if (vao == _tmpFrags[i].vao)
			{
				_tmpFrags[i].pos = pos;
				_tmpFrags[i].normals = normals;
				_tmpFrags[i].texture_coord = texture_coord;
				_tmpFrags[i].indices = indices;
				return;
			}
		}
	}
	tmp.pos = pos;
	tmp.normals = normals;
	tmp.texture_coord = texture_coord;
	tmp.indices = indices;
	tmp.vao = vao;

	_tmpFrags.push_back(tmp);
}

bool	VAOLoader::loadFrags(std::size_t count)
{
	static std::size_t max;

	if (_tmpFrags.empty() == true)
		return false;

	if (_tmpFrags.size() > count)
		max = count;
	else
		max = _tmpFrags.size();
		
	for (std::size_t i(0); i < max; ++i)
	{
		this->updateVAO(_tmpFrags.back().pos, _tmpFrags.back().normals, _tmpFrags.back().texture_coord, _tmpFrags.back().indices, _tmpFrags.back().vao);
		_tmpFrags.pop_back();
	}

	return true;
}

void	VAOLoader::updateVAO(const std::vector<float> &pos,
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
			return ;
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

void	VAOLoader::setTexture(const std::string &file, GLint level)
{
	unsigned char*	img;
	int				width, height, channels;


	img = SOIL_load_image(file.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
	if (img == NULL)
		std::cout << "could not load " << file.c_str() << std::endl;

	if (channels == 4)
		glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, img);
	else
		glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img);

	SOIL_free_image_data(img);
}

int	VAOLoader::loadTexture(const std::string &file, bool own_mipmap)
{
	GLuint			textureID;
	
	glGenTextures(1, &textureID);
	_textures.push_back(textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	if (strncmp(file.c_str(), "assets/Fonts/", strlen("assets/Fonts/")) == 0)
	{
		this->setTexture(file, 0);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4);
	}
	else
	{
		static std::string	current_file;

		current_file = file;
		if (own_mipmap == true)
			current_file.append("_256.png");
		this->setTexture(current_file, 0);
		
		current_file = file;
		if (own_mipmap == true)
			current_file.append("_128.png");
		this->setTexture(current_file, 1);

		current_file = file;
		if (own_mipmap == true)
			current_file.append("_64.png");
		this->setTexture(current_file, 2);

		current_file = file;
		if (own_mipmap == true)
			current_file.append("_32.png");
		this->setTexture(current_file, 3);

		current_file = file;
		if (own_mipmap == true)
			current_file.append("_16.png");
		this->setTexture(current_file, 4);

		current_file = file;
		if (own_mipmap == true)
			current_file.append("_8.png");
		this->setTexture(current_file, 5);

		current_file = file;
		if (own_mipmap == true)
			current_file.append("_4.png");
		this->setTexture(current_file, 6);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 1);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.6);

		/*
		if (gltIsExtSupported("GL_EXT_texture_filter_anisotropic"))
		{
			static float ani_level;

			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ani_level);
			ani_level = fmin(2.f, ani_level);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, ani_level);
		}
		else
			std::cout << "anisotropic filtering not supported" << std::endl;
			*/

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
