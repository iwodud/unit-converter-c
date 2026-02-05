#ifndef CONVERTER_H
#define CONVERTER_H

typedef struct {
    const char* name;
    const char* singular_label;
    const char* plural_label;
    double to_base;
} Unit;

typedef enum {
    UNIT_TYPE_LENGTH,
    UNIT_TYPE_MASS,
    UNIT_TYPE_VOLUME,
    UNIT_TYPE_AREA,
    UNIT_TYPE_TIME,
    UNIT_TYPE_ENERGY,
    UNIT_TYPE_POWER,
    UNIT_TYPE_PRESSURE,
    UNIT_TYPE_SPEED,
    UNIT_TYPE_TEMPERATURE
} UnitType;

double convert(double value, double from_factor, double to_factor);
double convert_temperature(double value, const char* from, const char* to);

Unit* get_units(UnitType type, int *count);
Unit* find_unit_by_name(UnitType type, const char* name);

#endif
