#include <windows.h>	
#include <stdio.h>


struct Render_state
{
	void* memory;
	int width;
	int height;
	BITMAPINFO bitmap_info;
};
bool running = true;
Render_state window_state;
#include "utilityFunctions.cpp"
#include "renderer.cpp"
#include "platformCommons.cpp"
#include "game.cpp"

LRESULT CALLBACK window_callback(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	LRESULT result = 0;
	switch (Msg)
	{
		case WM_CLOSE:
		{
			running = false;
		}
		break;
		case WM_DESTROY:
		{
			running = false;
		}
		break;
		case WM_SIZE:
		{
			RECT rectangle;
			GetClientRect(hWnd, &rectangle);
			window_state.width = rectangle.right - rectangle.left;
			window_state.height = rectangle.bottom - rectangle.top;

			int size = window_state.width * window_state.height * sizeof(unsigned int);

			if (window_state.memory) VirtualFree(window_state.memory, 0, MEM_RELEASE); //we check if the buffer memory pointer already has memory allocated. If so, we free it so we can reallocate it later
			window_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); //we allocate heap memory for the buffer. This will allow it to not go out of scope.

			window_state.bitmap_info.bmiHeader.biSize = sizeof(window_state.bitmap_info.bmiHeader);
			window_state.bitmap_info.bmiHeader.biWidth = window_state.width;
			window_state.bitmap_info.bmiHeader.biHeight = window_state.height;
			window_state.bitmap_info.bmiHeader.biPlanes = 1;
			window_state.bitmap_info.bmiHeader.biBitCount = 32;
			window_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
			typedef struct tagBITMAPINFOHEADER {
				DWORD biSize;
				LONG  biWidth;
				LONG  biHeight;
				WORD  biPlanes;
				WORD  biBitCount;
				DWORD biCompression;
				DWORD biSizeImage;
				LONG  biXPelsPerMeter;
				LONG  biYPelsPerMeter;
				DWORD biClrUsed;
				DWORD biClrImportant;
			} BITMAPINFOHEADER, * LPBITMAPINFOHEADER, * PBITMAPINFOHEADER;
		}
		break;
		default:
		{
			result = DefWindowProc(hWnd, Msg, wParam, lParam);
		}
		break;
	}
	return result;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//create window class
	WNDCLASS window_class = {};

	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Game Window";
	window_class.lpfnWndProc = window_callback;

	//register class
	RegisterClass(&window_class);


	// create window
	HWND window = CreateWindow(window_class.lpszClassName, "MY_FIRST_GAME", WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);
	
	Input input_array = {};
	float performance_freq;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_freq = perf.QuadPart;
	}

	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);
	while (running)
	{
		//input
		MSG message;
		for (int i = 0; i < BUTTON_COUNT; ++i)
		{
			input_array.buttons[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			
			switch (message.message)
			{
			case WM_KEYUP:
			case WM_KEYDOWN:
				{
					unsigned int pressed_key_code = message.wParam;
					bool isDown = ((message.lParam & (1 << 31)) == 0);

					switch (pressed_key_code)
					{
					case VK_UP:
					{
						input_array.buttons[BUTTON_UP].changed = isDown != input_array.buttons[BUTTON_UP].isDown;
						input_array.buttons[BUTTON_UP].isDown = isDown;
						
					}
					break;
					case VK_DOWN:
					{
						input_array.buttons[BUTTON_DOWN].isDown = isDown;
						input_array.buttons[BUTTON_DOWN].changed = true;
					}
					break;
					case VK_LEFT:
					{
						input_array.buttons[BUTTON_LEFT].isDown = isDown;
						input_array.buttons[BUTTON_LEFT].changed = true;
					}
					break;
					case VK_RIGHT:
					{
						input_array.buttons[BUTTON_RIGHT].isDown = isDown;
						input_array.buttons[BUTTON_RIGHT].changed = true;
					}
					break;
					case VK_SHIFT:
					{
						input_array.buttons[BUTTON_SHIFT].isDown = isDown;
						input_array.buttons[BUTTON_SHIFT].changed = true;
					}
					break;
					case 0x47:
					{
						input_array.buttons[BUTTON_G].changed = isDown != input_array.buttons[BUTTON_G].isDown;
						input_array.buttons[BUTTON_G].isDown = isDown;
						
					}
					break;
					case VK_SPACE:
					{
						input_array.buttons[BUTTON_SPACE].isDown = isDown;
						input_array.buttons[BUTTON_SPACE].changed = true;
					}
					break;
					case 0x42:
					{
						input_array.buttons[BUTTON_B].changed = isDown != input_array.buttons[BUTTON_B].isDown;
						input_array.buttons[BUTTON_B].isDown = isDown;
					}
					default:
					{
						;
					}
					break;
					}
				}
				break;
				
			default:
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
				break;
			}

			}

		//simulate
		simulateGame(&input_array, delta_time);


		//render
		StretchDIBits(hdc, 0, 0, window_state.width, window_state.height, 0, 0, window_state.width, window_state.height, window_state.memory, &window_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);

		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_freq;
		frame_begin_time = frame_end_time;
	}

}
