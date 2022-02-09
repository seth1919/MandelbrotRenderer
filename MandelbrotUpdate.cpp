#include <windows.h>
#include <string>
#include <cstdlib>
#include <math.h>
#include <ctime>

struct exact_number {
	long int numerator;
	long int denominator;
};

exact_number exact_addition(exact_number first, exact_number second) {
	exact_number answer;
	long int denominator1 = first.denominator;
	long int first_count = 0;
	long int denominator2 = second.denominator;
	long int second_count = 0;

	if (first.denominator == 0) {
		return second;
	}

	if (second.denominator == 0) {
		return first;
	}

	while (denominator1 != denominator2) {
		while (denominator1 < denominator2) {
			first_count++;
			denominator1 = denominator1 + first.denominator;
		}
		while (denominator2 < denominator1) {
			second_count++;
			denominator2 = denominator2 + second.denominator;
		}
	}

	answer.numerator = (first.numerator * first_count) + (second.numerator * second_count);
	answer.denominator = denominator1;

	//simplify
	long int divisor = 2;
	while (divisor < answer.numerator && divisor < answer.denominator) {
		while (answer.numerator % divisor == 0 && answer.denominator % divisor == 0) {
			answer.numerator = answer.numerator / divisor;
			answer.denominator = answer.denominator / divisor;
		}
		divisor++;
	}

	return answer;
};

exact_number exact_subtraction(exact_number first, exact_number second) {
	exact_number answer;
	long int denominator1 = first.denominator;
	long int first_count = 0;
	long int denominator2 = second.denominator;
	long int second_count = 0;

	while (denominator1 != denominator2) {
		while (denominator1 < denominator2) {
			first_count++;
			denominator1 = denominator1 + first.denominator;
		}
		while (denominator2 < denominator1) {
			second_count++;
			denominator2 = denominator2 + second.denominator;
		}
	}

	answer.numerator = (first.numerator * first_count) - (second.numerator * second_count);
	answer.denominator = denominator1;

	//simplify
	long int divisor = 2;
	while (divisor < answer.numerator && divisor < answer.denominator) {
		while (answer.numerator % divisor == 0 && answer.denominator % divisor == 0) {
			answer.numerator = answer.numerator / divisor;
			answer.denominator = answer.denominator / divisor;
		}
		divisor++;
	}

	return answer;
};

exact_number exact_multiplication(exact_number first, exact_number second) {
	exact_number answer;
	answer.numerator = first.numerator * second.numerator;
	answer.denominator = first.denominator * second.denominator;

	//simplify
	long int divisor = 2;
	while (divisor < answer.numerator && divisor < answer.denominator) {
		while (answer.numerator % divisor == 0 && answer.denominator % divisor == 0) {
			answer.numerator = answer.numerator / divisor;
			answer.denominator = answer.denominator / divisor;
		}
		divisor++;
	}

	return answer;
};

bool exact_greater_than(exact_number first, exact_number second) {
	exact_number answer_first;
	exact_number answer_second;
	long int denominator1 = first.denominator;
	long int first_count = 0;
	long int denominator2 = second.denominator;
	long int second_count = 0;

	while (denominator1 != denominator2) {
		while (denominator1 < denominator2) {
			first_count++;
			denominator1 = denominator1 + first.denominator;
		}
		while (denominator2 < denominator1) {
			second_count++;
			denominator2 = denominator2 + second.denominator;
		}
	}

	answer_first.numerator = (first.numerator * first_count);
	answer_second.numerator = (second.numerator * second_count);
	answer_first.denominator = denominator1;
	answer_second.denominator = denominator1;

	if (answer_first.numerator > answer_second.numerator) {
		return true;
	}
	else {
		return false;
	}
}

struct complex_number {
	exact_number real_component;
	exact_number imaginary_component;
};

complex_number complex_addition(complex_number first, complex_number second) {
	complex_number answer;
	answer.real_component = exact_addition(first.real_component, second.real_component);
	answer.imaginary_component = exact_addition(first.imaginary_component, second.imaginary_component);
	return answer;
};

complex_number complex_square(complex_number input) {
	complex_number answer;

	exact_number two;
	two.numerator = 2;
	two.denominator = 1;

	exact_number negative_one;
	negative_one.numerator = -1;
	negative_one.denominator = 1;

	answer.imaginary_component = exact_multiplication(two, (exact_multiplication(input.real_component, input.imaginary_component)));
	answer.real_component = exact_addition((exact_multiplication(input.real_component, input.real_component)), (exact_multiplication(exact_multiplication(input.imaginary_component, input.imaginary_component), negative_one)));
	return answer;
};

bool mandelbrot(complex_number coordinate_number) {
	exact_number two;
	two.numerator = 2;
	two.denominator = 1;

	complex_number z;
	z.real_component.numerator = 0;
	z.real_component.denominator = 1;
	z.imaginary_component.numerator = 0;
	z.imaginary_component.denominator = 1;

	for (int i = 0; i < 80; i++) {
		z = complex_addition(complex_square(z), coordinate_number);
		if (exact_greater_than(z.real_component, two)) {
			return false;
		}
	}
	return true;
};

/*
bool testbrot(complex_number coordinate_number) {
	if (abs(fmod(coordinate_number.real_component, 1)) < 0.1) {
		return true;
	}

	if (abs(fmod(coordinate_number.imaginary_component, 1)) < 0.1) {
		return true;
	}

	return false;
}
*/

bool running = true;
void* buffer_memory;
int buffer_width;
int buffer_height;
int buffer_size;
exact_number originX;
exact_number originY;
exact_number zoom;
BITMAPINFO buffer_bitmap_info;

#include "renderer.cpp"

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

	} break;

	case WM_KEYDOWN: {
		switch (wParam) {

		case VK_LEFT: {
			exact_number one_hundred;
			one_hundred.numerator = 100;
			one_hundred.denominator = 1;
			originX = exact_subtraction(originX, (exact_multiplication(one_hundred, zoom)));
		} break;

		case VK_RIGHT: {
			exact_number one_hundred;
			one_hundred.numerator = 100;
			one_hundred.denominator = 1;
			originX = exact_addition(originX, (exact_multiplication(one_hundred, zoom)));
		} break;

		case VK_UP: {
			exact_number one_hundred;
			one_hundred.numerator = 100;
			one_hundred.denominator = 1;
			originY = exact_addition(originY, (exact_multiplication(one_hundred, zoom)));
		} break;

		case VK_DOWN: {
			exact_number one_hundred;
			one_hundred.numerator = 100;
			one_hundred.denominator = 1;
			originY = exact_subtraction(originY, (exact_multiplication(one_hundred, zoom)));
		} break;

		case 0x49: {
			exact_number one_half;
			one_half.numerator = 1;
			one_half.denominator = 2;
			zoom = exact_multiplication( zoom, one_half);
		} break;

		case 0x4f: {
			exact_number two;
			two.numerator = 2;
			two.denominator = 1;
			zoom = exact_multiplication(zoom, two);
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
	HWND window = CreateWindow(window_class.lpszClassName, "My First Game!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
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
		update();

		//Render
		StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
}

void start() {
	originX.numerator = 0;
	originX.denominator = 1;
	originY.numerator = 0;
	originY.denominator = 1;
	zoom.numerator = 2;
	zoom.denominator = 1;
}

void update() {
	complex_number coordinates;
	exact_number buffer_width_over_two;
	buffer_width_over_two.numerator = buffer_width;
	buffer_width_over_two.denominator = 2;
	exact_number buffer_height_over_two;
	buffer_height_over_two.numerator = buffer_height;
	buffer_height_over_two.denominator = 2;
	coordinates.real_component = exact_subtraction(originX, (exact_multiplication(buffer_width_over_two, zoom)));
	coordinates.imaginary_component = exact_subtraction(originY, (exact_multiplication(buffer_height_over_two, zoom)));
	clear_screen(0xffffff);
	for (int i = 0; i < buffer_height; i++) {
		for (int j = 0; j < buffer_width; j++) {
			//assign coordinates the correct values
			coordinates.real_component = exact_addition(coordinates.real_component, zoom);

			if (mandelbrot(coordinates) == true) {
				draw_pixel(j, i, 0x000000);
			}
		}
		coordinates.real_component = exact_subtraction(originX, (exact_multiplication(buffer_width_over_two, zoom)));
		coordinates.imaginary_component = exact_addition(coordinates.imaginary_component, zoom);
	}
}

