
#include "chunkFactory.h"

chunkFactory::chunkFactory() {}

chunkFactory::~chunkFactory() {}

void		chunkFactory::setCubeFactory(cubeFactory& cubeFactory)
{
	_cubeFactory = cubeFactory;
}

bool	chunkFactory::test_hidden(glm::vec3 pos, std::pair<int, bool> (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z])
{
	if (pos.x == 0 || pos.x == (CHUNK_X - 1) ||
		pos.y == 0 || pos.y == (CHUNK_Y - 1) ||
		pos.z == 0 || pos.z == (CHUNK_Z - 1))
		return false;

	int top = chunk[(int)pos.x][(int)pos.y + 1][(int)pos.z].first;
	int bot = chunk[(int)pos.x][(int)pos.y - 1][(int)pos.z].first;
	int right = chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z].first;
	int left = chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z].first;
	int front = chunk[(int)pos.x][(int)pos.y][(int)pos.z - 1].first;
	int back = chunk[(int)pos.x][(int)pos.y][(int)pos.z + 1].first;

	if (top != cubeFactory::WATER && top != cubeFactory::VOID &&
		bot != cubeFactory::WATER && bot != cubeFactory::VOID &&
		right != cubeFactory::WATER && right != cubeFactory::VOID &&
		left != cubeFactory::WATER && left != cubeFactory::VOID &&
		front != cubeFactory::WATER && front != cubeFactory::VOID &&
		back != cubeFactory::WATER && back != cubeFactory::VOID)
		return true;

	return false;
}

void	chunkFactory::disableHiddenCubes(std::pair<int, bool> (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z])
{
	glm::vec3	pos;

	for (int x = 0; x < CHUNK_X; x++)
		for (int y = 0; y < CHUNK_Y; y++)
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (this->test_hidden(glm::vec3(x, y, z), chunk) == true)
				{
					chunk[x][y][z].second = false;
				}
			}
}

void	chunkFactory::setPile(std::pair<int, bool> (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z], glm::vec3& pos)
{
	int res = -1;

	for (float x = 0; x < CHUNK_X; x++)
		for (float y = 0; y < CHUNK_Y; y++)
			for (float z = 0; z < CHUNK_Z; z++)
			{
				if ((y == (CHUNK_Y - 1)
					&& chunk[(int)x][(int)y][(int)z].first == cubeFactory::DIRT)
					&& powf(((_perlin.octaveNoise(	(x + (pos.x * CHUNK_X)) / (CHUNK_X * 4) * 2,
													(y + 1.f + (pos.y * CHUNK_Y)) / (CHUNK_Y * 4) * 2,
													(z + (pos.z * CHUNK_Z)) / (CHUNK_Z * 4) * 2, 2) + 1.f) / 2.f), (y + 1.f + (pos.y * CHUNK_Y)) / 10.f) <= 0.5)
				{
					chunk[(int)x][(int)y][(int)z].first = cubeFactory::GRASS;
				}
				else if ((y < (CHUNK_Y - 1) && chunk[(int)x][(int)y][(int)z].first == cubeFactory::DIRT && chunk[(int)x][(int)y + 1][(int)z].first == cubeFactory::VOID))
				{
					chunk[(int)x][(int)y][(int)z].first = cubeFactory::GRASS;
				}
			}
}


s_chunk chunkFactory::getChunk(glm::vec3 pos)
{
	s_chunk		res;


	//CONSTRUCT CHUNK INFOS
	for (float x = 0; x < CHUNK_X; x++)
		for (float y = 0; y < CHUNK_Y; y++)
			for (float z = 0; z < CHUNK_Z; z++)
			{
				if (powf(((_perlin.octaveNoise(	(x + (pos.x * CHUNK_X)) / (CHUNK_X * 4) * 2,
												(y + (pos.y * CHUNK_Y)) / (CHUNK_Y * 4) * 2,
												(z + (pos.z * CHUNK_Z)) / (CHUNK_Z * 4) * 2, 2) + 1.f) / 2.f), (y + (pos.y * CHUNK_Y)) / 10.f) > 0.5)
				{
					res.chunkInfos[(int)x][(int)y][(int)z].first = cubeFactory::DIRT;
					res.chunkInfos[(int)x][(int)y][(int)z].second = true;
				}
				else
				{
					res.chunkInfos[(int)x][(int)y][(int)z].first = cubeFactory::VOID;
					res.chunkInfos[(int)x][(int)y][(int)z].second = false;
				}
			}
	
	this->disableHiddenCubes(res.chunkInfos);
	this->setPile(res.chunkInfos, pos);

	res.Chunk = _cubeFactory.getChunk(glm::vec3(pos.x * CHUNK_X, pos.y * CHUNK_Y, pos.z * CHUNK_Z), res.chunkInfos, -1);

	return res;
}