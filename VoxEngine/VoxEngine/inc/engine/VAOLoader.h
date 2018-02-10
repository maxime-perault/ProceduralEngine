
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
	std::vector<GLuint>	_vaos;
	std::vector<GLuint>	_vbos;
	std::vector<GLuint>	_textures;

	int			createVAO(void);
	void			storeData_attrList(const int &attrNum,
		const int &dimension,
		const std::vector<float> &data);
	void			unbindVAO(void);
	void			bindIndicesBuffer(const std::vector<int> &indices);

public:
	VAOLoader();
	~VAOLoader();

	rawModel	*loadtoVAO(const std::vector<float> &pos,
		const std::vector<int> &indices,
		const std::vector<float> &tex_coords);
	/*
	RawModel	*loadtoVAO(const std::vector<float> &pos,
	const std::vector<int> &indices,
	const std::vector<float> &tex_coords,
	const std::vector<int> &tex_indices);
	*/
	int	loadTexture(const std::string &file);

	void			cleanUp(void);
};

#endif //!VAOLOADER_HH_

