
#ifndef VAOLOADER_HH_
# define VAOLOADER_HH_

#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <GL/glu.h>
#include "rawModel.h"

struct s_frag
{
	std::vector<float>	pos;
	std::vector<float>	normals;
	std::vector<float>	texture_coord;
	std::vector<int>	indices;
	GLuint				vao;
};

class	VAOLoader
{
private:
	std::vector<GLuint>					_vaos;
	std::vector<std::vector<GLuint>>	_vbos;
	std::vector<GLuint>					_textures;

	std::vector<s_frag>		_tmpFrags;

	GLuint			bindIndicesBuffer(const std::vector<int> &indices);
	void			updateIndicesBuffer(const std::vector<int> &indices, GLuint vbo);


	GLuint			storeData_attrList(const int &attrNum,
					const int &dimension,
					const std::vector<float> &data);

	void			updateData_attrList(const int &attrNum,
					const int &dimension,
					const std::vector<float> &data, GLuint vbo);

	int				createVAO(void);
	void			unbindVAO(void);

public:
	VAOLoader();
	~VAOLoader();

	void	stockFrags(const std::vector<float> &pos,
		const std::vector<float> &normals,
		const std::vector<float> &texture_coord,
		const std::vector<int> &indices, GLuint vao);

	void	loadFrags(void);

	rawModel	loadtoVAO(const std::vector<float> &pos,
		const std::vector<float> &normals,
		const std::vector<float> &texture_coord,
		const std::vector<int> &indices);

	rawModel	updateVAO(const std::vector<float> &pos,
		const std::vector<float> &normals,
		const std::vector<float> &texture_coord,
		const std::vector<int> &indices, GLuint vao);

	rawModel	loadtoVAO(const std::vector<float> &pos,
				const std::vector<float> &texture_coord,
				const std::vector<int> &indices);

	rawModel	loadtoVAO(const std::vector<float> &pos,
		const std::vector<int> &indices);

	int	loadTexture(const std::string &file);

	void	cleanUp(void);
	void	deleteVAO(const GLuint vao);
};

#endif //!VAOLOADER_HH_

