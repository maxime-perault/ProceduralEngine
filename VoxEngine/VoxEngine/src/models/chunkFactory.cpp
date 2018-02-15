
#include "chunkFactory.h"

chunkFactory::chunkFactory() {}

chunkFactory::~chunkFactory() {}

void		chunkFactory::setCubeFactory(cubeFactory& cubeFactory)
{
	_cubeFactory = cubeFactory;
}

bool	chunkFactory::test_hidden(glm::vec3 pos, int chunk[CHUNK_X][CHUNK_Y][CHUNK_Z])
{
	if (pos.x == 0 || pos.x == (CHUNK_X - 1) ||
		pos.y == 0 || pos.y == (CHUNK_Y - 1) ||
		pos.z == 0 || pos.z == (CHUNK_Z - 1))
		return false;

	int top = chunk[(int)pos.x][(int)pos.y + 1][(int)pos.z];
	int bot = chunk[(int)pos.x][(int)pos.y - 1][(int)pos.z];
	int right = chunk[(int)pos.x + 1][(int)pos.y][(int)pos.z];
	int left = chunk[(int)pos.x - 1][(int)pos.y][(int)pos.z];
	int front = chunk[(int)pos.x][(int)pos.y][(int)pos.z - 1];
	int back = chunk[(int)pos.x][(int)pos.y][(int)pos.z + 1];

	if (top != WATER && top != VOID &&
		bot != WATER && bot != VOID &&
		right != WATER && right != VOID &&
		left != WATER && left != VOID &&
		front != WATER && front != VOID &&
		back != WATER && back != VOID)
		return true;

	return false;
}

void	chunkFactory::disableHiddenCubes(std::vector<Entity>& cubes, int chunk[CHUNK_X][CHUNK_Y][CHUNK_Z])
{
	glm::vec3	pos;

	for (int x = 0; x < CHUNK_X; x++)
		for (int y = 0; y < CHUNK_Y; y++)
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (this->test_hidden(glm::vec3(x, y, z), chunk) == true)
				{
					cubes[z + y * CHUNK_Y + x * CHUNK_Y * CHUNK_X]._draw = false;
				}
			}
}


s_chunk chunkFactory::getChunk(glm::vec3 pos)
{
	s_chunk					res;
	std::vector<Entity>		cubes;
	std::vector<GLuint>		vaos;

	bool	isVAO[4] = { false };
	
	Entity	cube;

	//CONSTRUCT CHUNK
	for (float x = 0; x < CHUNK_X; x++)
		for (float y = 0; y < CHUNK_Y; y++)
			for (float z = 0; z < CHUNK_Z; z++)
			{
				if (_perlin.noise(	(x + (pos.x * CHUNK_X)) / (CHUNK_X * 4) * 2,
									(y + (pos.y * CHUNK_Y)) / (CHUNK_Y * 4) * 2,
									(z + (pos.z * CHUNK_Z)) / (CHUNK_Z * 4) * 2) < 0)
				{
					cube = _cubeFactory.getCube(cubeFactory::GRASS,
						glm::vec3(x + (pos.x * CHUNK_X), y + (pos.y * CHUNK_Y), z + (pos.z * CHUNK_Z)));
					cube._draw = true;
					res.chunkInfos[(int)x][(int)y][(int)z] = GRASS;

					if (isVAO[0] == false)
					{
						vaos.push_back(cube._model._rawModel._vao_id);
						isVAO[0] = true;
					}
				}
				else
				{
					cube = _cubeFactory.getCube(cubeFactory::VOID,
						glm::vec3(x + (pos.x * CHUNK_X), y + (pos.y * CHUNK_Y), z + (pos.z * CHUNK_Z)));
					cube._draw = false;
					res.chunkInfos[(int)x][(int)y][(int)z] = VOID;

					if (isVAO[1] == false)
					{
						vaos.push_back(cube._model._rawModel._vao_id);
						isVAO[1] = true;
					}
				}
				cubes.push_back(cube);
			}
	this->disableHiddenCubes(cubes, res.chunkInfos);

	// DIVIDE CHUNK BY VAO
	std::vector<Entity> ChunkVAO;
	std::size_t	cube_it;
	std::size_t	vaoID_it;

	for (vaoID_it = 0; vaoID_it < vaos.size(); ++vaoID_it)
	{
		ChunkVAO.clear();
		for (cube_it = 0; cube_it < cubes.size(); ++cube_it)
			if (cubes[cube_it]._model._rawModel._vao_id == vaos[vaoID_it])
				ChunkVAO.push_back(cubes[cube_it]);
		res.VAOChunk.push_back(ChunkVAO);
	}
	return res;
}