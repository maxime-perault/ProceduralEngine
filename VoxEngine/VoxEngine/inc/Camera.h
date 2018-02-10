#ifndef CAMERA_HH_
# define CAMERA_HH_

# include <iostream>
# include <glm/glm.hpp>
# include <glm/gtc/type_ptr.hpp>
# include <glm/gtx/transform.hpp>

class	Camera
{
public:
	Camera(const glm::vec3 pos);
	~Camera();

	glm::mat4 _viewMatrix;

	void rotate(const glm::vec3 axe, float euler);
	void translate(const glm::vec3 pos);
};

#endif //!CAMERA_HH_

