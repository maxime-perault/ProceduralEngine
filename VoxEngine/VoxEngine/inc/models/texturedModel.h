
#ifndef TEXTUREDMODEL_HH_
# define TEXTUREDMODEL_HH_

# include "rawModel.h"
# include "Texture.h"

class	texturedModel
{
public:
	texturedModel(rawModel *rawModel, Texture *texture);
	~texturedModel();

	rawModel	*_rawModel;
	Texture		*_texture;
};

#endif //!TEXTUREDMODEL_HH_