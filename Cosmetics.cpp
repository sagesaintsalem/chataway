#include "Cosmetics.h"

void setColour(int textColour) {
	cout << "\033[" << textColour << "m";
}

void resetColour() {
	cout << "\033[0m";
}

void setconsolecolor(int textColor, int bgColor) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor + (bgColor * 16)));
}