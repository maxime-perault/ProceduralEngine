
#include "renderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

renderEngine::renderEngine(std::size_t win_x, std::size_t win_y)
{
	_projMat = glm::mat4(1.0);
	_staticShader = new staticShader();

	_fov = 70;
	_near = 0.1;
	_far = 1000;
	_win_x = win_x;
	_win_y = win_y;

	_camera = new Camera(glm::vec3(0, 0, 0));

	createProjectionMatrix(_staticShader);
	createViewMatrix(_camera, _staticShader);
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

void	renderEngine::createProjectionMatrix(AShader *shader)
{
	float	rate(_win_x / _win_y);
	float	y_scale(1 / tan(M_PI * _fov / 180 / 2) * rate);
	float	x_scale(y_scale / rate);
	float	cone_len(_far - _near);

	_projMat[0][0] = x_scale;
	_projMat[1][1] = y_scale;
	_projMat[2][2] = -((_far + _near) / cone_len);
	_projMat[2][3] = -1;
	_projMat[3][2] = -((2 * _near * _far) / cone_len);
	_projMat[3][3] = 0;

	shader->start();
	shader->loadProjectionMatrix(_projMat);
	shader->stop();
}

void	renderEngine::createModelMatrix(const Entity entity, AShader *shader)
{
	// /!\ bind
	shader->start();
	shader->loadTransformationMatrix(entity._modelMatrix);
	shader->stop();
}

void	renderEngine::createViewMatrix(Camera *cam, AShader *shader)
{
	// /!\ bind
	shader->start();
	shader->loadViewMatrix(cam->_viewMatrix);
	shader->stop();
}

void	renderEngine::renderEntities(const std::vector<Entity> entities)
{

}