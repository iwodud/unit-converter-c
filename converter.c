#include <string.h>
#include "converter.h"

double convert(double value, double from_factor, double to_factor) {
    double base = value * from_factor;
    return base / to_factor;
}

double convert_temperature(double value, const char* from, const char* to) {
    double c;

    if (strcmp(from, "celsius") == 0)
        c = value;
    else if (strcmp(from, "fahrenheit") == 0)
        c = (value - 32.0) * 5.0 / 9.0;
    else if (strcmp(from, "kelvin") == 0)
        c = value - 273.15;
    else
        return 0;

    if (strcmp(to, "celsius") == 0)
        return c;
    else if (strcmp(to, "fahrenheit") == 0)
        return c * 9.0 / 5.0 + 32.0;
    else if (strcmp(to, "kelvin") == 0)
        return c + 273.15;

    return 0;
}

static Unit length_units[] = {
    {"millimeter","millimeter","millimeters",0.001},
    {"centimeter","centimeter","centimeters",0.01},
    {"meter","meter","meters",1.0},
    {"kilometer","kilometer","kilometers",1000.0},
    {"inch","inch","inches",0.0254},
    {"foot","foot","feet",0.3048},
    {"yard","yard","yards",0.9144},
    {"mile","mile","miles",1609.34},
    {"nautical mile","nautical mile","nautical miles",1852.0}
};

static Unit mass_units[] = {
    {"milligram","milligram","milligrams",0.001},
    {"gram","gram","grams",1.0},
    {"kilogram","kilogram","kilograms",1000.0},
    {"tonne","tonne","tonnes",1000000.0},
    {"ounce","ounce","ounces",28.3495},
    {"pound","pound","pounds",453.592}
};

static Unit volume_units[] = {
    {"milliliter","milliliter","milliliters",0.001},
    {"centiliter","centiliter","centiliters",0.01},
    {"deciliter","deciliter","deciliters",0.1},
    {"liter","liter","liters",1.0},
    {"cubic meter","cubic meter","cubic meters",1000.0},
    {"gallon","gallon","gallons",3.78541}
};

static Unit area_units[] = {
    {"square meter","square meter","square meters",1.0},
    {"square kilometer","square kilometer","square kilometers",1000000.0},
    {"hectare","hectare","hectares",10000.0},
    {"acre","acre","acres",4046.8564224},
    {"square foot","square foot","square feet",0.09290304},
    {"square inch","square inch","square inches",0.00064516},
    {"square mile","square mile","square miles",2589988.110336}
};

static Unit time_units[] = {
    {"second","second","seconds",1.0},
    {"minute","minute","minutes",60.0},
    {"hour","hour","hours",3600.0},
    {"day","day","days",86400.0},
    {"week","week","weeks",604800.0},
    {"month","month","months",2629746.0},
    {"year","year","years",31557600.0}
};

static Unit temperature_units[] = {
    {"celsius","degree Celsius","degrees Celsius",0},
    {"fahrenheit","degree Fahrenheit","degrees Fahrenheit",0},
    {"kelvin","kelvin","kelvins",0}
};

static Unit energy_units[] = {
    {"joule","joule","joules",1.0},
    {"kilojoule","kilojoule","kilojoules",1000.0},
    {"calorie","calorie","calories",4.184},
    {"kilocalorie","kilocalorie","kilocalories",4184.0},
    {"watt hour","watt hour","watt hours",3600.0},
    {"kilowatt hour","kilowatt hour","kilowatt hours",3600000.0},
};

static Unit power_units[] = {
    {"watt","watt","watts",1.0},
    {"kilowatt","kilowatt","kilowatts",1000.0},
    {"megawatt","megawatt","megawatts",1000000.0},
    {"horsepower mechanical","horsepower (mechanical)","horsepower (mechanical)",745.69987158227022},
    {"horsepower metric","horsepower (metric)","horsepower (metric)",735.49875}
};

static Unit pressure_units[] = {
    {"pascal","pascal","pascals",1.0},
    {"kilopascal","kilopascal","kilopascals",1000.0},
    {"bar","bar","bars",100000.0},
    {"atmosphere","atmosphere","atmospheres",101325.0},
    {"mmHg","millimeter of mercury","millimeters of mercury",133.322387415},
    {"psi","psi","psi",6894.757293168}
};

static Unit speed_units[] = {
    {"meter per second","meter per second","meters per second",1.0},
    {"kilometer per hour","kilometer per hour","kilometers per hour",0.2777777777778},
    {"mile per hour","mile per hour","miles per hour",0.44704},
    {"knot","knot","knots",0.514444444444}
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
        case UNIT_TYPE_TEMPERATURE:
            *count = sizeof(temperature_units) / sizeof(Unit);
            return temperature_units;
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
