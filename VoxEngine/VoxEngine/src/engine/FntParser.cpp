
#include "FntParser.h"

FntParser::FntParser(const std::string path)
{
	std::ifstream file(path);

	if (file.is_open() == false)
	{
		std::cout << "Error, can't open " << path << std::endl;
		return;
	}

	s_info tmp = {-1};
	for (std::size_t i = 0; i < 127; ++i)
		_infos.push_back(tmp);

	for (std::string line; std::getline(file, line); )
	{
		if (strncmp(line.c_str(), "char ", strlen("char ")) == 0)
			this->getInfo(line);
	}
	/*
	for (std::size_t i(0); i < _infos.size(); ++i)
	{
		std::cout << _infos[i].id << "; " << _infos[i].x << "; " << _infos[i].y << "; "
			<< _infos[i].w << "; " << _infos[i].h << "; "
			<< _infos[i].xo << "; " << _infos[i].yo << "; " << _infos[i].xa << std::endl;
	}
	*/
}

FntParser::~FntParser()
{

}

int	FntParser::atoi(const std::string val)
{
	std::stringstream geek(val);

	int res = 0;
	geek >> res;
	return res;
}

// cursors are the same if the .fnt is generated from hiero converter
// https://libgdx.badlogicgames.com/tools.html
void	FntParser::getInfo(const std::string line)
{
	std::size_t x;
	std::string	buffer;
	s_info	res;

	x = 8;
	while (line[x] != ' ')
		buffer += line[x++];
	res.id = this->atoi(buffer);

	buffer.clear();

	x = 18;
	while (line[x] != ' ')
		buffer += line[x++];
	res.x = (float)this->atoi(buffer) / 512;
	buffer.clear();

	x = 25;
	while (line[x] != ' ')
		buffer += line[x++];
	res.y = (float)this->atoi(buffer) / 512;
	buffer.clear();

	x = 36;
	while (line[x] != ' ')
		buffer += line[x++];
	res.w = (float)this->atoi(buffer) / 512;
	buffer.clear();

	x = 48;
	while (line[x] != ' ')
		buffer += line[x++];
	res.h = (float)this->atoi(buffer) / 512;
	buffer.clear();

	x = 61;
	while (line[x] != ' ')
		buffer += line[x++];
	res.xo = (float)this->atoi(buffer) / 512;
	buffer.clear();

	x = 74;
	while (line[x] != ' ')
		buffer += line[x++];
	res.yo = (float)this->atoi(buffer) / 512;
	buffer.clear();

	x = 88;
	while (line[x] != ' ')
		buffer += line[x++];
	res.xa = (float)this->atoi(buffer) / 512;
	buffer.clear();

	_infos[res.id] = res;
}