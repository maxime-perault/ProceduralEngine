
#ifndef CHUNKFACTORY_HH_
# define CHUNKFACTORY_HH_

# include "cubeFactory.h"
# include "perlin.h"

# define CHUNK_X 16
# define CHUNK_Y 16
# define CHUNK_Z 16

struct	s_chunk
{
	Entity	Chunk;
	std::pair<int, bool>	chunkInfos[CHUNK_X][CHUNK_Y][CHUNK_Z];
};

class	chunkFactory
{
private:
	cubeFactory	_cubeFactory;
	PerlinNoise _perlin;

	bool	test_hidden(glm::vec3 pos, std::pair<int, bool> (&info)[CHUNK_X][CHUNK_Y][CHUNK_Z]);
	void	disableHiddenCubes(std::pair<int, bool> (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z]);
	void	setPile(std::pair<int, bool> (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z]);

public:
	chunkFactory();
	~chunkFactory();

	void		setCubeFactory(cubeFactory& cubeFactory);
	s_chunk		getChunk(glm::vec3 pos);
};

#endif //!CHUNKFACTORY_HH_
