#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include "converter.h"

typedef struct {
    GtkWidget *entry;
    GtkWidget *result_label;
    GtkWidget *type_combo;
    GtkWidget *from_combo;
    GtkWidget *to_combo;
} AppWidgets;

static void trim_trailing_zeros(char *str) {
    if (!str || str[0] == '\0') return;

    char *end = str + strlen(str) - 1;

    // remove trailing zeros
    while (end > str && *end == '0') {
        *end-- = '\0';
    }

    // remove trailing decimal separator (dot or comma)
    if (end > str && (*end == '.' || *end == ',')) {
        *end = '\0';
    }

    // normalize "-0" or "-0," etc.
    if (strcmp(str, "-0") == 0) strcpy(str, "0");
}


static void format_number(char *out, size_t out_size, double x) {
    double ax = fabs(x);

    if (ax == 0.0) {
        snprintf(out, out_size, "0");
        return;
    }

    if (ax < 1e-6) {
        snprintf(out, out_size, "%.18f", x);
    } else if (ax < 0.01) {
        snprintf(out, out_size, "%.12f", x);
    } else if (ax < 1.0) {
        snprintf(out, out_size, "%.6f", x);
    } else {
        snprintf(out, out_size, "%.6f", x);
    }

    trim_trailing_zeros(out);
}

static UnitType unit_type_from_string(const char *s, gboolean *ok) {
    if (ok) *ok = TRUE;

    if (strcmp(s, "length") == 0) return UNIT_TYPE_LENGTH;
    if (strcmp(s, "mass") == 0) return UNIT_TYPE_MASS;
    if (strcmp(s, "volume") == 0) return UNIT_TYPE_VOLUME;
    if (strcmp(s, "area") == 0) return UNIT_TYPE_AREA;
    if (strcmp(s, "time") == 0) return UNIT_TYPE_TIME;
    if (strcmp(s, "temperature") == 0) return UNIT_TYPE_TEMPERATURE;
    if (strcmp(s, "energy") == 0) return UNIT_TYPE_ENERGY;
    if (strcmp(s, "power") == 0) return UNIT_TYPE_POWER;
    if (strcmp(s, "pressure") == 0) return UNIT_TYPE_PRESSURE;
    if (strcmp(s, "speed") == 0) return UNIT_TYPE_SPEED;

    if (ok) *ok = FALSE;
    return UNIT_TYPE_LENGTH;
}

static void populate_units(GtkComboBoxText *combo, UnitType type, const char *placeholder_text) {
    gtk_combo_box_text_remove_all(combo);
    gtk_combo_box_text_append_text(combo, placeholder_text);

    int count = 0;
    Unit *unit_list = get_units(type, &count);

    for (int i = 0; i < count; i++) {
        gtk_combo_box_text_append_text(combo, unit_list[i].name);
    }

    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
}

static void on_type_changed(GtkComboBoxText *combo, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;
    const char *type_text = gtk_combo_box_text_get_active_text(combo);

    gboolean ok = FALSE;
    UnitType type = unit_type_from_string(type_text, &ok);
    if (!ok) return; // "Select unit type"

    populate_units(GTK_COMBO_BOX_TEXT(widgets->from_combo), type, "Source unit");
    populate_units(GTK_COMBO_BOX_TEXT(widgets->to_combo), type, "Target unit");
}

static void on_swap_clicked(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;

    int from_index = gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->from_combo));
    int to_index = gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->to_combo));

    if (from_index > 0 && to_index > 0) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->from_combo), to_index);
        gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->to_combo), from_index);
    }
}

static const char* unit_label_for_value(const Unit* unit, double value) {
    if (!unit) return "";

    double abs_val = fabs(value);
    if (fabs(abs_val - 1.0) < 1e-9) {
        return unit->singular_label;
    }
    return unit->plural_label;
}

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;

    const char *type_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->type_combo));
    gboolean type_ok = FALSE;
    UnitType type = unit_type_from_string(type_text, &type_ok);

    if (!type_ok) {
        gtk_label_set_text(GTK_LABEL(widgets->result_label), "Please select a unit type.");
        return;
    }

    const char *from_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->from_combo));
    const char *to_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->to_combo));

    if (!from_name || !to_name || strcmp(from_name, "Source unit") == 0 || strcmp(to_name, "Target unit") == 0) {
        gtk_label_set_text(GTK_LABEL(widgets->result_label), "Please select both source and target units.");
        return;
    }

    const char *text = gtk_entry_get_text(GTK_ENTRY(widgets->entry));
    char *endptr;
    double value = strtod(text, &endptr);

    if (text == endptr || *endptr != '\0') {
        gtk_label_set_text(GTK_LABEL(widgets->result_label), "Invalid number. Please enter a valid value.");
        return;
    }

    Unit *from = find_unit_by_name(type, from_name);
    Unit *to = find_unit_by_name(type, to_name);

    if (!from || !to) {
        gtk_label_set_text(GTK_LABEL(widgets->result_label), "Internal error: units not found.");
        return;
    }

    double result;

    if (type == UNIT_TYPE_TEMPERATURE)
        result = convert_temperature(value, from->name, to->name);
    else
        result = convert(value, from->to_base, to->to_base);

    char value_str[64];
    char result_str[64];
    format_number(value_str, sizeof(value_str), value);
    format_number(result_str, sizeof(result_str), result);

    const char* from_label = unit_label_for_value(from, value);
    const char* to_label   = unit_label_for_value(to, result);

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s %s = %s %s",
            value_str, from_label, result_str, to_label);
    gtk_label_set_text(GTK_LABEL(widgets->result_label), buffer);
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *entry;
    GtkWidget *type_combo;
    GtkWidget *from_combo;
    GtkWidget *to_combo;
    GtkWidget *swap_button;
    GtkWidget *button;
    GtkWidget *result_label;

    AppWidgets *widgets = g_new(AppWidgets, 1);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Unit Converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 420, 380);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Type selector
    type_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "Select unit type");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "length");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "mass");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "volume");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "area");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "time");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "temperature");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "energy");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "power");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "pressure");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "speed");
    gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo), 0);
    gtk_box_pack_start(GTK_BOX(box), type_combo, FALSE, FALSE, 0);

    // Source unit
    from_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(from_combo), "Source unit");
    gtk_combo_box_set_active(GTK_COMBO_BOX(from_combo), 0);
    gtk_box_pack_start(GTK_BOX(box), from_combo, FALSE, FALSE, 0);

    // Target unit
    to_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(to_combo), "Target unit");
    gtk_combo_box_set_active(GTK_COMBO_BOX(to_combo), 0);
    gtk_box_pack_start(GTK_BOX(box), to_combo, FALSE, FALSE, 0);

    // Swap
    swap_button = gtk_button_new_with_label("Swap units");
    gtk_box_pack_start(GTK_BOX(box), swap_button, FALSE, FALSE, 0);

    // Value input
    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter value...");
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    // Convert
    button = gtk_button_new_with_label("Convert");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    // Result
    result_label = gtk_label_new("Result will appear here.");
    gtk_box_pack_start(GTK_BOX(box), result_label, FALSE, FALSE, 0);

    widgets->entry = entry;
    widgets->result_label = result_label;
    widgets->type_combo = type_combo;
    widgets->from_combo = from_combo;
    widgets->to_combo = to_combo;

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), widgets);
    g_signal_connect(type_combo, "changed", G_CALLBACK(on_type_changed), widgets);
    g_signal_connect(swap_button, "clicked", G_CALLBACK(on_swap_clicked), widgets);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    // Ensure dot decimal separator regardless of system locale
    setlocale(LC_NUMERIC, "C");

    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.unitconverter", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
