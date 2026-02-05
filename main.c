#include <gtk/gtk.h>

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Button clicked!\n");
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *entry;
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Unit Converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter value...");
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("Convert");
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

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
