
#ifndef CHUNKFACTORY_HH_
# define CHUNKFACTORY_HH_

# include "cubeFactory.h"
# include "perlin.h"

# define CHUNK_X 16
# define CHUNK_Y 16
# define CHUNK_Z 16

struct	s_chunk
{
	std::vector<std::vector<Entity*>> VAOChunk;
	int chunkInfos[CHUNK_X][CHUNK_Y][CHUNK_Z];
};

class	chunkFactory
{
private:
	cubeFactory	_cubeFactory;
	PerlinNoise _perlin;

	bool test_hidden(int x, int y, int z);
	void disableHiddenCubes(std::vector<Entity*> &list);

public:
	enum e_id
	{
		VOID = 0,
		GRASS,
		DIRT,
		WATER,
	};
	chunkFactory(cubeFactory& cubeFactory);
	~chunkFactory();

	s_chunk	getChunk(glm::vec3 pos);
};

#endif //!CHUNKFACTORY_HH_
