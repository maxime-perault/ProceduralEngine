#ifndef SHADOWSHADER_HH_
# define SHADOWSHADER_HH_

#include "AShader.h"

class	AShader;

class	shadowShader : public AShader
{
protected:
	virtual void	bindAttributes(void);
	virtual void	getAllUniformLocations(void);

	int		_location_depthMVP;


public:
	shadowShader();
	virtual ~shadowShader();

	void	loadMVP(const glm::mat4 mvp);

};

#endif //!SHADOWSHADER_HH_
