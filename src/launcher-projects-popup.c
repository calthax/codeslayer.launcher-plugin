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
#include "launcher-projects-popup.h"

static void launcher_projects_popup_class_init  (LauncherProjectsPopupClass *klass);
static void launcher_projects_popup_init        (LauncherProjectsPopup      *projects_popup);
static void launcher_projects_popup_finalize    (LauncherProjectsPopup      *projects_popup);

static void run_action                          (LauncherProjectsPopup      *projects_popup, 
                                                 GList                      *selections);
                                        
enum
{
  RUN,
  LAST_SIGNAL
};

static guint launcher_projects_popup_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (LauncherProjectsPopup, launcher_projects_popup, CODESLAYER_MENU_ITEM_TYPE)

static void
launcher_projects_popup_class_init (LauncherProjectsPopupClass *klass)
{
  launcher_projects_popup_signals[RUN] =
    g_signal_new ("run", 
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                  G_STRUCT_OFFSET (LauncherProjectsPopupClass, run),
                  NULL, NULL, 
                  g_cclosure_marshal_VOID__POINTER, G_TYPE_NONE, 1, G_TYPE_POINTER);

  G_OBJECT_CLASS (klass)->finalize = (GObjectFinalizeFunc) launcher_projects_popup_finalize;
}

static void
launcher_projects_popup_init (LauncherProjectsPopup *projects_popup)
{
  gtk_menu_item_set_label (GTK_MENU_ITEM (projects_popup), "Run Program");
}

static void
launcher_projects_popup_finalize (LauncherProjectsPopup *projects_popup)
{
  G_OBJECT_CLASS (launcher_projects_popup_parent_class)->finalize (G_OBJECT (projects_popup));
}

GtkWidget*
launcher_projects_popup_new ()
{
  GtkWidget *projects_popup;
  
  projects_popup = g_object_new (launcher_projects_popup_get_type (), NULL);

  g_signal_connect_swapped (G_OBJECT (projects_popup), "projects-menu-selected", 
                            G_CALLBACK (run_action), projects_popup);

  return projects_popup;
}

static void 
run_action (LauncherProjectsPopup *projects_popup, 
            GList                 *selections) 
{
  g_signal_emit_by_name ((gpointer) projects_popup, "run", selections);
}
