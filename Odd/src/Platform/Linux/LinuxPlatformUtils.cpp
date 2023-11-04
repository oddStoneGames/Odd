#include "oddpch.h"
#include "Odd/Utils/PlatformUtils.h"
#include "Odd/Core/Application.h"

#include <GLFW/glfw3.h>
#include <gtk/gtk.h>

namespace Odd
{
    std::string FileDialogs::OpenFile(const char* filter)
    {
        std::string filename;

        // Initialize GTK
        if (!gtk_init_check(0, NULL))
            return filename;

        GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                       NULL,
                                                       GTK_FILE_CHOOSER_ACTION_OPEN,
                                                       "_Cancel", GTK_RESPONSE_CANCEL,
                                                       "_Open", GTK_RESPONSE_ACCEPT,
                                                       NULL);

        // File filter setup (this is a simple example, you may need to adapt for your needs)
        GtkFileFilter *fileFilter = gtk_file_filter_new();
        gtk_file_filter_set_name(fileFilter, filter);
        gtk_file_filter_add_pattern(fileFilter, filter);
        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), fileFilter);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
        {
            char *selectedFilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            filename = selectedFilename;
            g_free(selectedFilename);
        }

        gtk_widget_destroy(dialog);
        while (gtk_events_pending()) 
            gtk_main_iteration();

        return filename;
    }

    std::string FileDialogs::SaveFile(const char* filter)
    {
        std::string filename;

        if (!gtk_init_check(0, NULL))
            return filename;

        GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File",
                                                       NULL,
                                                       GTK_FILE_CHOOSER_ACTION_SAVE,
                                                       "_Cancel", GTK_RESPONSE_CANCEL,
                                                       "_Save", GTK_RESPONSE_ACCEPT,
                                                       NULL);

        gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
        
        GtkFileFilter *fileFilter = gtk_file_filter_new();
        gtk_file_filter_set_name(fileFilter, filter);
        gtk_file_filter_add_pattern(fileFilter, filter);
        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), fileFilter);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
        {
            char *selectedFilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            filename = selectedFilename;
            g_free(selectedFilename);
        }

        gtk_widget_destroy(dialog);
        while (gtk_events_pending()) 
            gtk_main_iteration();

        return filename;
    }
}