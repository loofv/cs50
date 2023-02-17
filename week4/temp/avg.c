#include <stdio.h>
#include <math.h>

int main(void) {
	float avg = (3+8+8+4) /4.0;
	int rounded = round(avg);
	printf("avg, expected: 6: %i\n", rounded);
}
