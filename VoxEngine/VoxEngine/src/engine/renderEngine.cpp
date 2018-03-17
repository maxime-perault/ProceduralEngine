
#include "renderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

renderEngine::renderEngine() {}

renderEngine::renderEngine(std::size_t win_x, std::size_t win_y)
{
	_projMat = glm::mat4(1.0);

	_fov = 70;
	_near = (float)0.1;
	_far = 120;
	_win_x = win_x;
	_win_y = win_y;

	createProjectionMatrix(_staticShader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	*/

	_vaosRendering = false;
}

renderEngine::~renderEngine()
{
	_staticShader.cleanUp();
	_fontShader.cleanUp();
	_shadowShader.cleanUp();
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
				if (chunks[x][y][z].Chunk._draw == true)
				{
					this->bindVAO(chunks[x][y][z].Chunk._model._rawModel._vao_id);

					glEnableVertexAttribArray(0);
					glEnableVertexAttribArray(1);
					glEnableVertexAttribArray(2);

					this->createModelMatrix(chunks[x][y][z].Chunk, _staticShader);
					glDrawElements(GL_TRIANGLES, chunks[x][y][z].Chunk._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);
				}
			}
}

void	renderEngine::staticRender(Camera& cam, World *world, const bool debug)
{
	Light&					sun = world->getSun();
	Entity&					player = world->getPlayer();
	std::vector<Entity>&	axis = world->getAxis();
	std::vector<Entity>&	text = world->getText();
	Entity&					wirelessCube = world->getWirelessCube();
	Entity&					water = world->getWater();

	
	/////////////////////////////////

	world->_fbo.bind(world->_fbo._depthFBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shadowShader.start();

	///////////////////////////// GET THE VIEW FRUSTUM CORNER //////////////////////////////////

	glm::vec3 sun_dir = glm::vec3(-50, 100, -50);
	sun_dir *= -1;
	sun_dir = glm::normalize(sun_dir);

	glm::vec3 sun_right = glm::normalize(glm::cross(sun_dir, glm::normalize(glm::vec3(0, 1, 0))));
	glm::vec3 sun_up = glm::cross(sun_right, sun_dir);

	float	hnear = 2.f * tanf(_fov / 2.f) * _near;
	float	wnear = hnear * ((float)_win_x / (float)_win_y);

	float	hfar = 2.f * tanf(_fov / 2.f) * _far;
	float	wfar = hfar * ((float)_win_x / (float)_win_y);

	glm::vec3	cnear = cam._pos + cam._dir * _near; // OK
	glm::vec3	cfar = cam._pos + cam._dir * _far; // OK

	glm::vec3	near_topleft = cnear + (cam._UpVec * (hnear / 2.f)) - (cam._rightVec * (wnear / 2.f));
	glm::vec3	near_topright = cnear + (cam._UpVec * (hnear / 2.f)) + (cam._rightVec * (wnear / 2.f));

	glm::vec3	near_botleft = cnear - (cam._UpVec * (hnear / 2.f)) - (cam._rightVec * (wnear / 2.f));
	glm::vec3	near_botright = cnear - (cam._UpVec * (hnear / 2.f)) + (cam._rightVec * (wnear / 2.f));

	glm::vec3	far_topleft = cfar + (cam._UpVec * (hfar / 2.f)) - (cam._rightVec * (wfar / 2.f));
	glm::vec3	far_topright = cfar + (cam._UpVec * (hfar / 2.f)) + (cam._rightVec * (wfar / 2.f));

	glm::vec3	far_botleft = cfar - (cam._UpVec * (hfar / 2.f)) - (cam._rightVec * (wfar / 2.f));
	glm::vec3	far_botright = cfar - (cam._UpVec * (hfar / 2.f)) + (cam._rightVec * (wfar / 2.f));



	///////////////////////////// !GET THE VIEW FRUSTUM CORNER //////////////////////////////////
	glm::mat4 depthViewMatrix;

	
	// Find the centroid
	
	glm::vec3 frustumCentroid = 
		(near_topleft + near_topright + near_botleft + near_botright
			+ far_topleft + far_topright + far_botleft + far_botright) / 8.f;


	//thin matrix method
	
	depthViewMatrix = glm::mat4(1.0);

	
	glm::vec3 center = frustumCentroid;

	center *= -1;

	float pitch = (float)acosf(glm::length(glm::vec2(sun_dir.x, sun_dir.z)));

	depthViewMatrix = glm::rotate(depthViewMatrix, pitch, glm::vec3(1, 0, 0));

	float yaw = (float)(atanf(sun_dir.x / sun_dir.z) * 180.f / M_PI);

	yaw = sun_dir.z > 0 ? yaw - 180.f : yaw;

	yaw = -(yaw * M_PI / 180.f);

	depthViewMatrix = glm::rotate(depthViewMatrix, yaw, glm::vec3(0, 1, 0));
	depthViewMatrix = glm::translate(depthViewMatrix, center);
	
	///////////////////////////////////////////////////////////////
	
	hnear = 2.f * tanf(_fov / 2.f) * _near;
	wnear = hnear * ((float)_win_x / (float)_win_y);

	hfar = 2.f * tanf(_fov / 2.f) * _far;
	wfar = hfar * ((float)_win_x / (float)_win_y);

	cnear = sun._entity._pos + sun_dir * _near;
	cfar = sun._entity._pos + sun_dir * _far;

	near_topleft = cnear + (sun_up * (hnear / 2.f)) - (sun_right * (wnear / 2.f));
	near_topright = cnear + (sun_up * (hnear / 2.f)) + (sun_right * (wnear / 2.f));

	near_botleft = cnear - (sun_up * (hnear / 2.f)) - (sun_right * (wnear / 2.f));
	near_botright = cnear - (sun_up * (hnear / 2.f)) + (sun_right * (wnear / 2.f));

	far_topleft = cfar + (sun_up * (hfar / 2.f)) - (sun_right * (wfar / 2.f));
	far_topright = cfar + (sun_up * (hfar / 2.f)) + (sun_right * (wfar / 2.f));

	far_botleft = cfar - (sun_up * (hfar / 2.f)) - (sun_right * (wfar / 2.f));
	far_botright = cfar - (sun_up * (hfar / 2.f)) + (sun_right * (wfar / 2.f));

	///////////////////////////////////////////////////////////////

	std::vector<glm::vec3> points;

	points.push_back(near_topleft);
	points.push_back(near_topright);
	points.push_back(near_botleft);
	points.push_back(near_botright);

	points.push_back(far_topleft);
	points.push_back(far_topright);
	points.push_back(far_botleft);
	points.push_back(far_botright);

	float minX, maxX, minY, maxY, minZ, maxZ;

	bool first = true;

	for (glm::vec3 point : points) {
		if (first) {
			minX = point.x;
			maxX = point.x;
			minY = point.y;
			maxY = point.y;
			minZ = point.z;
			maxZ = point.z;
			first = false;
			continue;
		}
		if (point.x > maxX) {
			maxX = point.x;
		}
		else if (point.x < minX) {
			minX = point.x;
		}
		if (point.y > maxY) {
			maxY = point.y;
		}
		else if (point.y < minY) {
			minY = point.y;
		}
		if (point.z > maxZ) {
			maxZ = point.z;
		}
		else if (point.z < minZ) {
			minZ = point.z;
		}
	}

	/////////////////////////////////////////////

	glm::mat4 depthProjectionMatrix = glm::mat4(1.0);

	depthProjectionMatrix[0][0] = 2.f / (maxX - minX);
	depthProjectionMatrix[1][1] = 2.f / (maxY - minY);
	depthProjectionMatrix[2][2] = -2.f / (maxZ - minZ);
	depthProjectionMatrix[3][3] = 1.0;

	glm::mat4 depthModelMatrix = glm::mat4(1.0);

	
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	//render
	for (std::size_t x(0); x < CHUNK_SIZE_X; ++x)
		for (std::size_t y(0); y < CHUNK_SIZE_Y; ++y)
			for (std::size_t z(0); z < CHUNK_SIZE_Z; ++z)
			{
				if (world->_chunks[x][y][z].Chunk._draw == true)
				{
					depthModelMatrix = world->_chunks[x][y][z].Chunk.getModelMatrix();
					depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
					_shadowShader.loadMVP(depthMVP);

					this->bindVAO(world->_chunks[x][y][z].Chunk._model._rawModel._vao_id);

					glEnableVertexAttribArray(0);

					glDrawElements(GL_TRIANGLES, world->_chunks[x][y][z].Chunk._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);
				}
			}
	//render player
	if (cam._tps == true)
	{
		depthModelMatrix = player.getModelMatrix();
		depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
		_shadowShader.loadMVP(depthMVP);

		this->bindVAO(player._model._rawModel._vao_id);

		glEnableVertexAttribArray(0);

		glDrawElements(GL_TRIANGLES, player._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);
	}

	//!render

	_shadowShader.stop();

	world->_fbo.unbind();

	/////////////////////////////////

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	///////////////////////////////////////

	_staticShader.start();

	_staticShader.loadProjectionMatrix(_projMat);

	glm::mat4 shadowProjViewMatrix;

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	shadowProjViewMatrix = biasMatrix * depthProjectionMatrix * depthViewMatrix;

	_staticShader.loadShadowMatrix(shadowProjViewMatrix);

	_staticShader.loadProjectionMatrix(_projMat);
	this->createViewMatrix(cam, _staticShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, player._model._texture._id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, world->_fbo._depthTexture._id);

	_staticShader.attTextures();

	//render sun
	_staticShader.loadLight(sun._entity._pos, sun._colour, sun._damper, sun._ambientLevel);
	this->renderVAO_oneTime(sun._entity);

	//render player
	if (cam._tps == true)
	this->renderVAO_oneTime(player);

	//render chunk
	this->renderChunks(world->_chunks);

	//render water

	this->bindVAO(water._model._rawModel._vao_id);
	glEnableVertexAttribArray(0);

	this->createModelMatrix(water, _staticShader);

	_staticShader.loadRawColour(water._colour);
	glDrawElements(GL_TRIANGLES, water._model._rawModel._vertex_count, GL_UNSIGNED_INT, 0);

	_staticShader.unloadRawColour();
	
	//render wirelessCube
	this->renderVAO_LINE_oneTime(wirelessCube);

	//render Axis
	if (debug == true)
		this->renderAxis(axis);

	_staticShader.stop();

	_staticShader.start();

	_staticShader.loadProjectionMatrix(_projMat);
	this->createViewMatrix(cam, _staticShader);

	_staticShader.loadLight(sun._entity._pos, sun._colour, sun._damper, sun._ambientLevel);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, world->_fbo._depthTexture._id);

	this->renderVAO_oneTime(world->_depth);

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