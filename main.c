#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    char from_unit[50], to_unit[50];

    if (!read_double_input("Enter value to convert: ", &input_value)) {
        printf("Invalid input. Please enter a number.\n");
        return 1;
    }
    
    printf("From unit (e.g. kilometer): ");
    scanf("%s", from_unit);

    printf("To unit (e.g. mile): ");
    scanf("%s", to_unit);

    UnitType type = UNIT_TYPE_LENGTH; // for now, only length

    Unit* from = find_unit_by_name(type, from_unit);
    Unit* to = find_unit_by_name(type, to_unit);

    if (from == NULL || to == NULL) {
        printf("Unknown unit.\n");
        return 1;
    }

    double result = convert(input_value, from->to_base, to->to_base);
    double rounded = round_to_two_decimals(result);

    printf("%.2lf %s = %.2lf %s\n", input_value, from->name, rounded, to->name);

    return 0;
}
