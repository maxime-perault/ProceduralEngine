
#ifndef CUBEFACTORY_HH_
# define CUBEFACTORY_HH_

# include "VAOLoader.h"
# include "Entity.h"

class	cubeFactory
{
public:
	enum e_Type
	{
		VOID = 0,
		WATER,
		GRASS,
		DIRT,
		STONE,
		COAL,
		SAND,
		SUN,
		FOLIAGE_R,
		FOLIAGE_Y,
		FOLIAGE_G,
		WOOD,
		PLAYER,
		LINE
	};

	enum e_face
	{
		RIGHT = 0,
		LEFT,
		TOP,
		BOT,
		FRONT,
		BACK
	};

private:
	Texture		_terrain;
	
	void	setFace(std::vector<int>& ind, std::vector<float>& uv, std::vector<float>& vtx, std::vector<float>& normals,
			glm::vec3 pos, e_face face, e_Type type, bool light, int nbFace);

public:

	cubeFactory();
	~cubeFactory();

	VAOLoader	_loader;

	bool	isInvisible(std::pair<int, bool>(&chunkInfos)[16][16][16], int x, int y, int z);

	Entity	getWirelessCube(glm::vec4 colour, glm::vec3 pos, GLuint vao);
	Entity	getCube(const e_Type type, glm::vec3 pos, GLuint vao);
	Entity	getChunk(glm::vec3 pos, std::pair<int, bool> (&chunkInfos)[16][16][16], GLuint vao, bool reload, bool modified = false);
	Entity	getLine(glm::vec4 colour, glm::vec3 pos, glm::vec3 rot, GLuint vao);

	Entity	getWater(float size);

	void	setTerrain(void);
};

#endif //!CUBEFACTORY_HH_
