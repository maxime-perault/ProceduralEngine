
#include "FBO.h"

FBO::FBO()
{
	_depthFBO = this->createFB();

	_depthTexture = Texture(this->CreateDepthTexture(2048, 2048));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Error: FBO ctor" << std::endl;

	this->unbind();
}

FBO::~FBO()
{
	glDeleteFramebuffers(1, &_depthFBO);

	GLuint text = _depthTexture._id;
	glDeleteTextures(1, &text);
}

void		FBO::setCubeFactory(cubeFactory* cubeFactory)
{
	_cubeFactory = cubeFactory;
}

void	FBO::bind(GLuint fb)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
	glViewport(0, 0, 2048, 2048);
}

void	FBO::unbind(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);
}

Entity	FBO::CreateDepthVAO(int w, int h)
{
	std::vector<int>	ind;
	std::vector<float>	uv;
	std::vector<float>	vtx;
	std::vector<float>	normals;

	//SHADOWMAP
	//h = w;

	vtx.insert(vtx.end(), {
		0, 0, 0,
		(float)w / 500.f, 0, 0,
		(float)w / 500.f, (float)h / 500.f, 0,
		0, (float)h / 500.f, 0,
	});
	
	normals.insert(normals.end(), {
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1 });

	uv.insert(uv.end(), {
		0, 0,
		1, 0,
		1, 1,
		0, 1});
	ind.insert(ind.end(), {
		0, 1, 3,
		3, 1, 2
	});

	Entity res = Entity(texturedModel(_cubeFactory->_loader.loadtoVAO(vtx, normals, uv, ind), _depthTexture));

	return res;
}

GLuint	FBO::CreateDepthTexture(int w, int h)
{
	GLuint	id;

	//SHADOWMAP
	//h = w;

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0);

	return (id);
}

GLuint	FBO::CreateColorTexture(int w, int h)
{
	GLuint	id;

	//SHADOWMAP
	//h = w;

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, id, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	return (id);
}

GLuint	FBO::CreateDepthBuffer(int w, int h)
{
	GLuint depthrenderbuffer;

	//SHADOWMAP
	//h = w;

	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	return depthrenderbuffer;
}

GLuint	FBO::createFB(void)
{
	GLuint	id;

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	return (id);
}