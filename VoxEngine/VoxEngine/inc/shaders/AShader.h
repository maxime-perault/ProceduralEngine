#ifndef ASHADER_HH_
# define ASHADER_HH_

# include <SDL2/SDL.h>
# include <GL/glew.h>
# include <GL/gl.h>
# include <GL/glu.h>
# include <cmath>
# include <glm/glm.hpp>
# include <glm/gtc/type_ptr.hpp>
# include <string>

class	AShader
{
protected:
	int	_program_id;
	int	_vertex_shader_id;
	int	_fragment_shader_id;

	int		loadShader(const std::string file, const int type);

	void	CheckShader(GLuint id, GLuint type, GLint *ret, const char *onfail);

	virtual void	bindAttribute(const int attribute, const GLchar *variableName);

	int	       		getUniformLocation(const GLchar *uniformName) const;
public:

	AShader(const std::string vertexFile, const std::string fragmentFile);
	virtual ~AShader();

	void		loadFloat(const int location, const float value) const;
	void		loadBool(const int location, const bool value) const;
	void		loadVector(const int location, const glm::vec4 v) const;
	void		loadVector(const int location, const glm::vec3 v) const;
	void		loadVector(const int location, const glm::vec2 v) const;
	void		loadMatrix(const int location, const glm::mat4 m) const;

	void	start(void);
	void	stop(void);
	void	cleanUp(void);
};

#endif //!ASHADER_HH_
