#include <windows.h>
#include <string>
#include <cstdlib>
#include <math.h>
#include <ctime>

bool running = true;
void* buffer_memory;
int buffer_width;
int buffer_height;
int buffer_size;
long double originX = 0;
long double originY = 0;
long double zoom = 0.003;
BITMAPINFO buffer_bitmap_info;

//increasing this number will improve the accuracy of the result, but also increase the render time.
//100 is a good value.
int detail = 100;

#include "renderer.cpp"

struct complex_number {
	long double real_component;
	long double imaginary_component;
};

complex_number complex_addition(complex_number first, complex_number second) {
	complex_number answer;
	answer.real_component = first.real_component + second.real_component;
	answer.imaginary_component = first.imaginary_component + second.imaginary_component;
	return answer;
};

complex_number complex_square(complex_number input) {
	complex_number answer;
	answer.imaginary_component = 2 * (input.real_component * input.imaginary_component);
	answer.real_component = (input.real_component * input.real_component) + (input.imaginary_component * input.imaginary_component * -1);
	return answer;
};

bool mandelbrot(complex_number coordinate_number) {
	complex_number z;
	z.real_component = 0;
	z.imaginary_component = 0;
	
	for (int i = 0; i < detail; i++) {
		z = complex_addition(complex_square(z), coordinate_number);
		if (abs(z.real_component) > 2) {
			return false;
		}
	}
	return true;
};

bool testbrot(complex_number coordinate_number) {
	if (abs(fmod(coordinate_number.real_component, 1)) < 0.02) {
		return true;
	}

	if (abs(fmod(coordinate_number.imaginary_component, 1)) < 0.02) {
		return true;
	}

	return false;
}

void start();

void update();

LRESULT CALLBACK window_callback(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
	case WM_CLOSE:

	case WM_DESTROY: {
		running = false;
	} break;

	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		buffer_width = rect.right - rect.left;
		buffer_height = rect.bottom - rect.top;

		buffer_size = buffer_width * buffer_height * sizeof(unsigned int);

		if (buffer_memory) {
			VirtualFree(buffer_memory, 0, MEM_RELEASE);
		}
		buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
		buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
		buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
		buffer_bitmap_info.bmiHeader.biPlanes = 1;
		buffer_bitmap_info.bmiHeader.biBitCount = 32;
		buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
		update();

	} break;

	case WM_KEYDOWN: {
		switch (wParam) {

		case VK_LEFT: {
			originX = originX - (100 * zoom);
			update();
		} break;

		case VK_RIGHT: {
			originX = originX + (100 * zoom);
			update();
		} break;

		case VK_UP: {
			originY = originY + (100 * zoom);
			update();
		} break;

		case VK_DOWN: {
			originY = originY - (100 * zoom);
			update();
		} break;

		case 0x49: {
			zoom = zoom * 0.5;
			update();
		} break;

		case 0x4f: {
			zoom = zoom * 2;
			update();
		} break;

		}
	} break;

	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
	return result;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Create Window Class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Game Window Class";
	window_class.lpfnWndProc = window_callback;

	// Register Class
	RegisterClass(&window_class);

	// Create Window
	HWND window = CreateWindow(window_class.lpszClassName, "Mandelbrot", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	//create Map
	//std::string map;

	srand(time(NULL));

	//start
	start();

	while (running) {
		//Input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		//update
		//update();

		//Render
		StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
}

void start() {
	update();
}

void update() {
	complex_number coordinates;
	coordinates.real_component = originX - (((long double)buffer_width / 2) * zoom);
	coordinates.imaginary_component = originY - (((long double)buffer_height / 2) * zoom);
	clear_screen(0xffffff);
	for (int i = 0; i < buffer_height; i++) {
		for (int j = 0; j < buffer_width; j++) {
			//assign coordinates the correct values
			coordinates.real_component = coordinates.real_component + zoom;

			if (mandelbrot(coordinates) == true) {
				draw_pixel(j, i, 0x000000);
			}

			/*
			if (testbrot(coordinates) == true) {
				draw_pixel(j, i, 0x55ff55);
			}
			*/
		}
		coordinates.real_component = originX - (((long double)buffer_width / 2) * zoom);
		coordinates.imaginary_component = coordinates.imaginary_component + zoom;
	}
}

