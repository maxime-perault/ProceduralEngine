
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

void	cubeFactory::initCube(void)
{
	/*
	** CUBE VERTEX
	*/
	static const float tmp_vrtx[] = {
		-0.5, 0.5, -0.5,
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, 0.5, -0.5,

		-0.5, 0.5, 0.5,
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, 0.5, 0.5,

		0.5, 0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, -0.5, 0.5,
		0.5, 0.5, 0.5,

		-0.5, 0.5, -0.5,
		-0.5, -0.5, -0.5,
		-0.5, -0.5, 0.5,
		-0.5, 0.5, 0.5,

		-0.5, 0.5, 0.5,
		-0.5, 0.5, -0.5,
		0.5, 0.5, -0.5,
		0.5, 0.5, 0.5,

		-0.5, -0.5, 0.5,
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, -0.5, 0.5 };
	static const std::vector<float>
		cube_vrtx(tmp_vrtx, tmp_vrtx + sizeof(tmp_vrtx) / sizeof(tmp_vrtx[0]));

	/*
	** CUBE INDICES
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

	//TEXTURE COORDS
	static const float tmp_text[] = { 0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0 };
	static const std::vector<float>
		cube_text_coord(tmp_text, tmp_text + sizeof(tmp_text) / sizeof(tmp_text[0]));


	/*
	** VAO (VBO1 = obj coord with indices; VBO2 = textures uv)
	*/
	_models.push_back(_loader.loadtoVAO(cube_vrtx, cube_ind, cube_text_coord));

	/*
	** TEXTURE + TEXTURED MODEL
	*/
	_textures.push_back(new Texture(_loader.loadTexture("assets/minecraft/textures/blocks/brick.png")));
	_tex_models.push_back(new texturedModel(_models[_models.size() - 1], _textures[_textures.size() - 1]));
}

void	cubeFactory::setupModels(void)
{
	this->initCube();
	this->initCube();
	this->initCube();
}

Entity	*cubeFactory::getCube(const e_Type type, glm::vec3 pos)
{
	Entity *res = new Entity(_tex_models[type]);
	
	res->translate(pos);
	
	return (res);
}
