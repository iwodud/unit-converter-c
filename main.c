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

static void populate_units(GtkComboBoxText *combo, UnitType type) {
    gtk_combo_box_text_remove_all(combo);

    int count = 0;
    Unit *unit_list = get_units(type, &count);

    for (int i = 0; i < count; i++) {
        gtk_combo_box_text_append_text(combo, unit_list[i].name);
    }

    if (count > 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
}


static void on_type_changed(GtkComboBoxText *combo, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;
    const char *type_text = gtk_combo_box_text_get_active_text(combo);

    UnitType type = UNIT_TYPE_LENGTH;
    if (strcmp(type_text, "mass") == 0) {
        type = UNIT_TYPE_MASS;
    } else if (strcmp(type_text, "volume") == 0) {
        type = UNIT_TYPE_VOLUME;
    }

    populate_units(GTK_COMBO_BOX_TEXT(widgets->from_combo), type);
    populate_units(GTK_COMBO_BOX_TEXT(widgets->to_combo), type);
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
    double rounded = round_to_two_decimals(result);

    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%.2f %s = %.2f %s", value, from->name, rounded, to->name);
    gtk_label_set_text(GTK_LABEL(widgets->result_label), buffer);
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *entry;
    GtkWidget *type_combo;
    GtkWidget *from_combo;
    GtkWidget *to_combo;
    GtkWidget *button;
    GtkWidget *result_label;

    AppWidgets *widgets = g_new(AppWidgets, 1);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Unit Converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    type_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "length");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "mass");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "volume");
    gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo), 0);
    gtk_box_pack_start(GTK_BOX(box), type_combo, FALSE, FALSE, 0);

    from_combo = gtk_combo_box_text_new();
    to_combo = gtk_combo_box_text_new();
    gtk_box_pack_start(GTK_BOX(box), from_combo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), to_combo, FALSE, FALSE, 0);

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter value...");
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("Convert");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    result_label = gtk_label_new("Result will appear here.");
    gtk_box_pack_start(GTK_BOX(box), result_label, FALSE, FALSE, 0);

    widgets->entry = entry;
    widgets->result_label = result_label;
    widgets->type_combo = type_combo;
    widgets->from_combo = from_combo;
    widgets->to_combo = to_combo;

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), widgets);
    g_signal_connect(type_combo, "changed", G_CALLBACK(on_type_changed), widgets);

    populate_units(GTK_COMBO_BOX_TEXT(from_combo), UNIT_TYPE_LENGTH);
    populate_units(GTK_COMBO_BOX_TEXT(to_combo), UNIT_TYPE_LENGTH);

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
