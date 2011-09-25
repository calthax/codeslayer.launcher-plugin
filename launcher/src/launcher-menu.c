/*
 * Copyright (C) 2010 - Jeff Johnston
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.remove_group_item
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <gdk/gdkkeysyms.h>
#include <codeslayer/codeslayer.h>
#include "launcher-menu.h"

static void launcher_menu_class_init  (LauncherMenuClass *klass);
static void launcher_menu_init        (LauncherMenu      *menu);
static void launcher_menu_finalize    (LauncherMenu      *menu);

static void run_action              (LauncherMenu      *menu);
                                        
enum
{
  RUN,
  LAST_SIGNAL
};

static guint launcher_menu_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (LauncherMenu, launcher_menu, GTK_TYPE_MENU_ITEM)

static void
launcher_menu_class_init (LauncherMenuClass *klass)
{
  launcher_menu_signals[RUN] =
    g_signal_new ("run", 
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                  G_STRUCT_OFFSET (LauncherMenuClass, run),
                  NULL, NULL, 
                  g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

  G_OBJECT_CLASS (klass)->finalize = (GObjectFinalizeFunc) launcher_menu_finalize;
}

static void
launcher_menu_init (LauncherMenu *menu)
{
  gtk_menu_item_set_label (GTK_MENU_ITEM (menu), "Run Program");
}

static void
launcher_menu_finalize (LauncherMenu *menu)
{
  G_OBJECT_CLASS (launcher_menu_parent_class)->finalize (G_OBJECT (menu));
}

GtkWidget*
launcher_menu_new (GtkAccelGroup *accel_group)
{
  GtkWidget *menu;

  menu = g_object_new (launcher_menu_get_type (), NULL);
  
  gtk_widget_add_accelerator (menu, "activate", 
                              accel_group, GDK_F8, 0, GTK_ACCEL_VISIBLE);  

  g_signal_connect_swapped (G_OBJECT (menu), "activate", 
                            G_CALLBACK (run_action), menu);

  return menu;
}

static void 
run_action (LauncherMenu *menu) 
{
  g_signal_emit_by_name ((gpointer) menu, "run");
}
