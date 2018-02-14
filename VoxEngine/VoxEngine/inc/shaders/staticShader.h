#ifndef STATICSHADER_HH_
# define STATICSHADER_HH_

#include "AShader.h"

# define VERTEX_FILE "shaders/vertexShader.txt"
# define FRAGMENT_FILE "shaders/fragmentShader.txt"

class	AShader;

class	staticShader : public AShader
{
protected:
	virtual void	bindAttributes(void);
	virtual void	getAllUniformLocations(void);

	int	_location_transformation_matrix;
	int	_location_projection_matrix;
	int	_location_view_matrix;

	int _location_lightPos;
	int _location_lightColour;
	int _location_shineDamper;
	int _location_ambientLevel;

	int _location_rawColour;
	int _location_colour;

public:
	staticShader();
	virtual ~staticShader();

	void		loadTransformationMatrix(const glm::mat4 m) const;
	void		loadProjectionMatrix(const glm::mat4 m) const;
	void		loadViewMatrix(const glm::mat4 m) const;

	void		loadLight(const glm::vec3 pos, const glm::vec3 colour, const float damper, const float ambientLevel) const;
	void		loadRawColour(glm::vec3(colour)) const;
	void		unloadRawColour(void) const;
};

#endif //!STATICSHADER_HH_
