#pragma once

class Deco
{
public:
	Coords pos;
	olc::Sprite *sprite;
	olc::Sprite* emissive;
	bool renderFromTop;

	Deco(Coords _pos, olc::Sprite *_sprite, olc::Sprite *_emissive, bool _renderFromTop) : 
		pos(_pos), sprite(_sprite), emissive(_emissive), renderFromTop(_renderFromTop) {}
};