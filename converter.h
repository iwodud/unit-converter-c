#ifndef CONVERTER_H
#define CONVERTER_H

typedef struct {
    const char* name;
    const char* singular_label; // e.g. "second", "square meter"
    const char* plural_label; // e.g. "seconds", "square meters"
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
    UNIT_TYPE_SPEED
} UnitType;

double convert(double value, double from_factor, double to_factor);
Unit* find_unit_by_name(UnitType type, const char* name);

// Provides access to unit lists for GUI population
Unit* get_units(UnitType type, int *count);

#endif
