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

#ifndef __LAUNCHER_PROJECTS_POPUP_H__
#define	__LAUNCHER_PROJECTS_POPUP_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define LAUNCHER_PROJECTS_POPUP_TYPE            (launcher_projects_popup_get_type ())
#define LAUNCHER_PROJECTS_POPUP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), LAUNCHER_PROJECTS_POPUP_TYPE, LauncherProjectsPopup))
#define LAUNCHER_PROJECTS_POPUP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LAUNCHER_PROJECTS_POPUP_TYPE, LauncherProjectsPopupClass))
#define IS_LAUNCHER_PROJECTS_POPUP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LAUNCHER_PROJECTS_POPUP_TYPE))
#define IS_LAUNCHER_PROJECTS_POPUP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LAUNCHER_PROJECTS_POPUP_TYPE))

typedef struct _LauncherProjectsPopup LauncherProjectsPopup;
typedef struct _LauncherProjectsPopupClass LauncherProjectsPopupClass;

struct _LauncherProjectsPopup
{
  CodeSlayerMenuItem parent_instance;
};

struct _LauncherProjectsPopupClass
{
  CodeSlayerMenuItemClass parent_class;

  void (*run) (LauncherProjectsPopup *menu);
};

GType launcher_projects_popup_get_type (void) G_GNUC_CONST;
  
GtkWidget*  launcher_projects_popup_new  (void);
                                            
G_END_DECLS

#endif /* __LAUNCHER_PROJECTS_POPUP_H__ */
