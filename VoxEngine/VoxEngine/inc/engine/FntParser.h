#ifndef FNTPARSER_HH_
# define FNTPARSER_HH_

# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>

class	FntParser
{
private:
	void	getInfo(const std::string line);
public:
	FntParser(const std::string path);
	~FntParser();

	int	atoi(const std::string val);

	struct s_info
	{
		int id;
		float x;
		float y;
		float w;
		float h;
		float xo;
		float yo;
		float xa;
	};

	std::vector<s_info>	_infos;
};

#endif // !FNTPARSER_HH_ 