
#ifndef CHUNKFACTORY_HH_
# define CHUNKFACTORY_HH_

# include "cubeFactory.h"
# include "perlin.h"

# define CHUNK_X 32
# define CHUNK_Y 32
# define CHUNK_Z 32

struct	s_chunk
{
	std::vector<std::vector<Entity>> VAOChunk;
	std::vector<int> type;
	int chunkInfos[CHUNK_X][CHUNK_Y][CHUNK_Z];
};

class	chunkFactory
{
private:
	cubeFactory	_cubeFactory;
	PerlinNoise _perlin;

	bool	test_hidden(glm::vec3 pos, int (&info)[CHUNK_X][CHUNK_Y][CHUNK_Z]);
	void	disableHiddenCubes(std::vector<Entity>& cubes, int (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z]);
	int		setPile(std::vector<Entity>& cubes, int (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z]);

public:
	enum e_id
	{
		VOID = 0,
		WATER,
		GRASS,
		DIRT,
	};
	chunkFactory();
	~chunkFactory();

	void		setCubeFactory(cubeFactory& cubeFactory);
	s_chunk		getChunk(glm::vec3 pos);
};

#endif //!CHUNKFACTORY_HH_
