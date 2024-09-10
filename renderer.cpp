

void renderBackground()
{
	unsigned int* pixel = (unsigned int*)window_state.memory;
	for (int i = 0; i < window_state.height; ++i)
	{
		for (int j = 0; j < window_state.width; ++j)
		{
			*pixel++ = 0x00ff00;
		}
	}
}

void clearScreen(unsigned int color)
{
	unsigned int* pixel = (unsigned int*)window_state.memory;
	for (int i = 0; i < window_state.height; ++i)
	{
		for (int j = 0; j < window_state.width; ++j)
		{
			*pixel++ = color;
		}
	}
}

void createRectangleInPixels(int x_position,int y_position,int width, int height, unsigned int color)
{
	x_position = clamp(0, x_position, window_state.width);
	y_position = clamp(0, y_position, window_state.height);
	height = clamp(0, height, window_state.height);
	width = clamp(0, width, window_state.width);
	
	
	for (int i = y_position; i < height; ++i)
	{
		unsigned int* pixel = (unsigned int*)window_state.memory + x_position + i*window_state.width;
		for (int j = x_position; j < width; j++)
		{
			*pixel++ = color;
		}
	}
}

void createRectangleSizeRelated(float x, float y, float half_size_x, float half_size_y, unsigned int color) //trouble understanding the conversion process!!! To relearn later
{
	x *= window_state.height;
	y *= window_state.height;
	half_size_x *= window_state.height;
	half_size_y *= window_state.height;

	x += window_state.width / 2;
	y += window_state.height / 2;

	int x0 = x - half_size_x;
    int y0 = y - half_size_y; // Corrected calculation
    int x1 = x + half_size_x;
    int y1 = y + half_size_y;

	
	createRectangleInPixels(x0, y0, x1, y1, color);
}

void createRectangleSizeAndPixel(float x, float y, float size_x, float size_y, unsigned int color) //trouble understanding the conversion process!!! To relearn later
{
	size_x *= window_state.height;
	size_y *= window_state.height;

	createRectangleInPixels(x, y, size_x, size_y, color);
}



