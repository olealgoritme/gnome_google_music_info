/* main.c
 *
 * Copyright 2018 Ole Algoritme
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib/gi18n.h>
#include <glib.h>
#include <gio/gio.h>
#include <string.h>

#include "google_music_info-config.h"
#include "google_music_info-window.h"

static GtkWindow *window;
static GFile *file;
static GFileMonitor* mon;
static GMainLoop *gMainLoop;
static GtkApplication *gApp;

static char* FILE_PATH = "/home/xuw/.config/Google Play Music Desktop Player/json_store/playback.json";

static GtkWindow *get_active_window() {
  return gtk_application_get_active_window(gApp);
}

static void set_title(gchar *title) {
  gtk_window_set_title(get_active_window(), title);
}


static char *decode(GFileMonitorEvent event) {
  
  char *fmt = g_malloc0 (1024);
  int caret = 0;

  #define dc(x)  \
    case G_FILE_MONITOR_EVENT_##x: \
      strcat(fmt, #x); \
      caret += strlen(#x); \
      break;
  
  switch (event) {
    dc(CHANGED);
    dc(CHANGES_DONE_HINT);
    dc(DELETED);
    dc(CREATED);
    dc(ATTRIBUTE_CHANGED);
    dc(PRE_UNMOUNT);
    dc(UNMOUNTED);
    dc(MOVED);
    dc(RENAMED);
    dc(MOVED_IN);
    dc(MOVED_OUT);
}                                     

  #undef dc

  return fmt;
}

static void on_file_changed (GFileMonitor *mon, GFile *file, GFileMonitorEvent event, gpointer udata) {
           
  char *msg = decode (event);
  #define fn(x) ((x) ? g_file_get_basename (x) : "--")
  
  g_printf ("%s: (code %d) | Path: %s \n File: %s | udata: %d\n",
              msg,
              event,
              g_file_get_parse_name(file),
              fn(file),
              udata);

    FILE *fp;
    gchar *line = NULL;
    gchar *artist = NULL, *title = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(FILE_PATH, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
  

    while ((read = getline(&line, &len, fp)) != -1) {
        
      if(strstr(line, "title") && title == NULL) {
            title = line;


        printf("Title: %s", title);
      } else if (strstr(line, "artist") && artist == NULL) {
            artist = line;
            printf("Artist: %s", artist);
      }                                                                          
    }

    //printf("Artist: %s\n Title: %s\n", artist, title);
    
    fclose(fp);
    g_free(line);
  
  #undef fn   
  g_free (msg);
}


static void on_gtk_activate (GtkApplication *gApp) {
	
	g_assert (GTK_IS_APPLICATION (gApp));
	
  window = get_active_window();
	
  if (window == NULL)
		window = g_object_new (GOOGLE_MUSIC_INFO_TYPE_WINDOW,
		                       "application", gApp,
		                       "default-width", 600,
		                       "default-height", 300,
		                       NULL);


  gtk_window_set_opacity (window, 0.3);
  gtk_window_present (window);


}


int main (int argc, char *argv[]) {
 
  int ret;
  
  file = g_file_new_for_path (FILE_PATH);
  g_assert (file != NULL);
  
  mon = g_file_monitor (file, G_FILE_MONITOR_SEND_MOVED, NULL, NULL);
  g_assert (mon != NULL);

  g_signal_connect (mon, "changed", G_CALLBACK (on_file_changed), NULL);
  gMainLoop = g_main_loop_new(NULL, TRUE);
  
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

  
  gApp = gtk_application_new ("org.gnome.Google_Music_Info", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (gApp, "activate", G_CALLBACK (on_gtk_activate), NULL);  
  
  ret = g_application_run (G_APPLICATION (gApp), argc, argv);
  
  g_object_unref (mon);
  g_object_unref (gMainLoop);
  
	return ret;
  
}
