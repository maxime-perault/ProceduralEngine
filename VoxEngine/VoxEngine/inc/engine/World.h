#ifndef WORLD_H_
# define WORLD_H_

# include "VAOLoader.h"
# include "Entity.h"
# include "cubeFactory.h"
# include "chunkFactory.h"
# include "FontFactory.h"
# include "Light.h"
# include "Camera.h"

class	World
{
private:
	cubeFactory		_cubeFactory;
	chunkFactory	*_chunkFactory;
	FontFactory		_fontFactory;
	
	Light					*_sun;
	Entity					*_player;
	std::vector<Entity*>	_axis;
	std::vector<Entity*>	_text;
	std::vector<s_chunk>	_chunks;

public:
	World(const int winx, const int winy);
	~World();

	void	update(float elapsed, Camera *camera, const int fps, const bool debug);

	Light					*getSun(void);
	Entity					*getPlayer(void);
	std::vector<Entity*>	getAxis(void);
	std::vector<Entity*>	getText(void);
	std::vector<s_chunk>	getChunks(void);

	glm::vec3	getScreenPos(const glm::vec3 pos);

	glm::vec2				_win;

	//std::vector<Entity*>	getWaters(void);
	//std::vector<Entity*>	getClouds(void);
	//std::vector<Entity*>	getFlora(void);
	//std::vector<Entity*>	getFauna(void);
};

#endif //!WORLD_H_