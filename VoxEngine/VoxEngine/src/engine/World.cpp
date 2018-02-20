
#include "World.h"

World::World()
{
	_chunkFactory.setCubeFactory(_cubeFactory);
	for (std::size_t x(0); x < CHUNK_SIZE_X; ++x)
		for (std::size_t y(0); y < CHUNK_SIZE_Y; ++y)
			for (std::size_t z(0); z < CHUNK_SIZE_Z; ++z)
				_chunks[x][y][z] = _chunkFactory.getChunk(glm::vec3(x, y, z));

	_sun = Light(_cubeFactory.getCube(cubeFactory::SUN, glm::vec3(100, 100, 100), -1), glm::vec3(1, 1, 1), 50, 0.5);
	_sun._entity.setModelMatrix();

	int onGrass = 0;

	for (int y(CHUNK_SIZE_Y * CHUNK_Y - 1); y > 0; --y)
		if (this->getBlockOnChunk(glm::vec3((CHUNK_X * CHUNK_SIZE_X) / 2, y, (CHUNK_Z * CHUNK_SIZE_Z) / 2)) == cubeFactory::GRASS)
		{
			onGrass = y + 1;
		}

	_player = _cubeFactory.getCube(cubeFactory::PLAYER, glm::vec3(
		(CHUNK_X * CHUNK_SIZE_X) / 2,
		onGrass,
		(CHUNK_Z * CHUNK_SIZE_Z) / 2), -1);

	_axis.push_back(_cubeFactory.getLine(glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -M_PI / 2), -1));
	_axis.push_back(_cubeFactory.getLine(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), -1));
	_axis.push_back(_cubeFactory.getLine(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(M_PI / 2, 0, 0), -1));

	_axis[0]._scale = glm::vec3(0.05, 0.05, 0.05);
	_axis[0].setModelMatrix();
	_axis[1]._scale = glm::vec3(0.05, 0.05, 0.05);
	_axis[1].setModelMatrix();
	_axis[2]._scale = glm::vec3(0.05, 0.05, 0.05);
	_axis[2].setModelMatrix();

	_win.x = 1280; _win.y = 720;

	_text.push_back(_fontFactory.getText(std::string("FPS: Loading"), this->getScreenPos(glm::vec3(10, 10, 0)), FontFactory::FPS));
	_text.push_back(_fontFactory.getText(std::string("PLAYER XYZ: Loading"), this->getScreenPos(glm::vec3(10, 50, 0)), FontFactory::XYZ));
	_text.push_back(_fontFactory.getText(std::string("CHUNK XYZ: Loading"), this->getScreenPos(glm::vec3(10, 30, 0)), FontFactory::CHUNK));
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

int	World::getBlockOnChunk(glm::vec3 pos)
{
	return (_chunks[(int)(pos.x / CHUNK_X)][(int)(pos.y / CHUNK_Y)][(int)(pos.z / CHUNK_Z)].chunkInfos[(int)pos.x % CHUNK_X][(int)pos.y % CHUNK_Y][(int)pos.z % CHUNK_Z].first);
}

glm::vec3 World::getScreenPos(const glm::vec3 pos)
{
	glm::vec3 res(pos.x / _win.x * 2, -pos.y / _win.y * 2, 0);

	return (res);
}

void	World::setWin(glm::vec2& win)
{
	_win.x = win.x;
	_win.y = win.y;
}

void	World::update(float elapsed, Camera& cam, const int fps, const bool debug)
{
	if (debug == true)
	{
		glm::vec3 pos = _player._pos;

		_text[FontFactory::XYZ] = _fontFactory.getText(std::string("PLAYER XYZ: "
			+ std::to_string(pos.x) + "; "
			+ std::to_string(pos.y) + "; "
			+ std::to_string(pos.z)), this->getScreenPos(glm::vec3(10, 50, 0)), FontFactory::XYZ);

		_text[FontFactory::CHUNK] = _fontFactory.getText(std::string("CHUNK XYZ: "
			+ std::to_string((int)(pos.x / CHUNK_X)) + "; "
			+ std::to_string((int)(pos.y / CHUNK_Y)) + "; "
			+ std::to_string((int)(pos.z / CHUNK_Z))), this->getScreenPos(glm::vec3(10, 30, 0)), FontFactory::CHUNK);
		
		if (fps == -1)
			_text[FontFactory::FPS] = _fontFactory.getText(std::string("FPS: Loading"), this->getScreenPos(glm::vec3(10, 10, 0)), FontFactory::FPS);
		else
			_text[FontFactory::FPS] = _fontFactory.getText(std::string("FPS: " + std::to_string(fps)), this->getScreenPos(glm::vec3(10, 10, 0)), FontFactory::FPS);
		if (fps > 200)
			_text[FontFactory::FPS]._colour = glm::vec3(0, 1, 0);
		else if (fps > 100)
			_text[FontFactory::FPS]._colour = glm::vec3(1, 0.8, 0);
		else if (fps == -1)
			_text[FontFactory::FPS]._colour = glm::vec3(0.1, 0, 1);
		else
			_text[FontFactory::FPS]._colour = glm::vec3(1, 0, 0);

		_axis[0]._pos = cam._pos + cam._dir;
		_axis[0].setModelMatrix();
		_axis[1]._pos = cam._pos + cam._dir;
		_axis[1].setModelMatrix();
		_axis[2]._pos = cam._pos + cam._dir;
		_axis[2].setModelMatrix();
	}
	return;
}