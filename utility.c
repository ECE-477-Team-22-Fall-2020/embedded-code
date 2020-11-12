#include "utility.h"

// doesn't actually wait one ms lol
void delay(int ms) {
	int i;
	int j = 0;
	while (j < ms) {
		i = 10000;
		while (i > 0) i--;
		j++;
	}
}
