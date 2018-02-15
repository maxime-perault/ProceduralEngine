
#include "FontFactory.h"

FontFactory::FontFactory()
{
	_loader = VAOLoader();
	_fnt = new FntParser("assets/Fonts/roboto_light.fnt");
	_texture = Texture(_loader.loadTexture("assets/Fonts/roboto_light.png"));

	this->initText("FPS: Loading", true, FPS);
	this->initText("XYZ: Loading", true, XYZ);

	//define it also in getText
	_size = 2;
}

FontFactory::~FontFactory()
{
	_loader.cleanUp();
}


void	FontFactory::initText(const std::string txt, bool create, const e_texts type)
{
	//dont change if the screen is resized -> TODO
	float	ratio = (1280.f / 720.f);
	float	size = 0.25;

	std::vector<float> text_uv;
	std::vector<float> text_vtx;
	std::vector<int> text_ind;

	float	xad = 0;
	int		ascii;

	//y coordinates are fliped to right it properly from (-1; -1) without y inversion
	for (std::size_t i(0); i < txt.size(); ++i)
	{
		ascii = (char)txt[i];
		if (_fnt->_infos[ascii].id == -1)
			ascii = ' ';

		//VTX
		text_vtx.push_back(((_fnt->_infos[ascii].xo + xad) / ratio) * size); //X1 TOP LEFT
		text_vtx.push_back(_fnt->_infos[ascii].yo * -size); //Y1 TOP LEFT
		text_vtx.push_back((((_fnt->_infos[ascii].xo + xad) + _fnt->_infos[ascii].w) / ratio) * size); //X2 TOP RIGHT
		text_vtx.push_back(_fnt->_infos[ascii].yo * -size); //Y2 TOP RIGHT
		text_vtx.push_back((((_fnt->_infos[ascii].xo + xad) + _fnt->_infos[ascii].w) / ratio) * size); //X3 BOT RIGHT
		text_vtx.push_back((_fnt->_infos[ascii].yo + _fnt->_infos[ascii].h) * -size); //Y3 BOT RIGHT
		text_vtx.push_back(((_fnt->_infos[ascii].xo + xad) / ratio) * size); //X4 BOT LEFT
		text_vtx.push_back((_fnt->_infos[ascii].yo + _fnt->_infos[ascii].h) * -size); //Y4 BOT LEFT

		//INVERTED UV
		text_uv.push_back(_fnt->_infos[ascii].x);
		text_uv.push_back(_fnt->_infos[ascii].y);
		text_uv.push_back(_fnt->_infos[ascii].x + _fnt->_infos[ascii].w);
		text_uv.push_back(_fnt->_infos[ascii].y);
		text_uv.push_back(_fnt->_infos[ascii].x + _fnt->_infos[ascii].w);
		text_uv.push_back(_fnt->_infos[ascii].y + _fnt->_infos[ascii].h);
		text_uv.push_back(_fnt->_infos[ascii].x);
		text_uv.push_back(_fnt->_infos[ascii].y + _fnt->_infos[ascii].h);

		//INDEXING
		text_ind.push_back(0 + i * 4);
		text_ind.push_back(1 + i * 4);
		text_ind.push_back(3 + i * 4);

		text_ind.push_back(1 + i * 4);
		text_ind.push_back(3 + i * 4);
		text_ind.push_back(2 + i * 4);

		xad += _fnt->_infos[ascii].xa;
	}

	if (create == false)
	{
		_models[type] = _loader.loadtoVAO(text_vtx, text_uv, text_ind);
		_tex_models[type] = texturedModel(_models[type], _texture);
		_entities[type] = Entity(_tex_models[type]);
	}
	else
	{
		_models.push_back(_loader.loadtoVAO(text_vtx, text_uv, text_ind));
		_tex_models.push_back(texturedModel(_models[type], _texture));
		_entities.push_back(Entity(_tex_models[type]));
	}
}

Entity	FontFactory::getText(std::string text, glm::vec3 pos, const e_texts type)
{
	static std::string s_text[2] = { "" };
	Entity	res;

	if (s_text[type] == text)
	{
		res = _entities[type];
		res._pos = pos;
		return res;
	}

	_loader.deleteVAO(_models[type]._vao_id);
	
	this->initText(text, false, type);
	
	res = _entities[type];
	res._pos = pos;

	s_text[type] = text;
	return (res);
}
