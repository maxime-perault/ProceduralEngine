
#include "cubeFactory.h"

cubeFactory::cubeFactory() {}

cubeFactory::~cubeFactory()
{
	_loader.cleanUp();
}

void	cubeFactory::setTerrain(void)
{
	_terrain = Texture(_loader.loadTexture("assets/minecraft/terrain2", true));
}

void	cubeFactory::setFace(std::vector<int>& ind, std::vector<float>& uv, std::vector<float>& vtx,
		std::vector<float>& normals, glm::vec3 pos, e_face face, e_Type type, bool light, int nbFace)
{
	//Determine offsets for uv offsets in Terrain.png
	static glm::vec2 pos_side;
	static glm::vec2 pos_top;
	static glm::vec2 pos_bot;

	switch (type)
	{
		case DIRT:
		{
			pos_side.x = 5;
			pos_side.y = 13;
			pos_top = pos_side;
			pos_bot = pos_top;
			break;
		}
		case STONE:
		{
			pos_side.x = 9;
			pos_side.y = 5;
			pos_top = pos_side;
			pos_bot = pos_top;
			break;
		}
		case COAL:
		{
			pos_side.x = 6;
			pos_side.y = 7;
			pos_top = pos_side;
			pos_bot = pos_top;
			break;
		}
		case GRASS:
		{
			
			pos_side.x = 6;
			pos_side.y = 13;
			pos_top.x = 5;
			pos_top.y = 11;
			pos_bot.x = 5;
			pos_bot.y = 13;
			/*
			pos_side.x = 7;
			pos_side.y = 13;
			pos_top.x = 7;
			pos_top.y = 12;
			pos_bot.x = 5;
			pos_bot.y = 13;
			*/
			break;
		}
		case FOLIAGE_R:
		{
			pos_side.x = 0;
			pos_side.y = 9;
			pos_top = pos_side;
			pos_bot = pos_top;
			break;
		}
		case FOLIAGE_G:
		{
			pos_side.x = 1;
			pos_side.y = 9;
			pos_top = pos_side;
			pos_bot = pos_top;
			break;
		}
		case FOLIAGE_Y:
		{
			pos_side.x = 2;
			pos_side.y = 9;
			pos_top = pos_side;
			pos_bot = pos_top;
			break;
		}
		case WOOD:
		{
			pos_side.x = 0;
			pos_side.y = 13;
			pos_top.x = 0;
			pos_top.y = 14;
			pos_bot = pos_top;
			break;
		}
		case SUN:
		{
			pos_side.x = 13;
			pos_side.y = 16;
			pos_top = pos_side;
			pos_bot = pos_top;
			break;
		}
		case PLAYER:
		{
			pos_side.x = 14;
			pos_side.y = 16;
			pos_top = pos_side;
			pos_bot = pos_top;
			break;
		}
		default: break;
	}

	switch (face)
	{
		case FRONT:
		{
			vtx.insert(vtx.end(), {
				-0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z,
				-0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,
				0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,
				0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z });
			if (!light)
				normals.insert(normals.end(), {
					0, 0, -1,
					0, 0, -1,
					0, 0, -1,
					0, 0, -1 });
			else
				normals.insert(normals.end(), {
					0, 0, 1,
					0, 0, 1,
					0, 0, 1,
					0, 0, 1 });
			uv.insert(uv.end(), {
				pos_side.x * (1.f / 16.f), pos_side.y * (1.f / 17.f),
				pos_side.x * (1.f / 16.f), (pos_side.y + 1.f) * (1.f / 17.f),
				(pos_side.x + 1.f) * (1.f / 16.f), (pos_side.y + 1.f) * (1.f / 17.f),
				(pos_side.x + 1.f) * (1.f / 16.f), pos_side.y * (1.f / 17.f) });
			ind.insert(ind.end(), {
				0 + nbFace * 4, 1 + nbFace * 4, 3 + nbFace * 4,
				3 + nbFace * 4, 1 + nbFace * 4, 2 + nbFace * 4, });
			break;
		}
		case BACK:
		{
			vtx.insert(vtx.end(), {
				-0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z,
				-0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z,
				0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z,
				0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z });
			if (!light)
				normals.insert(normals.end(), {
					0, 0, 1,
					0, 0, 1,
					0, 0, 1,
					0, 0, 1 });
			else
				normals.insert(normals.end(), {
					0, 0, -1,
					0, 0, -1,
					0, 0, -1,
					0, 0, -1 });
			uv.insert(uv.end(), {
				pos_side.x * (1.f / 16.f), pos_side.y * (1.f / 17.f),
				pos_side.x * (1.f / 16.f), (pos_side.y + 1.f) * (1.f / 17.f),
				(pos_side.x + 1.f) * (1.f / 16.f), (pos_side.y + 1.f) * (1.f / 17.f),
				(pos_side.x + 1.f) * (1.f / 16.f), pos_side.y * (1.f / 17.f) });
			ind.insert(ind.end(), {
				3 + nbFace * 4, 1 + nbFace * 4, 0 + nbFace * 4,
				2 + nbFace * 4, 1 + nbFace * 4, 3 + nbFace * 4 });
			break;
		}
		case RIGHT:
		{
			vtx.insert(vtx.end(), {
				0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z,
				0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,
				0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z,
				0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z });
			if (!light)
				normals.insert(normals.end(), {
					1, 0, 0,
					1, 0, 0,
					1, 0, 0,
					1, 0, 0 });
			else
				normals.insert(normals.end(), {
					-1, 0, 0,
					-1, 0, 0,
					-1, 0, 0,
					-1, 0, 0 });
			uv.insert(uv.end(), {
				pos_side.x * (1.f / 16.f), pos_side.y * (1.f / 17.f),
				pos_side.x * (1.f / 16.f), (pos_side.y + 1.f) * (1.f / 17.f),
				(pos_side.x + 1.f) * (1.f / 16.f), (pos_side.y + 1.f) * (1.f / 17.f),
				(pos_side.x + 1.f) * (1.f / 16.f), pos_side.y * (1.f / 17.f) });
			ind.insert(ind.end(), {
				0 + nbFace * 4, 1 + nbFace * 4, 3 + nbFace * 4,
				3 + nbFace * 4, 1 + nbFace * 4, 2 + nbFace * 4 });
			break;
		}
		case LEFT:
		{
			vtx.insert(vtx.end(), {
				-0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z,
				-0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,
				-0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z,
				-0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z });
			if (!light)
				normals.insert(normals.end(), {
					-1, 0, 0,
					-1, 0, 0,
					-1, 0, 0,
					-1, 0, 0 });
			else
				normals.insert(normals.end(), {
					1, 0, 0,
					1, 0, 0,
					1, 0, 0,
					1, 0, 0 });
			uv.insert(uv.end(), {
				pos_side.x * (1.f / 16.f), pos_side.y * (1.f / 17.f),
				pos_side.x * (1.f / 16.f), (pos_side.y + 1.f) * (1.f / 17.f),
				(pos_side.x + 1.f) * (1.f / 16.f), (pos_side.y + 1.f) * (1.f / 17.f),
				(pos_side.x + 1.f) * (1.f / 16.f), pos_side.y * (1.f / 17.f) });
			ind.insert(ind.end(), {
				3 + nbFace * 4, 1 + nbFace * 4, 0 + nbFace * 4,
				2 + nbFace * 4, 1 + nbFace * 4, 3 + nbFace * 4 });
			break;
		}
		case TOP:
		{
			vtx.insert(vtx.end(), {
				-0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z,
				-0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z,
				0.5f + pos.x, 0.5f + pos.y, -0.5f + pos.z,
				0.5f + pos.x, 0.5f + pos.y, 0.5f + pos.z });
			if (!light)
				normals.insert(normals.end(), {
					0, 1, 0,
					0, 1, 0,
					0, 1, 0,
					0, 1, 0 });
			else
				normals.insert(normals.end(), {
					0, -1, 0,
					0, -1, 0,
					0, -1, 0,
					0, -1, 0 });
			uv.insert(uv.end(), {
				pos_top.x * (1.f / 16.f), pos_top.y * (1.f / 17.f),
				pos_top.x * (1.f / 16.f), (pos_top.y + 1.f) * (1.f / 17.f),
				(pos_top.x + 1.f) * (1.f / 16.f), (pos_top.y + 1.f) * (1.f / 17.f),
				(pos_top.x + 1.f) * (1.f / 16.f), pos_top.y * (1.f / 17.f) });
			ind.insert(ind.end(), {
				0 + nbFace * 4, 1 + nbFace * 4, 3 + nbFace * 4,
				3 + nbFace * 4, 1 + nbFace * 4, 2 + nbFace * 4 });
			break;
		}
		case BOT:
		{
			vtx.insert(vtx.end(), {
				-0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z,
				-0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,
				0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z,
				0.5f + pos.x, -0.5f + pos.y, 0.5f + pos.z });
			if (!light)
				normals.insert(normals.end(), {
					0, -1, 0,
					0, -1, 0,
					0, -1, 0,
					0, -1, 0 });
			else
				normals.insert(normals.end(), {
					0, 1, 0,
					0, 1, 0,
					0, 1, 0,
					0, 1, 0 });
			uv.insert(uv.end(), {
				pos_bot.x * (1.f / 16.f), pos_bot.y * (1.f / 17.f),
				pos_bot.x * (1.f / 16.f), (pos_bot.y + 1.f) * (1.f / 17.f),
				(pos_bot.x + 1.f) * (1.f / 16.f), (pos_bot.y + 1.f) * (1.f / 17.f),
				(pos_bot.x + 1.f) * (1.f / 16.f), pos_bot.y * (1.f / 17.f) });
			ind.insert(ind.end(), {
				3 + nbFace * 4, 1 + nbFace * 4, 0 + nbFace * 4,
				2 + nbFace * 4, 1 + nbFace * 4, 3 + nbFace * 4 });
			break;
		}
		default: break;
	}
}

Entity	cubeFactory::getChunk(glm::vec3 pos, std::pair<int, bool> (&chunkInfos)[16][16][16], GLuint vao, bool reload)
{
	std::vector<int>	ind;
	std::vector<float>	uv;
	std::vector<float>	vtx;
	std::vector<float>	normals;

	std::size_t i(0);
	for (std::size_t x = 0; x < 16; ++x)
		for (std::size_t y = 0; y < 16; ++y)
			for (std::size_t z = 0; z < 16; ++z)
			{
				if (chunkInfos[x][y][z].first != VOID && chunkInfos[x][y][z].first != WATER && chunkInfos[x][y][z].second == true)
				{
					if (chunkInfos[x][y][z].first == WOOD)
					{
						this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), RIGHT, (e_Type)chunkInfos[x][y][z].first, false, i++);
						this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), LEFT, (e_Type)chunkInfos[x][y][z].first, false, i++);
						this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), TOP, (e_Type)chunkInfos[x][y][z].first, false, i++);
						this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), BOT, (e_Type)chunkInfos[x][y][z].first, false, i++);
						this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), FRONT, (e_Type)chunkInfos[x][y][z].first, false, i++);
						this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), BACK, (e_Type)chunkInfos[x][y][z].first, false, i++);
					}
					else
					{
						if (x == 15 || (chunkInfos[x + 1][y][z].first == VOID || chunkInfos[x + 1][y][z].first == WATER)
							|| chunkInfos[x + 1][y][z].first == FOLIAGE_R || chunkInfos[x + 1][y][z].first == FOLIAGE_G || chunkInfos[x + 1][y][z].first == FOLIAGE_Y)
							this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), RIGHT, (e_Type)chunkInfos[x][y][z].first, false, i++);
						if (x == 0 || (chunkInfos[x - 1][y][z].first == VOID || chunkInfos[x - 1][y][z].first == WATER)
							|| chunkInfos[x - 1][y][z].first == FOLIAGE_R || chunkInfos[x - 1][y][z].first == FOLIAGE_G || chunkInfos[x - 1][y][z].first == FOLIAGE_Y)
							this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), LEFT, (e_Type)chunkInfos[x][y][z].first, false, i++);

						if (y == 15 || (chunkInfos[x][y + 1][z].first == VOID || chunkInfos[x][y + 1][z].first == WATER)
							|| chunkInfos[x][y + 1][z].first == FOLIAGE_R || chunkInfos[x][y + 1][z].first == FOLIAGE_G || chunkInfos[x][y + 1][z].first == FOLIAGE_Y)
							this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), TOP, (e_Type)chunkInfos[x][y][z].first, false, i++);
						if (y == 0 || (chunkInfos[x][y - 1][z].first == VOID || chunkInfos[x][y - 1][z].first == WATER)
							|| chunkInfos[x][y - 1][z].first == FOLIAGE_R || chunkInfos[x][y - 1][z].first == FOLIAGE_G || chunkInfos[x][y - 1][z].first == FOLIAGE_Y)
							this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), BOT, (e_Type)chunkInfos[x][y][z].first, false, i++);

						if (z == 0 || (chunkInfos[x][y][z - 1].first == VOID || chunkInfos[x][y][z - 1].first == WATER)
							|| chunkInfos[x][y][z - 1].first == FOLIAGE_R || chunkInfos[x][y][z - 1].first == FOLIAGE_G || chunkInfos[x][y][z - 1].first == FOLIAGE_Y)
							this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), FRONT, (e_Type)chunkInfos[x][y][z].first, false, i++);
						if (z == 15 || (chunkInfos[x][y][z + 1].first == VOID || chunkInfos[x][y][z + 1].first == WATER)
							|| chunkInfos[x][y][z + 1].first == FOLIAGE_R || chunkInfos[x][y][z + 1].first == FOLIAGE_G || chunkInfos[x][y][z + 1].first == FOLIAGE_Y)
							this->setFace(std::ref(ind), std::ref(uv), std::ref(vtx), std::ref(normals), glm::vec3(x, y, z), BACK, (e_Type)chunkInfos[x][y][z].first, false, i++);
					}
				}
			}
	static Entity res;

	if (reload == true)
	{
		res = Entity(texturedModel(rawModel(vao, ind.size()), _terrain));
		_loader.stockFrags(vtx, normals, uv, ind, vao);
	}
	else
	{
		res = Entity(texturedModel(_loader.loadtoVAO(vtx, normals, uv, ind), _terrain));
	}

	res._pos = pos;
	res.setModelMatrix();
	return (res);
}

Entity	cubeFactory::getCube(const e_Type type, glm::vec3 pos, GLuint vao)
{
	std::vector<int>	ind;
	std::vector<float>	uv;
	std::vector<float>	vtx;
	std::vector<float>	normals;
	bool light = false;

	if (type == SUN)
		light = true;

	this->setFace(ind, uv, vtx, normals, glm::vec3(0, 0, 0), FRONT, type, light, 0);
	this->setFace(ind, uv, vtx, normals, glm::vec3(0, 0, 0), BACK, type, light, 1);
	this->setFace(ind, uv, vtx, normals, glm::vec3(0, 0, 0), RIGHT, type, light, 2);
	this->setFace(ind, uv, vtx, normals, glm::vec3(0, 0, 0), LEFT, type, light, 3);
	this->setFace(ind, uv, vtx, normals, glm::vec3(0, 0, 0), TOP, type, light, 4);
	this->setFace(ind, uv, vtx, normals, glm::vec3(0, 0, 0), BOT, type, light, 5);

	if (vao != -1)
		_loader.deleteVAO(vao);
	Entity res = Entity(texturedModel(_loader.loadtoVAO(vtx, normals, uv, ind), _terrain));
	
	res._pos = pos;
	res.setModelMatrix();
	
	return (res);
}

Entity	cubeFactory::getWirelessCube(glm::vec3 colour, glm::vec3 pos, GLuint vao)
{
	std::vector<int>	ind;
	std::vector<float>	vtx;

	vtx.insert(vtx.end(), {
		-0.501f, -0.501f, -0.501f, -0.501f, 0.501f, -0.501f, //FRONT
		-0.501f, 0.501f, -0.501f, 0.501f, 0.501f, -0.501f,
		0.501f, 0.501f, -0.501f, 0.501f, -0.501f, -0.501f,
		0.501f, -0.501f, -0.501f, -0.501f, -0.501f, -0.501f,
		
		-0.501f, -0.501f, 0.501f, -0.501f, 0.501f, 0.501f, //BACK
		-0.501f, 0.501f, 0.501f, 0.501f, 0.501f, 0.501f,
		0.501f, 0.501f, 0.501f, 0.501f, -0.501f, 0.501f,
		0.501f, -0.501f, 0.501f, -0.501f, -0.501f, 0.501f,
		
		-0.501f, -0.501f, -0.501f, -0.501f, -0.501f, 0.501f, //EDGES
		-0.501f, 0.501f, -0.501f, -0.501f, 0.501f, 0.501f,
		0.501f, 0.501f, -0.501f, 0.501f, 0.501f, 0.501f,
		0.501f, -0.501f, -0.501f, 0.501f, -0.501f, 0.501f,
	});
	ind.insert(ind.end(), {
		0, 1, 2, 3, 4, 5, 6, 7,
		8, 9, 10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
	});

	if (vao != -1)
		_loader.deleteVAO(vao);
	Entity res = Entity(texturedModel(_loader.loadtoVAO(vtx, ind), Texture(-1)));

	res._colour = colour;
	res._pos = pos;

	res.setModelMatrix();

	return (res);
}

Entity	cubeFactory::getLine(glm::vec3 colour, glm::vec3 pos, glm::vec3 rot, GLuint vao)
{
	std::vector<int>	ind;
	std::vector<float>	vtx;

	vtx.insert(vtx.end(), {
		0, 0, 0,
		0, 1, 0 });
	ind.insert(ind.end(), { 0, 1 });

	if (vao != -1)
		_loader.deleteVAO(vao);
	Entity res = Entity(texturedModel(_loader.loadtoVAO(vtx, ind), Texture(-1)));

	res._colour = colour;
	res._pos = pos;
	res._rot = rot;

	res.setModelMatrix();

	return (res);
}

