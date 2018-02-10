#ifndef STATICSHADER_HH_
# define STATICSHADER_HH_

#include "AShader.h"

# define VERTEX_FILE "shaders/vertexShader"
# define FRAGMENT_FILE "shaders/fragmentShader"

class	AShader;

class	staticShader : public AShader
{
protected:
	virtual void	bindAttributes(void);

public:
	staticShader();
	virtual ~staticShader();
};

#endif //!STATICSHADER_HH_
