#ifndef CONVERTER_H
#define CONVERTER_H

typedef struct {
    const char* name;
    double to_base;
} Unit;

typedef enum {
    UNIT_TYPE_LENGTH,
    UNIT_TYPE_MASS,
    UNIT_TYPE_VOLUME
} UnitType;

double convert(double value, double from_factor, double to_factor);
double round_to_two_decimals(double number);
Unit* find_unit_by_name(UnitType type, const char* name);

Unit* get_units(UnitType type, int *count);

#endif
