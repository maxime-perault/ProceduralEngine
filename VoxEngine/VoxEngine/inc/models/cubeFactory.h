
#ifndef CUBEFACTORY_HH_
# define CUBEFACTORY_HH_

# include "VAOLoader.h"
# include "Entity.h"

class	cubeFactory
{
private:
	VAOLoader		_loader;
	
	std::vector<rawModel*>		_models;
	std::vector<Texture*>		_textures;
	std::vector<texturedModel*>	_tex_models;
	

	void	initLine(void);
	void	initCube(std::string texturePath, const bool light);
	void	setupModels(void);

public:
	enum e_Type
	{
		GRASS = 0,
		DIRT,
		WATER,
		SUN,
		PLAYER,
		LINE
	};

	cubeFactory();
	~cubeFactory();

	Entity	*getCube(const e_Type type, glm::vec3 pos);
	Entity	*getLine(glm::vec3 colour, glm::vec3 pos, glm::vec3 rot);
};

#endif //!CUBEFACTORY_HH_
