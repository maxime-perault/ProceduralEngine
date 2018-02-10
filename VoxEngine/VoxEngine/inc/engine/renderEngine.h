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

class	renderEngine
{
public:
	renderEngine(std::size_t win_x, std::size_t win_y);
	~renderEngine();

	void	createProjectionMatrix(AShader *shader);
	void	createModelMatrix(const Entity entity, AShader *shader);
	void	createViewMatrix(Camera *cam, AShader *shader);

	void	updateWindow(std::size_t win_x, std::size_t win_y);

	void	renderEntities(const std::vector<Entity> entities);

	glm::mat4 _projMat;
	glm::mat4 _viewMat;

	std::size_t	_win_y;
	std::size_t	_win_x;

	float		_fov;
	float		_near;
	float		_far;

	staticShader	*_staticShader;

	Camera		*_camera;
};

#endif //!RENDERENGINE_HH_
