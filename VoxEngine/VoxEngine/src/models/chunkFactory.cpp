
#include "chunkFactory.h"

chunkFactory::chunkFactory(cubeFactory& cubeFactory)
{
	_cubeFactory = cubeFactory;
}

chunkFactory::~chunkFactory()
{

}
/*
bool	chunkFactory::test_hidden(int x, int y, int z)
{
	NYCube * cubeXPrev = NULL;
	NYCube * cubeXNext = NULL;
	NYCube * cubeYPrev = NULL;
	NYCube * cubeYNext = NULL;
	NYCube * cubeZPrev = NULL;
	NYCube * cubeZNext = NULL;

	if (x == 0 && Voisins[0] != NULL)
		cubeXPrev = &(Voisins[0]->_Cubes[CHUNK_SIZE - 1][y][z]);
	else if (x > 0)
		cubeXPrev = &(_Cubes[x - 1][y][z]);

	if (x == CHUNK_SIZE - 1 && Voisins[1] != NULL)
		cubeXNext = &(Voisins[1]->_Cubes[0][y][z]);
	else if (x < CHUNK_SIZE - 1)
		cubeXNext = &(_Cubes[x + 1][y][z]);

	if (y == 0 && Voisins[2] != NULL)
		cubeYPrev = &(Voisins[2]->_Cubes[x][CHUNK_SIZE - 1][z]);
	else if (y > 0)
		cubeYPrev = &(_Cubes[x][y - 1][z]);

	if (y == CHUNK_SIZE - 1 && Voisins[3] != NULL)
		cubeYNext = &(Voisins[3]->_Cubes[x][0][z]);
	else if (y < CHUNK_SIZE - 1)
		cubeYNext = &(_Cubes[x][y + 1][z]);

	if (z == 0 && Voisins[4] != NULL)
		cubeZPrev = &(Voisins[4]->_Cubes[x][y][CHUNK_SIZE - 1]);
	else if (z > 0)
		cubeZPrev = &(_Cubes[x][y][z - 1]);

	if (z == CHUNK_SIZE - 1 && Voisins[5] != NULL)
		cubeZNext = &(Voisins[5]->_Cubes[x][y][0]);
	else if (z < CHUNK_SIZE - 1)
		cubeZNext = &(_Cubes[x][y][z + 1]);

	if (cubeXPrev == NULL || cubeXNext == NULL ||
		cubeYPrev == NULL || cubeYNext == NULL ||
		cubeZPrev == NULL || cubeZNext == NULL)
		return false;

	if (cubeXPrev->isSolid() == true && //droite
		cubeXNext->isSolid() == true && //gauche
		cubeYPrev->isSolid() == true && //haut
		cubeYNext->isSolid() == true && //bas
		cubeZPrev->isSolid() == true && //devant
		cubeZNext->isSolid() == true)  //derriere
		return true;
	return false;
}

void	chunkFactory::disableHiddenCubes(std::vector<Entity*> &list)
{
	for (int x = 0; x < _size.y; y++)
		for (int y = 0; y < _size.z; z++)
			for (int z = 0; z < _size.x; x++)
			{
				if (this->test_hidden(list))

			}
}
*/

s_chunk chunkFactory::getChunk(glm::vec3 pos)
{
	s_chunk					res;
	std::vector<Entity*>	cubes;
	std::vector<GLuint>		vaos;

	bool	isVAO[4] = { false };
	
	Entity *cube;

	//CONSTRUCT CHUNK
	for (float y = 0; y < CHUNK_Y; y++)
		for (float z = 0; z < CHUNK_Z; z++)
			for (float x = 0; x < CHUNK_X; x++)
			{
				if (_perlin.noise(x / CHUNK_X * 2, y / CHUNK_Y * 2, z / CHUNK_Z * 2) < 0)
				{
					cube = _cubeFactory.getCube(cubeFactory::GRASS, glm::vec3(x, y, z));
					cube->_draw = true;
					res.chunkInfos[(int)x][(int)y][(int)z] = GRASS;

					if (isVAO[0] == false)
					{
						vaos.push_back(cube->_model->_rawModel->_vao_id);
						isVAO[0] = true;
					}
				}
				else
				{
					cube = _cubeFactory.getCube(cubeFactory::VOID, glm::vec3(x, y, z));
					cube->_draw = false;
					res.chunkInfos[(int)x][(int)y][(int)z] = VOID;

					if (isVAO[1] == false)
					{
						vaos.push_back(cube->_model->_rawModel->_vao_id);
						isVAO[1] = true;
					}
				}
				cubes.push_back(cube);
			}

	// DIVIDE CHUNK BY VAO
	std::vector<Entity*> ChunkVAO;
	std::vector<Entity*>::iterator	cube_it;
	std::vector<GLuint>::iterator	vaoID_it;

	for (vaoID_it = vaos.begin(); vaoID_it < vaos.end(); ++vaoID_it)
	{
		ChunkVAO.clear();
		for (cube_it = cubes.begin(); cube_it < cubes.end(); ++cube_it)
			if ((*cube_it)->_model->_rawModel->_vao_id == *vaoID_it)
				ChunkVAO.push_back(*cube_it);
		res.VAOChunk.push_back(ChunkVAO);
	}
	return res;
}