#include "anchor.h"


Anchor operator|(Anchor a, Anchor b)
{
	return static_cast<Anchor>(static_cast<int>(a) | static_cast<int>(b));
}

Anchor operator&(Anchor a, Anchor b)
{
	return static_cast<Anchor>(static_cast<int>(a) & static_cast<int>(b));
}
