#ifndef RENDERENGINE_HH_
# define RENDERENGINE_HH_

# include <iostream>
# include <gl/glew.h>
# include <GL/glu.h>
# include <glm/glm.hpp>
# include <SDL2/SDL.h>
# include <vector>
# include "staticShader.h"
# include "fontShader.h"
# include "Camera.h"
# include "Entity.h"
# include "World.h"

class	renderEngine
{
public:
	renderEngine();
	renderEngine(std::size_t win_x, std::size_t win_y);
	~renderEngine();

	void	createProjectionMatrix(staticShader& shader);
	void	createModelMatrix(Entity entity, staticShader& shader);
	void	createViewMatrix(Camera& cam, staticShader& shader);

	void	updateWindow(std::size_t win_x, std::size_t win_y);

	void	renderVAO_oneTime(Entity& entity);
	void	renderVAO_multipleTime(std::vector<Entity>& entity);

	void	renderAxis(std::vector<Entity> axis);
	void	renderText(std::vector<Entity> text);
	void	renderEngine::renderChunks(s_chunk (&chunks)[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z]);

	void	staticRender(Camera& cam, World *world, const bool debug);
	void	fontRender(World *world, const bool debug);

	void	renderWorld(Camera& cam, World *world, const bool debug);

	void	stop(void);

	glm::mat4	_projMat;

	std::size_t	_win_y;
	std::size_t	_win_x;

	float		_fov;
	float		_near;
	float		_far;

	staticShader	_staticShader;
	fontShader		_fontShader;
};

#endif //!RENDERENGINE_HH_
