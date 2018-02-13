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

	glm::mat4	_viewMatrix;
	glm::vec3	_pos;
	glm::vec3	_lookAt;
	glm::vec3	_UpVec;
	glm::vec3	_rightVec;
	glm::vec3	_dir;

	bool		_tps;

	void		rotate(const glm::vec3 axe, float euler);
	void		rotateAround(const glm::vec3 axe, float euler);
	void		rotateIG(const glm::vec3 axe, float euler);
	void		move(const glm::vec3 delta);

	void		changeView(glm::vec3 pos);

	void		update(void);
	glm::mat4	getViewMatrix(void);
};

#endif //!CAMERA_HH_

