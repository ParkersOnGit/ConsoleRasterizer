#include <iostream>
#include <conio.h>
#include <vector>
#include <windows.h>

// Utilities
struct Vector3i {
public:
	int x, y, z;
	int& r = x; int& g = y; int& b = z;
	int& s = x; int& t = y; int& p = z;
};

struct Vector2i {
public:
	int x, y;
	int& r = x; int& g = y;
	int& s = x; int& t = y;
};

struct Vector3f {
public:
	float x, y, z;
	float& r = x; float& g = y; float& b = z;
	float& s = x; float& t = y; float& p = z;
};

struct Vector2f {
public:
	float x, y;
	float& r = x; float& g = y;
	float& s = x; float& t = y;
};

// Console
static class Console {
public:
	static void print(const std::string& string) { std::cout << string; }
	static void printLine(const std::string& string) { std::cout << string << '\n'; }

	static void cursorHome() { std::cout << "\033[H" << std::flush; }
	static void cursorMove(Vector2i position) { std::cout << "\x1B[" << position.y << ";" << position.x << "H" << std::flush; }

	static char getChar() {
		if (_kbhit()) {
			return _getch();
		}
	}
};

// Fun stuff
std::vector<Vector3f> triangle = {
	Vector3f{-2, 0, 0}, // Left
	Vector3f{2, 0, 0}, // Right
	Vector3f{0, -2, 0}, // Top
};

std::vector<Vector3f> cube = {
	Vector3f{2, -2, 0}, // Front top right
	Vector3f{-2, -2, 0}, // Front top left
	Vector3f{-2, 2, 0}, // Front bottom left
	Vector3f{2, 2, 0}, // Front bottom right

	Vector3f{2, -2, -0.05}, // Back top right
	Vector3f{-2, -2, -0.05}, // Back top left
	Vector3f{-2, 2, -0.05}, // Back bottom left
	Vector3f{2, 2, -0.05}, // Back bottom right
};

Vector3f camera = {0, 0, -.5f};

float lerp(float a, float b, float t) { return a + (b - a) * t; }

void drawPoints(std::vector<Vector3f> points, Vector3f camera, bool erase = false) {
	std::vector<Vector2i> drawnPoints;
	
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleScreenBufferInfo(hStdout, &csbi)) {
		Vector2i screenSize = Vector2i{ csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };


		for (Vector3f point : points) {

			Vector3f pointVal = Vector3f{ point.x - camera.x, point.y - camera.y, point.z - camera.z };

			if (pointVal.z == 0) continue;

			Vector2f screenVal = Vector2f{ screenSize.x / 2 + pointVal.x / pointVal.z, screenSize.y / 2 + pointVal.y / pointVal.z };

			Console::cursorMove(Vector2i{ (int)screenVal.x, (int)screenVal.y });
			Console::print(erase ? " " : "o");

			drawnPoints.push_back(Vector2i{ (int)screenVal.x, (int)screenVal.y });
		}

		for (int i = 0; i < drawnPoints.size(); i++) {
			int index1 = i;
			int index2 = i + 1;

			if (index2 >= drawnPoints.size()) {
				index2 = 0;
			}

			for (float t = 0; t < 1; t += 0.1) {
				Console::cursorMove(Vector2i{ (int)lerp(drawnPoints[index1].x, drawnPoints[index2].x, t), (int)lerp(drawnPoints[index1].y, drawnPoints[index2].y, t) });
				Console::print(erase ? " " : ".");
			}
		}
	}
}

int main() {

	while (true) {
		drawPoints(cube, camera, true); // Erase
		

		switch (Console::getChar()) {
		case 'w': camera.z+= 0.01f; break;
		case 'a': camera.x-= 0.1f; break;
		case 's': camera.z-= 0.01f; break;
		case 'd': camera.x+= 0.1f; break;
		case ' ': camera.y-= 0.1f; break;
		case 'c': camera.y+= 0.1f; break;
		}

		drawPoints(cube, camera, false); // Draw
	}

	Console::cursorMove({ 1, 20 });
	return 0;
}