
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
	cubeFactory*	_cubeFactory;
	PerlinNoise _perlin;

	bool	isSolid(glm::vec3 pos, glm::vec3 chunk_pos);

	bool	test_hidden(glm::vec3& pos, glm::vec3& chunk_pos, std::pair<int, bool> (&info)[CHUNK_X][CHUNK_Y][CHUNK_Z]);
	void	disableHiddenCubes(std::pair<int, bool> (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z], glm::vec3& pos);
	void	setPile(std::pair<int, bool> (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z], glm::vec3& chunk_pos);
	void	mixUpDirt(std::pair<int, bool>(&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z], glm::vec3& chunk_pos);

	void	perlinWorm(std::pair<int, bool>(&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z], glm::vec3& chunk_pos);

public:
	chunkFactory();
	~chunkFactory();

	void		setCubeFactory(cubeFactory* cubeFactory);
	s_chunk		getChunk(glm::vec3 pos, int vao, bool reload);
};

#endif //!CHUNKFACTORY_HH_
