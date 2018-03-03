
#include "World.h"

World::World()
{
	_chunkFactory.setCubeFactory(_cubeFactory);
	for (std::size_t x(0); x < CHUNK_SIZE_X; ++x)
		for (std::size_t y(0); y < CHUNK_SIZE_Y; ++y)
			for (std::size_t z(0); z < CHUNK_SIZE_Z; ++z)
				_chunks[x][y][z] = _chunkFactory.getChunk(glm::vec3(x, y, z), -1);

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

	_wirelessCube = _cubeFactory.getWirelessCube(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), -1);

	_win.x = 1280; _win.y = 720;

	_text.push_back(_fontFactory.getText(std::string("FPS: Loading"), this->getScreenPos(glm::vec3(10, 10, 0)), FontFactory::FPS));
	_text.push_back(_fontFactory.getText(std::string("PLAYER XYZ: Loading"), this->getScreenPos(glm::vec3(10, 50, 0)), FontFactory::XYZ));
	_text.push_back(_fontFactory.getText(std::string("CHUNK XYZ: Loading"), this->getScreenPos(glm::vec3(10, 30, 0)), FontFactory::CHUNK));

	_deltaPlayer = glm::vec3(0, 0, 0);
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

Entity	&World::getWirelessCube(void)
{
	return _wirelessCube;
}

int	World::getBlockOnChunk(glm::vec3 pos)
{
	pos += _deltaPlayer;
	return (_chunks[(int)(pos.x / CHUNK_X)][(int)(pos.y / CHUNK_Y)][(int)(pos.z / CHUNK_Z)].chunkInfos[(int)pos.x % CHUNK_X][(int)pos.y % CHUNK_Y][(int)pos.z % CHUNK_Z].first);
}

std::pair<int, bool>& World::getChunkInfos(glm::vec3 pos)
{
	pos += _deltaPlayer;
	return (_chunks[(int)(pos.x / CHUNK_X)][(int)(pos.y / CHUNK_Y)][(int)(pos.z / CHUNK_Z)].chunkInfos[(int)pos.x % CHUNK_X][(int)pos.y % CHUNK_Y][(int)pos.z % CHUNK_Z]);
}

void	World::updateBlock(glm::vec3 pos, int type, bool draw)
{
	if (pos == glm::vec3(0, 0, 0))
		return;
	if (draw == false)
	{
		this->getChunkInfos(pos).first = type;
		this->getChunkInfos(pos).second = true;

		if (_player.canMove(this) == false)
		{
			this->getChunkInfos(pos).first = 0;
			this->getChunkInfos(pos).second = false;
			return;
		}

		this->getChunkInfos(pos + glm::vec3(1, 0, 0)).second = true;
		this->getChunkInfos(pos - glm::vec3(1, 0, 0)).second = true;

		this->getChunkInfos(pos + glm::vec3(0, 1, 0)).second = true;
		this->getChunkInfos(pos - glm::vec3(0, 1, 0)).second = true;

		this->getChunkInfos(pos + glm::vec3(0, 0, 1)).second = true;
		this->getChunkInfos(pos - glm::vec3(0, 0, 1)).second = true;

		pos += _deltaPlayer;

		_chunks[(int)(pos.x / CHUNK_X)][(int)(pos.y / CHUNK_Y)][(int)(pos.z / CHUNK_Z)].Chunk
			= _cubeFactory.getChunk(
				glm::vec3(
				(int)((pos.x / CHUNK_X) + _deltaChunk.x) * CHUNK_X,
				(int)((pos.y / CHUNK_Y) + _deltaChunk.y) * CHUNK_Y,
				(int)((pos.z / CHUNK_Z) + _deltaChunk.z) * CHUNK_Z),
				_chunks[(int)(pos.x / CHUNK_X)][(int)(pos.y / CHUNK_Y)][(int)(pos.z / CHUNK_Z)].chunkInfos,
				_chunks[(int)(pos.x / CHUNK_X)][(int)(pos.y / CHUNK_Y)][(int)(pos.z / CHUNK_Z)].Chunk._model._rawModel._vao_id);
	}
	else
	{
		switch (_wirelessCubeFace)
		{
			case cubeFactory::RIGHT:
			{
				pos += glm::vec3(1, 0, 0);
				break;
			}
			case cubeFactory::LEFT:
			{
				pos -= glm::vec3(1, 0, 0);
				break;
			}
			case cubeFactory::TOP:
			{
				pos += glm::vec3(0, 1, 0);
				break;
			}
			case cubeFactory::BOT:
			{
				pos -= glm::vec3(0, 1, 0);
				break;
			}
			case cubeFactory::FRONT:
			{
				pos -= glm::vec3(0, 0, 1);
				break;
			}
			case cubeFactory::BACK:
			{
				pos += glm::vec3(0, 0, 1);
				break;
			}
		}
		this->getChunkInfos(pos).first = type;
		this->getChunkInfos(pos).second = true;

		if (_player.canMove(this) == false)
		{
			this->getChunkInfos(pos).first = 0;
			this->getChunkInfos(pos).second = false;
			return;
		}

		pos += _deltaPlayer;

		_chunks[(int)(pos.x / CHUNK_X)][(int)(pos.y / CHUNK_Y)][(int)(pos.z / CHUNK_Z)].Chunk
			= _cubeFactory.getChunk(
				glm::vec3(
				(int)((pos.x / CHUNK_X) + _deltaChunk.x) * CHUNK_X,
				(int)((pos.y / CHUNK_Y) + _deltaChunk.y) * CHUNK_Y,
				(int)((pos.z / CHUNK_Z) + _deltaChunk.z) * CHUNK_Z),
				_chunks[(int)(pos.x / CHUNK_X)][(int)(pos.y / CHUNK_Y)][(int)(pos.z / CHUNK_Z)].chunkInfos,
				_chunks[(int)(pos.x / CHUNK_X)][(int)(pos.y / CHUNK_Y)][(int)(pos.z / CHUNK_Z)].Chunk._model._rawModel._vao_id);
	}
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

glm::vec3	World::getWirelessCubePos(Camera& cam)
{
	static glm::vec3	picking;
	int					lastCube;

	for (float i(0); i <= 5; i+= 0.05f)
	{
		if (cam._tps == true)
		{
			picking = glm::vec3(cam._lookAt + (cam._dir * i));
		}
		else
		{
			picking = glm::vec3(cam._pos + (cam._dir * i));
		}
		picking = glm::vec3((int)(picking.x + 0.5f), (int)(picking.y + 0.5f), (int)(picking.z + 0.5f));

		lastCube = this->getChunkInfos(picking).first;

		this->getChunkInfos(picking).first = cubeFactory::DIRT;

		if (_player.canMove(this) == true && lastCube > cubeFactory::WATER)
		{
			this->getChunkInfos(picking).first = lastCube;

			//SET FACE
			static glm::vec3 dis;

			if (cam._tps == true)
			{
				picking = glm::vec3(cam._lookAt + (cam._dir * i));
				dis = picking - glm::vec3((int)(picking.x + 0.5f), (int)(picking.y + 0.5f), (int)(picking.z + 0.5f));
			}
			else
			{
				picking = glm::vec3(cam._pos + (cam._dir * i));
				dis = picking - glm::vec3((int)(picking.x + 0.5f), (int)(picking.y + 0.5f), (int)(picking.z + 0.5f));
			}
			if (fabs(dis.x) > fabs(dis.y) && fabs(dis.x) > fabs(dis.z))
			{
				if (dis.x >= 0)
					_wirelessCubeFace = cubeFactory::RIGHT;
				else
					_wirelessCubeFace = cubeFactory::LEFT;
			}
			else if (fabs(dis.y) > fabs(dis.x) && fabs(dis.y) > fabs(dis.z))
			{
				if (dis.y >= 0)
					_wirelessCubeFace = cubeFactory::TOP;
				else
					_wirelessCubeFace = cubeFactory::BOT;
			}
			else if (fabs(dis.z) > fabs(dis.y) && fabs(dis.z) > fabs(dis.x))
			{
				if (dis.z >= 0)
					_wirelessCubeFace = cubeFactory::BACK;
				else
					_wirelessCubeFace = cubeFactory::FRONT;
			}
			picking = glm::vec3((int)(picking.x + 0.5f), (int)(picking.y + 0.5f), (int)(picking.z + 0.5f));
			//!SET FACE

			return picking;
		}
		this->getChunkInfos(picking).first = lastCube;
	}

	return glm::vec3(0, 0, 0);
}

void	World::updateChunksCenter(void)
{
	static glm::vec3 current_chunk = glm::vec3((int)(_player._pos.x / CHUNK_SIZE_X), (int)(_player._pos.y / CHUNK_SIZE_Y), (int)(_player._pos.z / CHUNK_SIZE_Z));


	// X CHUNK changes
	if ((int)(_player._pos.x / CHUNK_SIZE_X) > current_chunk.x)
	{
		_deltaPlayer.x -= CHUNK_X;
		++_deltaChunk.x;
		
		for (std::size_t x(0); x < CHUNK_SIZE_X; ++x)
			for (std::size_t y(0); y < CHUNK_SIZE_Y; ++y)
				for (std::size_t z(0); z < CHUNK_SIZE_Z; ++z)
				{
					if (x == 0)
						_cubeFactory._loader.deleteVAO(_chunks[x][y][z].Chunk._model._rawModel._vao_id);
					if (x < (CHUNK_SIZE_X - 1))
					{
						_chunks[x][y][z] = _chunks[x + 1][y][z];
					}
					else
					{
						_chunks[x][y][z] = _chunkFactory.getChunk(glm::vec3(x, y, z) + _deltaChunk, -1);
					}
				}
		current_chunk.x = (int)(_player._pos.x / CHUNK_SIZE_X);
	}
	else if ((int)(_player._pos.x / CHUNK_SIZE_X) < current_chunk.x)
	{
		_deltaPlayer.x += CHUNK_X;
		--_deltaChunk.x;

		for (std::size_t x(CHUNK_SIZE_X - 1); x > 0; --x)
			for (std::size_t y(0); y < CHUNK_SIZE_Y; ++y)
				for (std::size_t z(0); z < CHUNK_SIZE_Z; ++z)
				{
					if (x == CHUNK_SIZE_X - 1)
						_cubeFactory._loader.deleteVAO(_chunks[x][y][z].Chunk._model._rawModel._vao_id);
					if (x > 1)
					{
						_chunks[x][y][z] = _chunks[x - 1][y][z];
					}
					else
					{
						_chunks[x][y][z] = _chunkFactory.getChunk(glm::vec3(x, y, z) + _deltaChunk, -1);
					}
				}
		current_chunk.x = (int)(_player._pos.x / CHUNK_SIZE_X);
	}
	// Z CHUNK changes
	if ((int)(_player._pos.z / CHUNK_SIZE_Z) > current_chunk.z)
	{
		_deltaPlayer.z -= CHUNK_Z;
		++_deltaChunk.z;

		for (std::size_t x(0); x < CHUNK_SIZE_X; ++x)
			for (std::size_t y(0); y < CHUNK_SIZE_Y; ++y)
				for (std::size_t z(0); z < CHUNK_SIZE_Z; ++z)
				{
					if (z == 0)
						_cubeFactory._loader.deleteVAO(_chunks[x][y][z].Chunk._model._rawModel._vao_id);
					if (z < (CHUNK_SIZE_Z - 1))
					{
						_chunks[x][y][z] = _chunks[x][y][z + 1];
					}
					else
					{
						_chunks[x][y][z] = _chunkFactory.getChunk(glm::vec3(x, y, z) + _deltaChunk, -1);
					}
				}
		current_chunk.z = (int)(_player._pos.z / CHUNK_SIZE_Z);
	}
	else if ((int)(_player._pos.z / CHUNK_SIZE_Z) < current_chunk.z)
	{
		_deltaPlayer.z += CHUNK_Z;
		--_deltaChunk.z;

		for (std::size_t x(0); x < CHUNK_SIZE_X; ++x)
			for (std::size_t y(0); y < CHUNK_SIZE_Y; ++y)
				for (std::size_t z(CHUNK_SIZE_Z - 1); z > 0; --z)
				{
					if (z == CHUNK_SIZE_Z - 1)
						_cubeFactory._loader.deleteVAO(_chunks[x][y][z].Chunk._model._rawModel._vao_id);
					if (z > 1)
					{
						_chunks[x][y][z] = _chunks[x][y][z - 1];
					}
					else
					{
						_chunks[x][y][z] = _chunkFactory.getChunk(glm::vec3(x, y, z) + _deltaChunk, -1);
					}
				}
		current_chunk.z = (int)(_player._pos.z / CHUNK_SIZE_Z);
	}
}

void	World::update(float elapsed, Camera& cam, const int fps, const bool debug)
{
	_wirelessCube._pos = this->getWirelessCubePos(cam);
	_wirelessCube.setModelMatrix();

	this->updateChunksCenter();

	if (debug == true)
	{
		glm::vec3	pos = _player._pos;

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
}