#ifndef WORLD_H_
# define WORLD_H_

# include <thread>
# include <future>
# include "VAOLoader.h"
# include "Entity.h"
# include "cubeFactory.h"
# include "chunkFactory.h"
# include "FontFactory.h"
# include "Light.h"
# include "Camera.h"
# include "FBO.h"

# define CHUNK_SIZE_X 13
# define CHUNK_SIZE_Y 6
# define CHUNK_SIZE_Z 13

void	initThread();

class	World
{
public:
	FontFactory				_fontFactory;
	
	FBO		_fbo;

	Entity					_wirelessCube;
	int						_wirelessCubeFace;
	Light					_sun;
	Entity					_player;
	std::vector<Entity>		_axis;
	std::vector<Entity>		_text;
	s_chunk					_gen_chunks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	s_chunk					_chunks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];

	Entity		_depth;

	World();
	~World();

	void	setWin(glm::vec2& win);
	void	update(float elapsed, Camera& camera, const int fps, const bool debug);
	void	updateBlock(glm::vec3 pos, int type, bool draw);
	int		checkForChunksMove();

	Light	&getSun(void);
	Entity	&getPlayer(void);
	Entity	&getWirelessCube(void);

	glm::vec3				getWirelessCubePos(Camera& cam);
	std::pair<int, bool>&	getChunkInfos(glm::vec3 pos);
	int						getBlockOnChunk(glm::vec3 pos);
	std::vector<Entity>		&getAxis(void);
	std::vector<Entity>		&getText(void);

	void	updateChunk(glm::vec3 pos, bool recursive);

	glm::vec3	getScreenPos(const glm::vec3 pos);

	glm::vec2	_win;
	glm::vec3	_deltaPlayer;
	glm::vec3	_deltaChunk;
};

#endif //!WORLD_H_