#pragma once

class Deco
{
public:
	Coords pos;
	olc::Sprite *sprite;
	bool renderFromTop;

	Deco(Coords _pos, olc::Sprite* _sprite, bool _renderFromTop) : pos(_pos), sprite(_sprite), renderFromTop(_renderFromTop) {}
};