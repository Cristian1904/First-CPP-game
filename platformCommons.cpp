

struct ButtonState
{
	bool isDown;
	bool changed;
};


enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_SHIFT,
	BUTTON_G,
	BUTTON_SPACE,
	BUTTON_B,

	BUTTON_COUNT,
};


struct Input
{
	ButtonState buttons[BUTTON_COUNT];
};