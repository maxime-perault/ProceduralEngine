
#ifndef FONTFACTORY_HH_
# define FONTFACTORY_HH_

# include "VAOLoader.h"
# include "Entity.h"
# include "FntParser.h"

class	FontFactory
{
public:
	enum e_texts
	{
		FPS = 0,
		XYZ
	};

	Entity	getText(std::string text, glm::vec3 pos, const e_texts type);
	void	initText(const std::string, bool create, const e_texts type);

	std::vector<Entity>		_entities;
	int		_size;

	FontFactory();
	~FontFactory();
private:
	VAOLoader		_loader;

	Texture		_texture;
	FntParser	*_fnt;

	std::vector<rawModel>		_models;
	std::vector<texturedModel>	_tex_models;
};

#endif //!FONTFACTORY_HH_
