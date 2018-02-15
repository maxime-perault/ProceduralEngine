
#ifndef ENTITY_HH_
# define ENTITY_HH_

# include "texturedModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL.h>

class	Entity
{
private:
	glm::mat4	_modelMatrix;

	void	translate(glm::vec3 pos);
	void	rotate(glm::vec3 axe, float euler);
	void	scale(glm::vec3 scal);
public:
	Entity(texturedModel *model);
	~Entity();

	texturedModel	*_model;
	glm::vec3		_pos;
	glm::vec3		_rot;
	glm::vec3		_scale;

	glm::vec3		_colour;

	bool			_draw;

	glm::mat4		getModelMatrix(void);
};

#endif //!ENTITY_HH_
