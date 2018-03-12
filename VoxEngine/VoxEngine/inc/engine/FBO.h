#ifndef FBO_HH_
# define FBO_HH_

#include <iostream>
#include <gl/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "cubeFactory.h"

class FBO
{
public:
	FBO();
	~FBO();

	GLuint	_depthFBO;
	GLuint	_depthBuffer;
	Texture	_depthTexture;

	cubeFactory*	_cubeFactory;

	void	setCubeFactory(cubeFactory* cubeFactory);

	void	bind(GLuint fb);
	void	unbind(void);

	GLuint	createFB(void);
	Entity	CreateDepthVAO(int w, int h);

	GLuint	CreateDepthTexture(int w, int h);
	GLuint	CreateColorTexture(int w, int h);

	GLuint	CreateDepthBuffer(int w, int h);
};


#endif //!FBO_HH_