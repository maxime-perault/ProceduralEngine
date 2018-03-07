
#include "chunkFactory.h"
#include <ctime>

chunkFactory::chunkFactory()
{
	_perlin.reseed(time(NULL));
}

chunkFactory::~chunkFactory() {}

void		chunkFactory::setCubeFactory(cubeFactory* cubeFactory)
{
	_cubeFactory = cubeFactory;
	_cubeFactory->setTerrain();
}

float	chunkFactory::getPerlin(glm::vec3 pos, glm::vec3 chunk_pos)
{
	return powf(((_perlin.octaveNoise((pos.x + (chunk_pos.x * CHUNK_X)) / (CHUNK_X * 4) * 2,
		(pos.y + (chunk_pos.y * CHUNK_Y)) / (CHUNK_Y * 4) * 2,
		(pos.z + (chunk_pos.z * CHUNK_Z)) / (CHUNK_Z * 4) * 2, 2) + 1.f) / 2.f), (pos.y + (chunk_pos.y * CHUNK_Y)) / 15.f);
}

//determine if the block is Air or Solid
bool	chunkFactory::isSolid(glm::vec3 pos, glm::vec3 chunk_pos)
{
	if (this->getPerlin(pos, chunk_pos) > 0.5)
		return true;
	return false;
}

//test if the block can be visible
bool	chunkFactory::test_hidden(glm::vec3& pos, glm::vec3& chunk_pos, std::pair<int, bool> (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z])
{
	static int top, bot, right, left, front, back;

	//Y AXIS
	if (pos.y == (CHUNK_Y - 1))
	{
		if (this->isSolid(glm::vec3(pos.x, pos.y + 1.f, pos.z), chunk_pos) == false)
			top = cubeFactory::VOID;
		else
			top = cubeFactory::DIRT;
		bot = chunk[(int)pos.x][(int)pos.y - 1][(int)pos.z].first;
	}
	else if (pos.y == 0)
	{
		if (this->isSolid(glm::vec3(pos.x, pos.y - 1.f, pos.z), chunk_pos) == false)
			bot = cubeFactory::VOID;
		else
			bot = cubeFactory::DIRT;
		top = chunk[(int)pos.x][(int)pos.y + 1][(int)pos.z].first;
	}
	else
	{
		top = chunk[(int)pos.x][(int)pos.y + 1][(int)pos.z].first;
		bot = chunk[(int)pos.x][(int)pos.y - 1][(int)pos.z].first;
	}

	//X AXIS
	if (pos.x == (CHUNK_X - 1))
	{
		if (this->isSolid(glm::vec3(pos.x + 1.f, pos.y, pos.z), chunk_pos) == false)
			right = cubeFactory::VOID;
		else
			right = cubeFactory::DIRT;
		left = chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z].first;
	}
	else if (pos.x == 0)
	{
		if (this->isSolid(glm::vec3(pos.x - 1.f, pos.y, pos.z), chunk_pos) == false)
			left = cubeFactory::VOID;
		else
			left = cubeFactory::DIRT;
		right = chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z].first;
	}
	else
	{
		right = chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z].first;
		left = chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z].first;
	}

	//Z AXIS
	if (pos.z == (CHUNK_Z - 1))
	{
		if (this->isSolid(glm::vec3(pos.x, pos.y, pos.z + 1.f), chunk_pos) == false)
			back = cubeFactory::VOID;
		else
			back = cubeFactory::DIRT;
		front = chunk[(int)pos.x][(int)pos.y][(int)pos.z - 1].first;
	}
	else if (pos.z == 0)
	{
		if (this->isSolid(glm::vec3(pos.x, pos.y, pos.z - 1.f), chunk_pos) == false)
			front = cubeFactory::VOID;
		else
			front = cubeFactory::DIRT;
		back = chunk[(int)pos.x][(int)pos.y][(int)pos.z + 1].first;
	}
	else
	{
		front = chunk[(int)pos.x][(int)pos.y][(int)pos.z - 1].first;
		back = chunk[(int)pos.x][(int)pos.y][(int)pos.z + 1].first;
	}

	if (top != cubeFactory::WATER && top != cubeFactory::VOID &&
		bot != cubeFactory::WATER && bot != cubeFactory::VOID &&
		right != cubeFactory::WATER && right != cubeFactory::VOID &&
		left != cubeFactory::WATER && left != cubeFactory::VOID &&
		front != cubeFactory::WATER && front != cubeFactory::VOID &&
		back != cubeFactory::WATER && back != cubeFactory::VOID)
		return true;

	return false;
}

//disable non visible blocks
void	chunkFactory::disableHiddenCubes(std::pair<int, bool> (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z], glm::vec3& pos)
{
	for (int x = 0; x < CHUNK_X; x++)
		for (int y = 0; y < CHUNK_Y; y++)
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (this->test_hidden(glm::vec3(x, y, z), pos, chunk) == true)
				{
					chunk[x][y][z].second = false;
				}
			}
}

void	chunkFactory::genTree(std::pair<int, bool>(&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z], glm::vec3& pos)
{
	static int rand_val;
	static int foliage;

	int inc(1);
	int init_y(pos.y);

	if ((rand_val = (rand() % 3)) <= 1)
		foliage = cubeFactory::FOLIAGE_R;
	else if (rand_val == 2)
		foliage = cubeFactory::FOLIAGE_G;
	else
		foliage = cubeFactory::FOLIAGE_Y;

	while (inc <= 7)
	{
		if (inc <= 6)
		{
			chunk[(int)pos.x][(int)pos.y][(int)pos.z].first = cubeFactory::WOOD;
			chunk[(int)pos.x][(int)pos.y][(int)pos.z].second = true;
		}
		if (inc == 7)
		{
			chunk[(int)pos.x][(int)pos.y][(int)pos.z].first = foliage;
			chunk[(int)pos.x][(int)pos.y][(int)pos.z].second = true;
		}

		if (inc >= 4)
		{
			chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z].first = foliage;
			chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z].second = true;
			chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z].first = foliage;
			chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z].second = true;

			chunk[(int)pos.x][(int)pos.y][(int)pos.z - 1].first = foliage;
			chunk[(int)pos.x][(int)pos.y][(int)pos.z - 1].second = true;
			chunk[(int)pos.x][(int)pos.y][(int)pos.z + 1].first = foliage;
			chunk[(int)pos.x][(int)pos.y][(int)pos.z + 1].second = true;
		}

		if (inc >= 4 && inc <= 6)
		{
			chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z - 1].first = foliage;
			chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z - 1].second = true;
			chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z + 1].first = foliage;
			chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z + 1].second = true;

			chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z - 1].first = foliage;
			chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z - 1].second = true;
			chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z + 1].first = foliage;
			chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z + 1].second = true;
		}

		if (inc == 4 || inc == 5)
		{
			chunk[(int)pos.x - 2][(int)pos.y][(int)pos.z].first = foliage;
			chunk[(int)pos.x - 2][(int)pos.y][(int)pos.z].second = true;
			chunk[(int)pos.x - 2][(int)pos.y][(int)pos.z - 1].first = foliage;
			chunk[(int)pos.x - 2][(int)pos.y][(int)pos.z - 1].second = true;
			chunk[(int)pos.x - 2][(int)pos.y][(int)pos.z + 1].first = foliage;
			chunk[(int)pos.x - 2][(int)pos.y][(int)pos.z + 1].second = true;

			chunk[(int)pos.x + 2][(int)pos.y][(int)pos.z].first = foliage;
			chunk[(int)pos.x + 2][(int)pos.y][(int)pos.z].second = true;
			chunk[(int)pos.x + 2][(int)pos.y][(int)pos.z - 1].first = foliage;
			chunk[(int)pos.x + 2][(int)pos.y][(int)pos.z - 1].second = true;
			chunk[(int)pos.x + 2][(int)pos.y][(int)pos.z + 1].first = foliage;
			chunk[(int)pos.x + 2][(int)pos.y][(int)pos.z + 1].second = true;

			chunk[(int)pos.x][(int)pos.y][(int)pos.z - 2].first = foliage;
			chunk[(int)pos.x][(int)pos.y][(int)pos.z - 2].second = true;
			chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z - 2].first = foliage;
			chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z - 2].second = true;
			chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z - 2].first = foliage;
			chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z - 2].second = true;

			chunk[(int)pos.x][(int)pos.y][(int)pos.z + 2].first = foliage;
			chunk[(int)pos.x][(int)pos.y][(int)pos.z + 2].second = true;
			chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z + 2].first = foliage;
			chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z + 2].second = true;
			chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z + 2].first = foliage;
			chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z + 2].second = true;
		}

		if (inc == 4)
		{
			chunk[(int)pos.x - 2][(int)pos.y][(int)pos.z - 2].first = foliage;
			chunk[(int)pos.x - 2][(int)pos.y][(int)pos.z - 2].second = true;

			chunk[(int)pos.x - 2][(int)pos.y][(int)pos.z + 2].first = foliage;
			chunk[(int)pos.x - 2][(int)pos.y][(int)pos.z + 2].second = true;

			chunk[(int)pos.x + 2][(int)pos.y][(int)pos.z - 2].first = foliage;
			chunk[(int)pos.x + 2][(int)pos.y][(int)pos.z - 2].second = true;

			chunk[(int)pos.x + 2][(int)pos.y][(int)pos.z + 2].first = foliage;
			chunk[(int)pos.x + 2][(int)pos.y][(int)pos.z + 2].second = true;
		}
		
		
		pos.y = init_y + inc;
		++inc;
	}
}

//Set grass top of the dirt pile
void	chunkFactory::setPile(std::pair<int, bool> (&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z], glm::vec3& chunk_pos)
{
	int res = -1;
	
	for (float x = 0; x < CHUNK_X; x++)
		for (float y = 0; y < CHUNK_Y; y++)
			for (float z = 0; z < CHUNK_Z; z++)
			{
				if ((y == (CHUNK_Y - 1)
					&& chunk[(int)x][(int)y][(int)z].first == cubeFactory::DIRT)
					&& this->isSolid(glm::vec3(x, y + 1.f, z), chunk_pos) == false)
				{
					chunk[(int)x][(int)y][(int)z].first = cubeFactory::GRASS;
				}
				else if ((y < (CHUNK_Y - 1) && chunk[(int)x][(int)y][(int)z].first == cubeFactory::DIRT && chunk[(int)x][(int)y + 1][(int)z].first == cubeFactory::VOID))
				{
					chunk[(int)x][(int)y][(int)z].first = cubeFactory::GRASS;
					if ((rand() % 100) <= 1
						&& ((int)x <= (CHUNK_X - 3)) && ((int)x >= 2)
						&& ((int)z <= (CHUNK_Z - 3)) && ((int)z >= 2)
						&& ((int)y + 1) <= (CHUNK_Y - 7))
					{
						this->genTree(chunk, glm::vec3((int)x, (int)y + 1, (int)z));
					}
				}
			}
}

//Mix dirt with stones and other special blocks
void	chunkFactory::mixUpDirt(std::pair<int, bool>(&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z], glm::vec3& chunk_pos)
{
	for (float x = 0; x < CHUNK_X; x++)
		for (float y = 0; y < CHUNK_Y; y++)
			for (float z = 0; z < CHUNK_Z; z++)
			{
				if (chunk[(int)x][(int)y][(int)z].first == cubeFactory::DIRT)
				{
					if ((rand() % 100) <= 60
						&& (y + chunk_pos.y * CHUNK_Y) < 24)
					{
						chunk[(int)x][(int)y][(int)z].first = cubeFactory::STONE;
					}
					else if ((rand() % 100) <= 10)
					{
						chunk[(int)x][(int)y][(int)z].first = cubeFactory::STONE;
					}
				}
			}
	for (float x = 0; x < CHUNK_X; x++)
		for (float y = 0; y < CHUNK_Y; y++)
			for (float z = 0; z < CHUNK_Z; z++)
			{
				if (chunk[(int)x][(int)y][(int)z].first == cubeFactory::STONE)
				{
					if ((rand() % 100) <= 4 && (y + chunk_pos.y * CHUNK_Y) < 24)
					{
						chunk[(int)x][(int)y][(int)z].first = cubeFactory::COAL;
					}
				}
			}
}

void	chunkFactory::perlinWorm(std::pair<int, bool>(&chunk)[CHUNK_X][CHUNK_Y][CHUNK_Z], glm::vec3& chunk_pos)
{

}

s_chunk chunkFactory::getChunk(glm::vec3 pos, int vao, bool reload)
{
	s_chunk		res;

	for (float x = 0; x < CHUNK_X; x++)
		for (float y = 0; y < CHUNK_Y; y++)
			for (float z = 0; z < CHUNK_Z; z++)
			{
				if (this->isSolid(glm::vec3(x, y, z), pos) == true)
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
	this->perlinWorm(res.chunkInfos, pos);
	this->disableHiddenCubes(res.chunkInfos, pos);
	this->setPile(res.chunkInfos, pos);
	this->mixUpDirt(res.chunkInfos, pos);

	res.Chunk = _cubeFactory->getChunk(glm::vec3(pos.x * CHUNK_X, pos.y * CHUNK_Y, pos.z * CHUNK_Z), std::ref(res.chunkInfos), vao, reload);

	return res;
}