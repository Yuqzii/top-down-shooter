#pragma once

namespace UI {

enum XAnchorType {
	XANCHOR_NONE = 0,
	XANCHOR_LEFT,
	XANCHOR_CENTER,
	XANCHOR_RIGHT,
};

enum YAnchorType {
	YANCHOR_NONE = 0,
	YANCHOR_TOP,
	YANCHOR_MIDDLE,
	YANCHOR_BOTTOM,
};

struct AnchorType {
	XAnchorType x;
	YAnchorType y;
};

}
