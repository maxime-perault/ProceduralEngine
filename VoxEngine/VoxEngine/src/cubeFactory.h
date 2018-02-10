
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
	std::vector<Entity*>		_entities;
	

	void	initCube(void);
	void	setupModels(void);

public:
	enum e_Type
	{
		GRASS = 0,
		DIRT,
		WATER
	};

	cubeFactory();
	~cubeFactory();

	Entity	*getCube(const e_Type type, glm::vec3 pos, glm::vec3 rotAxe, float euler, glm::vec3 scale);
};

#endif //!CUBEFACTORY_HH_
