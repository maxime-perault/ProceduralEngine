
#include "renderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

renderEngine::renderEngine()
{

}

renderEngine::renderEngine(std::size_t win_x, std::size_t win_y)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_projMat = glm::mat4(1.0);

	_fov = 70;
	_near = (float)0.1;
	_far = 1000;
	_win_x = win_x;
	_win_y = win_y;

	createProjectionMatrix(_staticShader);
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

	glBindVertexArray(entity._model._rawModel._vao_id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, entity._model._texture._id);

	glDrawElements(GL_TRIANGLES, entity._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);
}

void	renderEngine::renderVAO_multipleTime(std::vector<Entity>& entities)
{
	if (entities.size() == 0)
		return;

	glBindVertexArray(entities[0]._model._rawModel._vao_id);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, entities[0]._model._texture._id);

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

	glBindVertexArray(axis[0]._model._rawModel._vao_id);

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

		glBindVertexArray(text[i]._model._rawModel._vao_id);

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
}

void	renderEngine::staticRender(Camera& cam, World *world, const bool debug)
{
	std::vector<s_chunk>&	chunks = world->getChunks();
	Light&					sun = world->getSun();
	Entity&					player = world->getPlayer();
	std::vector<Entity>&	axis = world->getAxis();
	std::vector<Entity>&	text = world->getText();

	_staticShader.start();

	this->createViewMatrix(cam, _staticShader);
	//render sun
	_staticShader.loadLight(sun._entity._pos, sun._colour, sun._damper, sun._ambientLevel);
	this->renderVAO_oneTime(sun._entity);

	//render player
	this->renderVAO_oneTime(player);

	//render chunk
	std::vector<std::vector<Entity*>>::iterator chunkVAO_it;

	for (std::size_t i(0); i < chunks.size(); ++i)
	{
		for (std::size_t VAO_id = 0; VAO_id < chunks[i].VAOChunk.size(); ++VAO_id)
			this->renderVAO_multipleTime(chunks[i].VAOChunk[VAO_id]);
	}
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