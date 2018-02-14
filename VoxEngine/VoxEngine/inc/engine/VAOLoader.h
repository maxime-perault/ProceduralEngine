
#ifndef VAOLOADER_HH_
# define VAOLOADER_HH_

#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <GL/glu.h>
#include "rawModel.h"

class	VAOLoader
{
private:
	std::vector<GLuint>					_vaos;
	std::vector<std::vector<GLuint>>	_vbos;
	std::vector<GLuint>					_textures;

	GLuint			bindIndicesBuffer(const std::vector<int> &indices);
	GLuint			storeData_attrList(const int &attrNum,
					const int &dimension,
					const std::vector<float> &data);

	int				createVAO(void);
	void			unbindVAO(void);

public:
	VAOLoader();
	~VAOLoader();

	rawModel	*loadtoVAO(const std::vector<float> &pos,
		const std::vector<float> &normals,
		const std::vector<float> &texture_coord,
		const std::vector<int> &indices);

	rawModel	*loadtoVAO(const std::vector<float> &pos,
				const std::vector<float> &texture_coord,
				const std::vector<int> &indices);

	rawModel	*loadtoVAO(const std::vector<float> &pos,
		const std::vector<int> &indices);

	int	loadTexture(const std::string &file);

	void	cleanUp(void);
	void	deleteVAO(const GLuint vao);
};

#endif //!VAOLOADER_HH_

