/*
 * Copyright (C) 2010 - Jeff Johnston
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __LAUNCHER_MENU_H__
#define	__LAUNCHER_MENU_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define LAUNCHER_MENU_TYPE            (launcher_menu_get_type ())
#define LAUNCHER_MENU(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), LAUNCHER_MENU_TYPE, LauncherMenu))
#define LAUNCHER_MENU_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LAUNCHER_MENU_TYPE, LauncherMenuClass))
#define IS_LAUNCHER_MENU(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LAUNCHER_MENU_TYPE))
#define IS_LAUNCHER_MENU_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LAUNCHER_MENU_TYPE))

typedef struct _LauncherMenu LauncherMenu;
typedef struct _LauncherMenuClass LauncherMenuClass;

struct _LauncherMenu
{
  GtkMenuItem parent_instance;
};

struct _LauncherMenuClass
{
  GtkMenuItemClass parent_class;

  void (*run) (LauncherMenu *menu);
};

GType launcher_menu_get_type (void) G_GNUC_CONST;
  
GtkWidget*  launcher_menu_new  (GtkAccelGroup *accel_group);
                                            
G_END_DECLS

#endif /* __LAUNCHER_MENU_H__ */
