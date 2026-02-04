#include <string.h> 
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

static Unit length_units[] = {
    {"millimeter", 0.001},
    {"centimeter", 0.01},
    {"meter", 1.0},
    {"kilometer", 1000.0},
    {"inch", 0.0254},
    {"foot", 0.3048},
    {"yard", 0.9144},
    {"mile", 1609.34},
    {"nautical_mile", 1852.0}
};


static Unit mass_units[] = {
    {"milligram", 0.001},
    {"gram", 1.0},
    {"kilogram", 1000.0},
    {"tonne", 1000000.0},
    {"ounce", 28.3495},
    {"pound", 453.592},
    {"stone", 6350.29}
};


static Unit volume_units[] = {
    {"milliliter", 0.001},
    {"centiliter", 0.01},
    {"deciliter", 0.1},
    {"liter", 1.0},
    {"cubic_meter", 1000.0},
    {"gallon", 3.78541}
};


Unit* find_unit_by_name(UnitType type, const char* name) {
    Unit* list = NULL;
    int count = 0;

    switch (type) {
        case UNIT_TYPE_LENGTH:
            list = length_units;
            count = sizeof(length_units) / sizeof(Unit);
            break;
        case UNIT_TYPE_MASS:
            list = mass_units;
            count = sizeof(mass_units) / sizeof(Unit);
            break;
        case UNIT_TYPE_VOLUME:
            list = volume_units;
            count = sizeof(volume_units) / sizeof(Unit);
            break;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].name, name) == 0) {
            return &list[i];
        }
    }

    return NULL; // not found
}
