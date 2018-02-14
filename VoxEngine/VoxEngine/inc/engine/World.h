#ifndef WORLD_H_
# define WORLD_H_

# include "VAOLoader.h"
# include "Entity.h"
# include "cubeFactory.h"
# include "Light.h"
# include "Camera.h"
# include "FontFactory.h"

class	World
{
private:
	cubeFactory		_cubeFactory;
	FontFactory		_fontFactory;
	
	Light					*_sun;
	Entity					*_player;
	std::vector<Entity*>	_chunks;
	std::vector<Entity*>	_axis;
	std::vector<Entity*>	_text;
public:
	World(const int winx, const int winy);
	~World();

	void	update(float elapsed, Camera *camera, const int fps, const bool debug);

	Light					*getSun(void);
	Entity					*getPlayer(void);
	std::vector<Entity*>	getAxis(void);
	std::vector<Entity*>	getChunks(void);
	std::vector<Entity*>	getText(void);

	glm::vec3				getScreenPos(const glm::vec3 pos);

	glm::vec2				_win;

	//std::vector<Entity*>	getWaters(void);
	//std::vector<Entity*>	getClouds(void);
	//std::vector<Entity*>	getFlora(void);
	//std::vector<Entity*>	getFauna(void);
};

#endif //!WORLD_H_