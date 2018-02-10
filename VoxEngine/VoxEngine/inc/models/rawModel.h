
#ifndef RAWMODEL_H_
# define RAWMODEL_H_

# include <iostream>

class	rawModel
{
public:
	rawModel(const std::size_t vao_id, const std::size_t vertex_count);
	~rawModel();

	std::size_t	_vao_id;
	std::size_t	_vertex_count;
};

#endif //!RAWMODEL_H_
