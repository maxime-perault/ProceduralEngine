
#ifndef ENTITY_HH_
# define ENTITY_HH_

# include "texturedModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL.h>

class	Entity
{
public:
	Entity(texturedModel *model);
	~Entity();

	texturedModel	*_model;
	glm::mat4		_modelMatrix;

	void	translate(glm::vec3 pos);
	void	rotation(glm::vec3 axe, float euler);

	void	scale(glm::vec3 scal);
};

#endif //!ENTITY_HH_
