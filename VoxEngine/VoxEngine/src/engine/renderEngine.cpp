
#include "renderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <limits>

renderEngine::renderEngine(std::size_t win_x, std::size_t win_y)
{
	_projMat = glm::mat4(1.0);

	_staticShader = new staticShader();

	_fov = 70;
	_near = 0.1;
	_far = 1000;
	_win_x = win_x;
	_win_y = win_y;

	_camera = new Camera(glm::vec3(0, 0, -5));
	
	createProjectionMatrix(_staticShader);
	createViewMatrix(_camera, _staticShader);

	_entities.push_back(_cubeFactory.getCube(cubeFactory::GRASS, glm::vec3(0, 0, 0)));
}

renderEngine::~renderEngine()
{
	_staticShader->cleanUp();
	delete(_staticShader);
	delete(_camera);
}

void	renderEngine::updateWindow(std::size_t win_x, std::size_t win_y)
{
	_win_x = win_x;
	_win_y = win_y;
	this->createProjectionMatrix(_staticShader);
}

void	renderEngine::createProjectionMatrix(staticShader *shader)
{
	float	n(_near);
	float	f(_far);
	float	scale = tan(_fov * 0.5 * M_PI / 180) * n;
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

	shader->start();
	shader->loadProjectionMatrix(_projMat);
	shader->stop();
}

void	renderEngine::createOrthographicProjectionMatrix(staticShader *shader)
{
	float	n(_near);
	float	f(_far);
	float	max = _camera->_pos.z * -1;
	float	r = ((float)_win_x / (float)_win_y) * max;
	float	l = -r;
	float	t = max;
	float	b = -t;

	_projMat[0][0] = 1 / r;
	_projMat[1][1] = 1 / t;
	_projMat[2][2] = -2 / (f - n);
	_projMat[3][2] = -((f + n) / (f - n));
	_projMat[3][3] = 1;

	shader->start();
	shader->loadProjectionMatrix(_projMat);
	shader->stop();
}

void	renderEngine::createModelMatrix(Entity *entity, staticShader *shader)
{
	shader->loadTransformationMatrix(entity->_modelMatrix);
}

void	renderEngine::createViewMatrix(Camera *cam, staticShader *shader)
{
	shader->loadViewMatrix(cam->_viewMatrix);
}

void	renderEngine::renderEntities(void/*const std::vector<Entity*> entities*/)
{
	std::vector<Entity*> entities = _entities;

	_staticShader->start();
	this->createViewMatrix(_camera, _staticShader);

	for (std::size_t i(0); i < entities.size(); ++i)
	{
		_entities[i]->rotation(glm::vec3(0, 1, 0), 0.01);

		this->createModelMatrix(entities[i], _staticShader);

		glBindVertexArray(entities[i]->_model->_rawModel->_vao_id);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, entities[i]->_model->_texture->_id);

		glDrawElements(GL_TRIANGLES, entities[i]->_model->_rawModel->_vertex_count, GL_UNSIGNED_INT, 0);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	_staticShader->stop();
}