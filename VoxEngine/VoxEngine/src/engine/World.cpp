
#include "World.h"

World::World()
{
	_chunks.push_back(_cubeFactory.getCube(cubeFactory::GRASS, glm::vec3(0, 0, 0)));
	_sun = new Light(_cubeFactory.getCube(cubeFactory::SUN, glm::vec3(20, 20, 20)), glm::vec3(1, 1, 1), 50, 0.5);
	_player = _cubeFactory.getCube(cubeFactory::PLAYER, glm::vec3(0, 0, 5));
}

World::~World()
{

}

Light	*World::getSun(void)
{
	return _sun;
}

Entity	*World::getPlayer(void)
{
	return _player;
}

std::vector<Entity*>	World::getChunks(void)
{
	return _chunks;
}

void	World::update(float elapsed)
{
	return;
}