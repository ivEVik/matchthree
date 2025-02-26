#ifndef ANCHOR_H
#define ANCHOR_H


enum class Anchor
{
	NONE = 0,
	TOP = 1 << 0,
	BOTTOM = 1 << 1,
	LEFT = 1 << 2,
	RIGHT = 1 << 3,
	CENTRE_H = 1 << 4,
	CENTRE_V = 1 << 5
};

Anchor operator|(Anchor a, Anchor b);
Anchor operator&(Anchor a, Anchor b);


#endif // ANCHOR_H
