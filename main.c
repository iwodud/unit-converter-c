#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "converter.h"

typedef struct {
    GtkWidget *entry;
    GtkWidget *result_label;
} AppWidgets;

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;

    const char *text = gtk_entry_get_text(GTK_ENTRY(widgets->entry));
    char *endptr;
    double value = strtod(text, &endptr);

    if (text == endptr || *endptr != '\0') {
        gtk_label_set_text(GTK_LABEL(widgets->result_label), "Invalid number. Please enter a valid value.");
        return;
    }

    UnitType type = UNIT_TYPE_LENGTH;

    Unit *from = find_unit_by_name(type, "kilometer");
    Unit *to = find_unit_by_name(type, "mile");

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
    GtkWidget *button;
    GtkWidget *result_label;

    AppWidgets *widgets = g_new(AppWidgets, 1);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Unit Converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter value in kilometers...");
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("Convert to miles");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    result_label = gtk_label_new("Result will appear here.");
    gtk_box_pack_start(GTK_BOX(box), result_label, FALSE, FALSE, 0);

    widgets->entry = entry;
    widgets->result_label = result_label;

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), widgets);

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
