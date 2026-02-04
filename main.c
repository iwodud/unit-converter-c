#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "converter.h"

bool read_double_input(const char* prompt, double* out_value) {
    char buffer[100];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return false;

    char* endptr;
    *out_value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') return false;

    return true;
}

int main() {
    double input_value;
    double from_factor = 1000.0; // kilometers to meters
    double to_factor = 1609.34; // miles to meters

    printf("UNIT CONVERTER (console version)\n");

    if (!read_double_input("Enter value in kilometers: ", &input_value)) {
        printf("Invalid input. Please enter a valid number.\n");
        return 1;
    }

    double result = convert(input_value, from_factor, to_factor);
    double rounded = round_to_two_decimals(result);

    printf("%.2lf kilometers = %.2lf miles\n", input_value, rounded);

    return 0;
}
