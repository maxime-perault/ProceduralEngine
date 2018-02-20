#ifndef WORLD_H_
# define WORLD_H_

# include "VAOLoader.h"
# include "Entity.h"
# include "cubeFactory.h"
# include "chunkFactory.h"
# include "FontFactory.h"
# include "Light.h"
# include "Camera.h"

# define CHUNK_SIZE_X 13
# define CHUNK_SIZE_Y 6
# define CHUNK_SIZE_Z 13

class	World
{
public:
	cubeFactory		_cubeFactory;
	chunkFactory	_chunkFactory;
	FontFactory		_fontFactory;

	Light					_sun;
	Entity					_player;
	std::vector<Entity>		_axis;
	std::vector<Entity>		_text;
	s_chunk					_chunks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];

	World();
	~World();

	void	setWin(glm::vec2& win);
	void	update(float elapsed, Camera& camera, const int fps, const bool debug);

	Light	&getSun(void);
	Entity	&getPlayer(void);
	int						getBlockOnChunk(glm::vec3 pos);
	std::vector<Entity>		&getAxis(void);
	std::vector<Entity>		&getText(void);

	glm::vec3	getScreenPos(const glm::vec3 pos);

	glm::vec2				_win;

	//std::vector<Entity*>	getWaters(void);
	//std::vector<Entity*>	getClouds(void);
	//std::vector<Entity*>	getFlora(void);
	//std::vector<Entity*>	getFauna(void);
};

#endif //!WORLD_H_