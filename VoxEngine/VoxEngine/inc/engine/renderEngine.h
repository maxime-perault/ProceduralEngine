#ifndef RENDERENGINE_HH_
# define RENDERENGINE_HH_

# include <iostream>
# include <gl/glew.h>
# include <GL/glu.h>
# include <glm/glm.hpp>
# include <SDL2/SDL.h>
# include <vector>
# include "staticShader.h"
# include "Camera.h"
# include "Entity.h"
# include "cubeFactory.h"

class	renderEngine
{
public:
	renderEngine(std::size_t win_x, std::size_t win_y);
	~renderEngine();

	void	createProjectionMatrix(staticShader *shader);
	void	createOrthographicProjectionMatrix(staticShader *shader);
	void	createModelMatrix(Entity *entity, staticShader *shader);
	void	createViewMatrix(Camera *cam, staticShader *shader);

	void	updateWindow(std::size_t win_x, std::size_t win_y);

	void	renderEntities(void);

	glm::mat4	_projMat;

	std::size_t	_win_y;
	std::size_t	_win_x;

	float		_fov;
	float		_near;
	float		_far;

	Camera		*_camera;

	cubeFactory			_cubeFactory;
	std::vector<Entity*> _entities;

	staticShader	*_staticShader;
};

#endif //!RENDERENGINE_HH_
