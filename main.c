#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "converter.h"


typedef struct {
    GtkWidget *entry;
    GtkWidget *result_label;
    GtkWidget *type_combo;
    GtkWidget *from_combo;
    GtkWidget *to_combo;
} AppWidgets;


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

    UnitType type;

    if (strcmp(type_text, "length") == 0) {
        type = UNIT_TYPE_LENGTH;
    } else if (strcmp(type_text, "mass") == 0) {
        type = UNIT_TYPE_MASS;
    } else if (strcmp(type_text, "volume") == 0) {
        type = UNIT_TYPE_VOLUME;
    } else {
        return; // "Select unit type" selected
    }

    populate_units(GTK_COMBO_BOX_TEXT(widgets->from_combo),
                   type,
                   "Source unit");

    populate_units(GTK_COMBO_BOX_TEXT(widgets->to_combo),
                   type,
                   "Target unit");
}


static void on_button_clicked(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;

    const char *text = gtk_entry_get_text(GTK_ENTRY(widgets->entry));
    char *endptr;
    double value = strtod(text, &endptr);

    if (text == endptr || *endptr != '\0') {
        gtk_label_set_text(GTK_LABEL(widgets->result_label), "Invalid number. Please enter a valid value.");
        return;
    }

    const char *type_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->type_combo));
    const char *from_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->from_combo));
    const char *to_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->to_combo));

    UnitType type = UNIT_TYPE_LENGTH;
    if (strcmp(type_text, "mass") == 0) {
        type = UNIT_TYPE_MASS;
    } else if (strcmp(type_text, "volume") == 0) {
        type = UNIT_TYPE_VOLUME;
    }

    Unit *from = find_unit_by_name(type, from_name);
    Unit *to = find_unit_by_name(type, to_name);

    if (from == NULL || to == NULL) {
        gtk_label_set_text(GTK_LABEL(widgets->result_label), "Internal error: units not found.");
        return;
    }

    double result = convert(value, from->to_base, to->to_base);

    char result_str[32];
    if (result < 0.01 && result > 0.0) {
        snprintf(result_str, sizeof(result_str), "%.9f", result);
    } else {
        snprintf(result_str, sizeof(result_str), "%.2f", result);
    }

    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%.2f %s = %s %s", value, from->name, result_str, to->name);
    gtk_label_set_text(GTK_LABEL(widgets->result_label), buffer);
}


static void on_swap_clicked(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;

    int from_index = gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->from_combo));
    int to_index = gtk_combo_box_get_active(GTK_COMBO_BOX(widgets->to_combo));

    if (from_index > 0 && to_index > 0) {  // tylko jeśli obie jednostki są wybrane
        gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->from_combo), to_index);
        gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->to_combo), from_index);
    }
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
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 350);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Unit type combo box
    type_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "Select unit type");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "length");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "mass");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "volume");
    gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo), 0);
    gtk_box_pack_start(GTK_BOX(box), type_combo, FALSE, FALSE, 0);

    // Source unit combo box
    from_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(from_combo), "Source unit");
    gtk_combo_box_set_active(GTK_COMBO_BOX(from_combo), 0);
    gtk_box_pack_start(GTK_BOX(box), from_combo, FALSE, FALSE, 0);

    // Target unit combo box
    to_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(to_combo), "Target unit");
    gtk_combo_box_set_active(GTK_COMBO_BOX(to_combo), 0);
    gtk_box_pack_start(GTK_BOX(box), to_combo, FALSE, FALSE, 0);

    // Swap button
    swap_button = gtk_button_new_with_label("Swap units");
    gtk_box_pack_start(GTK_BOX(box), swap_button, FALSE, FALSE, 0);

    // Value input entry
    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter value...");
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    // Convert button
    button = gtk_button_new_with_label("Convert");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    // Result label
    result_label = gtk_label_new("Result will appear here.");
    gtk_box_pack_start(GTK_BOX(box), result_label, FALSE, FALSE, 0);

    // Assign widgets to AppWidgets struct
    widgets->entry = entry;
    widgets->result_label = result_label;
    widgets->type_combo = type_combo;
    widgets->from_combo = from_combo;
    widgets->to_combo = to_combo;

    // Signal connections
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), widgets);
    g_signal_connect(type_combo, "changed", G_CALLBACK(on_type_changed), widgets);
    g_signal_connect(swap_button, "clicked", G_CALLBACK(on_swap_clicked), widgets);

    populate_units(GTK_COMBO_BOX_TEXT(from_combo), UNIT_TYPE_LENGTH, "Source unit");
    populate_units(GTK_COMBO_BOX_TEXT(to_combo), UNIT_TYPE_LENGTH, "Target unit");

    gtk_widget_show_all(window);
}


int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.unitconverter", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
