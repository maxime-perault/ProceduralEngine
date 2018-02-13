#ifndef WORLD_H_
# define WORLD_H_

# include "VAOLoader.h"
# include "Entity.h"
# include "cubeFactory.h"
# include "Light.h"

class	World
{
private:
	cubeFactory		_cubeFactory;
	
	Light					*_sun;
	Entity					*_player;
	std::vector<Entity*>	_chunks;
public:
	World();
	~World();

	void	update(float elapsed);

	Light					*getSun(void);
	Entity					*getPlayer(void);
	std::vector<Entity*>	getChunks(void);

	//std::vector<Entity*>	getWaters(void);
	//std::vector<Entity*>	getClouds(void);
	//std::vector<Entity*>	getFlora(void);
	//std::vector<Entity*>	getFauna(void);
};

#endif //!WORLD_H_