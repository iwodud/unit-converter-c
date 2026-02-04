#include "converter.h"
#include <math.h>

double convert(double value, double from_factor, double to_factor) {
    double base = value * from_factor;
    double result = base / to_factor;
    return result;
}

double round_to_two_decimals(double number) {
    return round(number * 100.0) / 100.0;
}
