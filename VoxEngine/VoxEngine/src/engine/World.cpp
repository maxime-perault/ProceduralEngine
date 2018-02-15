
#include "World.h"

World::World() {}

World::World(const int winx, const int winy)
{
	_chunkFactory = new chunkFactory(_cubeFactory);
	for (std::size_t x(0); x < 4; ++x)
		for (std::size_t z(0); z < 4; ++z)
		{
			_chunks.push_back(_chunkFactory->getChunk(glm::vec3(x, 0, z)));
		}

	_sun = Light(_cubeFactory.getCube(cubeFactory::SUN, glm::vec3(20, 20, 20)), glm::vec3(1, 1, 1), 50, 0.5);
	_player = _cubeFactory.getCube(cubeFactory::PLAYER, glm::vec3(0, 0, 5));

	_axis.push_back(_cubeFactory.getLine(glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -M_PI / 2)));
	_axis.push_back(_cubeFactory.getLine(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)));
	_axis.push_back(_cubeFactory.getLine(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(M_PI / 2, 0, 0)));

	_axis[0]._scale = glm::vec3(0.05, 0.05, 0.05);
	_axis[1]._scale = glm::vec3(0.05, 0.05, 0.05);
	_axis[2]._scale = glm::vec3(0.05, 0.05, 0.05);

	_win.x = winx; _win.y = winy;

	_text.push_back(_fontFactory.getText(std::string("FPS: Loading"), this->getScreenPos(glm::vec3(10, 10, 0)), FontFactory::FPS));
	_text.push_back(_fontFactory.getText(std::string("XYZ: Loading"), this->getScreenPos(glm::vec3(10, 30, 0)), FontFactory::XYZ));
}

World::~World()
{

}

Entity&		World::getPlayer(void)
{
	return _player;
}

Light&		World::getSun(void)
{
	return _sun;
}

std::vector<Entity>&	World::getAxis(void)
{
	return _axis;
}
std::vector<Entity>&	World::getText(void)
{
	return _text;
}

std::vector<s_chunk>&	World::getChunks(void)
{
	return _chunks;
}

glm::vec3 World::getScreenPos(const glm::vec3 pos)
{
	glm::vec3 res(pos.x / _win.x * 2, -pos.y / _win.y * 2, 0);

	return (res);
}

void	World::update(float elapsed, Camera& cam, const int fps, const bool debug)
{
	if (debug == true)
	{
		glm::vec3 pos = _player._pos;

		_text[FontFactory::XYZ] = _fontFactory.getText(std::string("XYZ: "
			+ std::to_string(pos.x) + "; "
			+ std::to_string(pos.y) + "; "
			+ std::to_string(pos.z)), this->getScreenPos(glm::vec3(10, 30, 0)), FontFactory::XYZ);
		if (fps == -1)
			_text[FontFactory::FPS] = _fontFactory.getText(std::string("FPS: Loading"), this->getScreenPos(glm::vec3(10, 10, 0)), FontFactory::FPS);
		else
			_text[FontFactory::FPS] = _fontFactory.getText(std::string("FPS: " + std::to_string(fps)), this->getScreenPos(glm::vec3(10, 10, 0)), FontFactory::FPS);
		if (fps > 200)
			_text[FontFactory::FPS]._colour = glm::vec3(0, 1, 0);
		else if (fps > 100)
			_text[FontFactory::FPS]._colour = glm::vec3(1, 0.8, 0);
		else
			_text[FontFactory::FPS]._colour = glm::vec3(1, 0, 0);

		_axis[0]._pos = cam._pos + cam._dir;
		_axis[1]._pos = cam._pos + cam._dir;
		_axis[2]._pos = cam._pos + cam._dir;
	}
	return;
}