
#include "renderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

renderEngine::renderEngine() {}

renderEngine::renderEngine(std::size_t win_x, std::size_t win_y)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	*/

	_projMat = glm::mat4(1.0);

	_fov = 70;
	_near = (float)0.1;
	_far = 1000;
	_win_x = win_x;
	_win_y = win_y;

	createProjectionMatrix(_staticShader);

	_vaosRendering = false;
}

renderEngine::~renderEngine()
{
	_staticShader.cleanUp();
	_fontShader.cleanUp();
}

void	renderEngine::updateWindow(std::size_t win_x, std::size_t win_y)
{
	_win_x = win_x;
	_win_y = win_y;
	this->createProjectionMatrix(_staticShader);
}

void	renderEngine::createProjectionMatrix(staticShader& shader)
{
	float	n(_near);
	float	f(_far);
	float	scale = (float)tan(_fov * 0.5 * M_PI / 180) * n;
	float	r = ((float)_win_x / (float)_win_y) * scale;
	float	l = -r;
	float	t = scale;
	float	b = -t;

	_projMat[0][0] = n / r;
	_projMat[1][1] = n / t;
	_projMat[2][2] = -(f + n) / (f - n);
	_projMat[2][3] = -1;
	_projMat[3][2] = (-2 * f * n) / (f - n);
	_projMat[3][3] = 0;

	shader.start();
	shader.loadProjectionMatrix(_projMat);
	shader.stop();
}

void	renderEngine::createModelMatrix(Entity entity, staticShader& shader)
{
	shader.loadTransformationMatrix(entity.getModelMatrix());
}

void	renderEngine::createViewMatrix(Camera& cam, staticShader& shader)
{
	shader.loadViewMatrix(cam.getViewMatrix());
}

void	renderEngine::renderVAO_oneTime(Entity& entity)
{
	if (entity._draw == false)
		return;
	this->createModelMatrix(entity, _staticShader);

	this->bindVAO(entity._model._rawModel._vao_id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, entity._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);
}

void	renderEngine::renderVAO_LINE_oneTime(Entity& entity)
{
	this->createModelMatrix(entity, _staticShader);
	_staticShader.loadRawColour(entity._colour);

	this->bindVAO(entity._model._rawModel._vao_id);

	glEnableVertexAttribArray(0);

	glLineWidth(2);
	glDrawElements(GL_LINES, entity._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);

	_staticShader.unloadRawColour();
}

void	renderEngine::renderVAO_multipleTime(std::vector<Entity>& entities)
{
	if (entities.size() == 0)
		return;

	this->bindVAO(entities[0]._model._rawModel._vao_id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (std::size_t i(0); i < entities.size(); ++i)
	{
		if (entities[i]._draw == true)
		{
			this->createModelMatrix(entities[i], _staticShader);
			glDrawElements(GL_TRIANGLES, entities[i]._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);
		}
	}
}

void	renderEngine::renderAxis(std::vector<Entity> axis)
{
	if (axis.size() == 0)
		return;

	this->bindVAO(axis[0]._model._rawModel._vao_id);
	glEnableVertexAttribArray(0);

	for (std::size_t i(0); i < axis.size(); ++i)
	{
		this->createModelMatrix(axis[i], _staticShader);
		_staticShader.loadRawColour(axis[i]._colour);
		glDrawElements(GL_LINES, axis[i]._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);
	}
	_staticShader.unloadRawColour();
}

void	renderEngine::renderText(std::vector<Entity> text)
{
	
	for (std::size_t i(0); i < text.size(); ++i)
	{
		_fontShader.loadColour(text[i]._colour);
		_fontShader.loadPos(glm::vec2(text[i]._pos.x, text[i]._pos.y));

		this->bindVAO(text[i]._model._rawModel._vao_id);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, text[i]._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);
	}
}

void	renderEngine::stop(void)
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
	_vaosRendering = false;
}

void	renderEngine::bindVAO(GLuint vao)
{
	static GLint current_vao;

	if (_vaosRendering == false)
	{
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
		while (current_vao != 0)
		{
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
		}
		_vaosRendering = true;
	}
	glBindVertexArray(vao);
}

void	renderEngine::renderChunks(s_chunk(&chunks)[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z])
{
	for (std::size_t x(0); x < CHUNK_SIZE_X; ++x)
		for (std::size_t y(0); y < CHUNK_SIZE_Y; ++y)
			for (std::size_t z(0); z < CHUNK_SIZE_Z; ++z)
			{
				this->bindVAO(chunks[x][y][z].Chunk._model._rawModel._vao_id);

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);

				this->createModelMatrix(chunks[x][y][z].Chunk, _staticShader);
				glDrawElements(GL_TRIANGLES, chunks[x][y][z].Chunk._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);
			}
}

void	renderEngine::staticRender(Camera& cam, World *world, const bool debug)
{
	Light&					sun = world->getSun();
	Entity&					player = world->getPlayer();
	std::vector<Entity>&	axis = world->getAxis();
	std::vector<Entity>&	text = world->getText();
	Entity&					wirelessCube = world->getWirelessCube();

	_staticShader.start();

	this->createViewMatrix(cam, _staticShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, player._model._texture._id);

	//render sun
	_staticShader.loadLight(sun._entity._pos, sun._colour, sun._damper, sun._ambientLevel);
	this->renderVAO_oneTime(sun._entity);

	//render player
	if (cam._tps == true)
	this->renderVAO_oneTime(player);

	//render chunk
	this->renderChunks(world->_chunks);

	//render wirelessCube
	this->renderVAO_LINE_oneTime(wirelessCube);

	//render Axis
	if (debug == true)
		this->renderAxis(axis);

	_staticShader.stop();
}

void	renderEngine::fontRender(World *world, const bool debug)
{
	std::vector<Entity>&	text = world->getText();


	_fontShader.start();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text[0]._model._texture._id);

	this->renderText(text);

	_fontShader.stop();
}

void	renderEngine::renderWorld(Camera& cam, World *world, const bool debug)
{

	this->staticRender(cam, world, debug);

	if (debug == true)
		this->fontRender(world, debug);

	this->stop();
}