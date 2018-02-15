#ifndef FONTSHADER_HH_
# define FONTSHADER_HH_

#include "AShader.h"

class	AShader;

class	fontShader : public AShader
{
protected:
	virtual void	bindAttributes(void);
	virtual void	getAllUniformLocations(void);

	int		_location_translation;
	int		_location_colour;

public:
	fontShader();
	virtual ~fontShader();

	void	loadColour(const glm::vec3 colour);
	void	loadPos(const glm::vec2 pos);
};

#endif //!FONTSHADER_HH_
