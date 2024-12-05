#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <random>

GtkWidget *entry_length, *entry_password, *window;
GtkCssProvider *css_provider;

std::unordered_map<std::string, std::string> lang_map;

// Function to generate a random password
std::string generate_password(int length) {
    const char charset[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    std::random_device rd;                  // random password generator
    std::mt19937 generator(rd());           
    std::uniform_int_distribution<size_t> distribution(0, sizeof(charset) - 2);

    std::string password;
    for (int i = 0; i < length; i++) {
        password += charset[distribution(generator)];
    }
    return password;
}

// Callback : Générer un mot de passe
void on_generate_password(GtkWidget *widget, gpointer data) {
    const char *len_text = gtk_entry_get_text(GTK_ENTRY(entry_length));
    try {
        int length = std::stoi(len_text);
        if (length <= 0) throw std::invalid_argument("Negative or zero length");
        std::string password = generate_password(length);
        gtk_entry_set_text(GTK_ENTRY(entry_password), password.c_str());
    } catch (std::invalid_argument&) {
        gtk_entry_set_text(GTK_ENTRY(entry_password), lang_map["error_invalid_length"].c_str());
    } catch (std::out_of_range&) {
        gtk_entry_set_text(GTK_ENTRY(entry_password), lang_map["error_length_out_of_range"].c_str());
    }
}

// Callback : copy to clipboard
void on_copy_to_clipboard(GtkWidget *widget, gpointer data) {
    const char *password = gtk_entry_get_text(GTK_ENTRY(entry_password));
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard, password, -1);
}

// Initialize dictionnary for the selected language
void set_language(const std::string& lang) {
    if (lang == "en") {
        lang_map = {
            {"title", "Open Password Generator"},
            {"enter_length", "Enter password length:"},
            {"placeholder_length", "Example - 12"},
            {"generate_button", "Generate Password"},
            {"placeholder_password", "Generated password will appear here!"},
            {"copy_button", "Copy to Clipboard"},
            {"error_invalid_length", "Error - Invalid length"},
            {"error_length_out_of_range", "Error - Length out of range"}
        };
    } else if (lang == "fr") {
        lang_map = {
            {"title", "Générateur de mot de passe"},
            {"enter_length", "Entrer la longueur du mot de passe :"},
            {"placeholder_length", "Exemple : 12"},
            {"generate_button", "Générer le mot de passe"},
            {"placeholder_password", "Le mot de passe généré apparaîtra ici"},
            {"copy_button", "Copier dans le presse-papier"},
            {"error_invalid_length", "Erreur - Taille invalide"},
            {"error_length_out_of_range", "Erreur - Taille trop grande"}
        };
    } else if (lang == "ru") {
        lang_map = {
            {"title", "Генератор паролей"},
            {"enter_length", "Введите длину пароля:"},
            {"placeholder_length", "Пример: 12"},
            {"generate_button", "Создать пароль"},
            {"placeholder_password", "Сгенерированный пароль появится здесь"},
            {"copy_button", "Скопировать в буфер обмена"},
            {"error_invalid_length", "Ошибка - Неверная длина"},
            {"error_length_out_of_range", "Ошибка - Длина вне допустимого диапазона"}
        };
    }
}

// Construct interface for the selected language
void build_ui(GtkWidget *widget, gpointer data) {
    const char *lang = static_cast<const char *>(data);
    set_language(lang);

    // Main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), lang_map["title"].c_str());
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 250);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 15);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Tittle
    GtkWidget *title = gtk_label_new(("<u><b><big>" + lang_map["title"] + "</big></b></u>").c_str());
    gtk_label_set_use_markup(GTK_LABEL(title), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 3);

    // Password length entry
    GtkWidget *label_length = gtk_label_new(lang_map["enter_length"].c_str());
    gtk_box_pack_start(GTK_BOX(vbox), label_length, FALSE, FALSE, 5);

    entry_length = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_length), lang_map["placeholder_length"].c_str());
    gtk_box_pack_start(GTK_BOX(vbox), entry_length, FALSE, FALSE, 5);

    // Generate password button
    GtkWidget *generate_button = gtk_button_new_with_label(lang_map["generate_button"].c_str());
    g_signal_connect(generate_button, "clicked", G_CALLBACK(on_generate_password), nullptr);
    gtk_box_pack_start(GTK_BOX(vbox), generate_button, FALSE, FALSE, 10);


    entry_password = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_password), lang_map["placeholder_password"].c_str());
    gtk_box_pack_start(GTK_BOX(vbox), entry_password, FALSE, FALSE, 5);

    // Click to copy to clipboard
    GtkWidget *copy_button = gtk_button_new_with_label(lang_map["copy_button"].c_str());
    g_signal_connect(copy_button, "clicked", G_CALLBACK(on_copy_to_clipboard), nullptr);
    gtk_box_pack_start(GTK_BOX(vbox), copy_button, FALSE, FALSE, 5);

    gtk_widget_show_all(window);
}

// Créer l'interface pour choisir la langue
void create_language_selection_ui() {
    GtkWidget *lang_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(lang_window), "Select Language");
    gtk_window_set_default_size(GTK_WINDOW(lang_window), 300, 200);
    gtk_window_set_resizable(GTK_WINDOW(lang_window), FALSE);
    g_signal_connect(lang_window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 15);
    gtk_container_add(GTK_CONTAINER(lang_window), vbox);

    GtkWidget *label = gtk_label_new("Choose your language:");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 10);

    GtkWidget *button_en = gtk_button_new_with_label("English");
    g_signal_connect(button_en, "clicked", G_CALLBACK(build_ui), (gpointer)"en");
    gtk_box_pack_start(GTK_BOX(vbox), button_en, FALSE, FALSE, 5);

    GtkWidget *button_fr = gtk_button_new_with_label("Français");
    g_signal_connect(button_fr, "clicked", G_CALLBACK(build_ui), (gpointer)"fr");
    gtk_box_pack_start(GTK_BOX(vbox), button_fr, FALSE, FALSE, 5);

    GtkWidget *button_ru = gtk_button_new_with_label("Русский");
    g_signal_connect(button_ru, "clicked", G_CALLBACK(build_ui), (gpointer)"ru");
    gtk_box_pack_start(GTK_BOX(vbox), button_ru, FALSE, FALSE, 5);

    gtk_widget_show_all(lang_window);
}

// Fonction principale
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    create_language_selection_ui();
    gtk_main();
    return 0;
}
