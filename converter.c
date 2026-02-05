#include <string.h>
#include "converter.h"

double convert(double value, double from_factor, double to_factor) {
    double base = value * from_factor;
    return base / to_factor;
}

/* -------------------- UNITS -------------------- */

// Base: meter
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

// Base: gram
static Unit mass_units[] = {
    {"milligram", 0.001},
    {"gram", 1.0},
    {"kilogram", 1000.0},
    {"tonne", 1000000.0},
    {"ounce", 28.3495},
    {"pound", 453.592}
};

// Base: liter
static Unit volume_units[] = {
    {"milliliter", 0.001},
    {"centiliter", 0.01},
    {"deciliter", 0.1},
    {"liter", 1.0},
    {"cubic_meter", 1000.0},
    {"gallon", 3.78541}
};

// Base: square_meter
static Unit area_units[] = {
    {"square_meter", 1.0},
    {"square_kilometer", 1000000.0},
    {"hectare", 10000.0},
    {"acre", 4046.8564224},
    {"square_foot", 0.09290304},
    {"square_inch", 0.00064516},
    {"square_mile", 2589988.110336}
};

// Base: second
static Unit time_units[] = {
    {"second", 1.0},
    {"minute", 60.0},
    {"hour", 3600.0},
    {"day", 86400.0},
    {"week", 604800.0},
    {"month", 2629746.0},     // approx 30.44 days
    {"year", 31557600.0}      // approx 365.25 days
};

// Base: joule
static Unit energy_units[] = {
    {"joule", 1.0},
    {"kilojoule", 1000.0},
    {"calorie", 4.184},           // thermochemical calorie
    {"kilocalorie", 4184.0},
    {"watt_hour", 3600.0},
    {"kilowatt_hour", 3600000.0},
    {"electronvolt", 1.602176634e-19}
};

// Base: watt
static Unit power_units[] = {
    {"watt", 1.0},
    {"kilowatt", 1000.0},
    {"megawatt", 1000000.0},
    {"horsepower_mechanical", 745.69987158227022},
    {"horsepower_metric", 735.49875}
};

// Base: pascal
static Unit pressure_units[] = {
    {"pascal", 1.0},
    {"kilopascal", 1000.0},
    {"bar", 100000.0},
    {"atmosphere", 101325.0},
    {"mmHg", 133.322387415},
    {"psi", 6894.757293168}
};

// Base: meter_per_second
static Unit speed_units[] = {
    {"meter_per_second", 1.0},
    {"kilometer_per_hour", 0.2777777777778},
    {"mile_per_hour", 0.44704},
    {"knot", 0.514444444444}
};

Unit* get_units(UnitType type, int *count) {
    switch (type) {
        case UNIT_TYPE_LENGTH:
            *count = (int)(sizeof(length_units) / sizeof(Unit));
            return length_units;
        case UNIT_TYPE_MASS:
            *count = (int)(sizeof(mass_units) / sizeof(Unit));
            return mass_units;
        case UNIT_TYPE_VOLUME:
            *count = (int)(sizeof(volume_units) / sizeof(Unit));
            return volume_units;
        case UNIT_TYPE_AREA:
            *count = (int)(sizeof(area_units) / sizeof(Unit));
            return area_units;
        case UNIT_TYPE_TIME:
            *count = (int)(sizeof(time_units) / sizeof(Unit));
            return time_units;
        case UNIT_TYPE_ENERGY:
            *count = (int)(sizeof(energy_units) / sizeof(Unit));
            return energy_units;
        case UNIT_TYPE_POWER:
            *count = (int)(sizeof(power_units) / sizeof(Unit));
            return power_units;
        case UNIT_TYPE_PRESSURE:
            *count = (int)(sizeof(pressure_units) / sizeof(Unit));
            return pressure_units;
        case UNIT_TYPE_SPEED:
            *count = (int)(sizeof(speed_units) / sizeof(Unit));
            return speed_units;
        default:
            *count = 0;
            return NULL;
    }
}

Unit* find_unit_by_name(UnitType type, const char* name) {
    int count = 0;
    Unit* list = get_units(type, &count);
    if (!list || !name) return NULL;

    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].name, name) == 0) {
            return &list[i];
        }
    }
    return NULL;
}
