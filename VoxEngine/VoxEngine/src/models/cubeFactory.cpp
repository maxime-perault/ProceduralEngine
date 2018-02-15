
#include "cubeFactory.h"

cubeFactory::cubeFactory()
{
	_loader = VAOLoader();
	this->setupModels();
}

cubeFactory::~cubeFactory()
{
	_loader.cleanUp();
}

void	cubeFactory::initCube(std::string texturePath, const bool light)
{
	/*
	** CUBE VERTEX
	*/
	static const float tmp_vrtx[] = {
		-0.5, 0.5, -0.5, //FRONT
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, 0.5, -0.5,

		-0.5, 0.5, 0.5, //BACK
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, 0.5, 0.5,

		0.5, 0.5, -0.5, //RIGHT
		0.5, -0.5, -0.5,
		0.5, -0.5, 0.5,
		0.5, 0.5, 0.5,

		-0.5, 0.5, -0.5, //LEFT
		-0.5, -0.5, -0.5,
		-0.5, -0.5, 0.5,
		-0.5, 0.5, 0.5,

		-0.5, 0.5, 0.5, //TOP
		-0.5, 0.5, -0.5,
		0.5, 0.5, -0.5,
		0.5, 0.5, 0.5,

		-0.5, -0.5, 0.5, //BOT
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, -0.5, 0.5 };
	static const std::vector<float>
		cube_vrtx(tmp_vrtx, tmp_vrtx + sizeof(tmp_vrtx) / sizeof(tmp_vrtx[0]));

	/*
	** CUBE NORMALS
	*/
	static const float tmp_normals[] = {
		0, 0, -1, //FRONT
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,

		0, 0, 1, //BACK
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,

		1, 0, 0, //RIGHT
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,

		-1, 0, 0, //LEFT
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,

		0, 1, 0, //TOP
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,

		0, -1, 0, //BOT
		0, -1, 0,
		0, -1, 0,
		0, -1, 0 };
		static const std::vector<float>
			cube_normals(tmp_normals, tmp_normals + sizeof(tmp_normals) / sizeof(tmp_normals[0]));

	static const float light_normals[] = {
		0, 0, 1, //FRONT
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,

		0, 0, -1, //BACK
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,

		-1, 0, 0, //RIGHT
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,

		1, 0, 0, //LEFT
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,

		0, -1, 0, //TOP
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,

		0, 1, 0, //BOT
		0, 1, 0,
		0, 1, 0,
		0, 1, 0 };
		static const std::vector<float>
			cube_lightNormals(light_normals, light_normals + sizeof(light_normals) / sizeof(light_normals[0]));

	//TEXTURE COORDS
	static const float tmp_text[] = { 0, 0,
		0, 1, //FRONT
		1, 1,
		1, 0,

		0, 0, //BACK
		0, 1,
		1, 1,
		1, 0,

		0, 0, //RIGHT
		0, 1,
		1, 1,
		1, 0,

		0, 0, //LEFT
		0, 1,
		1, 1,
		1, 0,

		0, 0, //TOP
		0, 1,
		1, 1,
		1, 0,

		0, 0, //BOT
		0, 1,
		1, 1,
		1, 0 };
	static const std::vector<float>
		cube_text_coord(tmp_text, tmp_text + sizeof(tmp_text) / sizeof(tmp_text[0]));

	/*
	** INDICES
	*/
	static const int tmp_ind[] = { 0, 1, 3,
		3, 1, 2,
		4, 5, 7,
		7, 5, 6,
		8, 9, 11,
		11, 9, 10,
		12, 13, 15,
		15, 13, 14,
		16, 17, 19,
		19, 17, 18,
		20, 21, 23,
		23, 21, 22 };
	static const std::vector<int>
		cube_ind(tmp_ind, tmp_ind + sizeof(tmp_ind) / sizeof(tmp_ind[0]));


	/*
	** VAO (VBO1 = obj coord with indices, VBO2 = normals, VBO3 = textures uv)
	*/
	if (light == true)
		_models.push_back(_loader.loadtoVAO(cube_vrtx, cube_lightNormals, cube_text_coord, cube_ind));
	else
		_models.push_back(_loader.loadtoVAO(cube_vrtx, cube_normals, cube_text_coord, cube_ind));
	/*
	** TEXTURE + TEXTURED MODEL
	*/
	_textures.push_back(Texture(_loader.loadTexture(texturePath)));
	_tex_models.push_back(texturedModel(_models[_models.size() - 1], _textures[_textures.size() - 1]));
}

void	cubeFactory::initLine(void)
{
	/*
	** LINE VERTEX
	*/
	static const float tmp_vrtx[] = {
		0, 0, 0,
		0, 1, 0};
	static const std::vector<float>
		line_vrtx(tmp_vrtx, tmp_vrtx + sizeof(tmp_vrtx) / sizeof(tmp_vrtx[0]));

	/*
	** INDICES
	*/
	static const int tmp_ind[] = {0, 1};
	static const std::vector<int>
		line_ind(tmp_ind, tmp_ind + sizeof(tmp_ind) / sizeof(tmp_ind[0]));


	/*
	** VAO (VBO1 = obj coord with indices, VBO2 = normals, VBO3 = textures uv)
	*/
		_models.push_back(_loader.loadtoVAO(line_vrtx, line_ind));
	/*
	** TEXTURE + TEXTURED MODEL
	*/
	_textures.push_back(Texture(-1));
	_tex_models.push_back(texturedModel(_models[_models.size() - 1], _textures[_textures.size() - 1]));
}

void	cubeFactory::setupModels(void)
{
	//GRASS
	this->initCube("assets/minecraft/textures/blocks/hardened_clay_stained_lime.png", false);
	//DIRT
	this->initCube("assets/minecraft/textures/blocks/hardened_clay_stained_brown.png", false);
	//VOID
	this->initCube("assets/minecraft/textures/blocks/stone_diorite_smooth.png", false);
	//WATER
	this->initCube("assets/minecraft/textures/blocks/wool_colored_cyan.png", false);
	//SUN
	this->initCube("assets/minecraft/textures/blocks/hardened_clay_stained_yellow.png", true);
	//PLAYER
	this->initCube("assets/minecraft/textures/blocks/hardened_clay_stained_red.png", false);
	//LINE
	this->initLine();
}

Entity	cubeFactory::getCube(const e_Type type, glm::vec3 pos)
{
	Entity res = Entity(_tex_models[type]);
	
	res._pos = pos;
	
	return (res);
}

Entity	cubeFactory::getLine(glm::vec3 colour, glm::vec3 pos, glm::vec3 rot)
{
	Entity res = Entity(_tex_models[LINE]);

	res._colour = colour;
	res._pos = pos;
	res._rot = rot;

	return (res);
}

