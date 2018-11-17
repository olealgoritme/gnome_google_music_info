/* google_music_info-window.c
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

#include "google_music_info-config.h"
#include "google_music_info-window.h"

struct _Google_music_infoWindow {
  GtkApplicationWindow  parent_instance;

  /* Template widgets */
  GtkHeaderBar        *header_bar;
  GtkLabel            *label;
};

G_DEFINE_TYPE (Google_music_infoWindow, google_music_info_window, GTK_TYPE_APPLICATION_WINDOW)


static void google_music_info_window_class_init (Google_music_infoWindowClass *klass) {
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/Google_Music_Info/google_music_info-window.ui");
  gtk_widget_class_bind_template_child (widget_class, Google_music_infoWindow, header_bar);
  gtk_widget_class_bind_template_child (widget_class, Google_music_infoWindow, label);
}

static void google_music_info_window_init (Google_music_infoWindow *self) {
  gtk_widget_init_template (GTK_WIDGET (self));
}
 
