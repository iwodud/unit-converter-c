# Unit Converter

A simple unit converter written in C using the **GTK+ 3** library.  
The application lets the user choose a physical quantity type, select source and target units, enter a value, and perform the conversion with a button click or by pressing **Enter**.

---

## Features

The program supports conversions between units in the following categories:

### 1. Length
- millimeter, centimeter, meter, kilometer, inch, foot, yard, mile, nautical mile

### 2. Mass
- milligram, gram, kilogram, tonne, ounce, pound

### 3. Volume
- milliliter, centiliter, deciliter, liter, cubic meter, gallon

### 4. Area
- square meter, square kilometer, hectare, acre, square foot, square inch, square mile

### 5. Time
- second, minute, hour, day, week, month, year

### 6. Temperature
- Celsius degrees, Fahrenheit degrees, kelvin  
  (handled with a dedicated conversion function, not just a linear factor)

### 7. Energy
- joule, kilojoule, calorie, kilocalorie, watt hour, kilowatt hour

### 8. Power
- watt, kilowatt, megawatt, horsepower (mechanical), horsepower (metric)

### 9. Pressure
- pascal, kilopascal, bar, atmosphere, millimeter of mercury (mmHg), psi

### 10. Speed
- meters per second, kilometers per hour, miles per hour, knots

Additional features:

- input validation (error messages for invalid number, missing unit type or units),
- number formatting without unnecessary trailing zeros,
- correct singular/plural handling in unit labels (`1 second` vs `2 seconds`),
- **Swap units** button to quickly swap source and target units,
- pressing **Enter** in the input field triggers the conversion (same as clicking **Convert**).

---

## Requirements

- C compiler (e.g. `gcc`)
- **GTK+ 3** development files
- `pkg-config` for compiler flags

### Installing dependencies on Debian/Ubuntu/Linux Mint

```bash
sudo apt update
sudo apt install build-essential libgtk-3-dev pkg-config
