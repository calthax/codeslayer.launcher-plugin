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

#ifndef __LAUNCHER_PROJECT_PROPERTIES_H__
#define	__LAUNCHER_PROJECT_PROPERTIES_H__

#include <gtk/gtk.h>
#include <codeslayer/codeslayer.h>
#include "launcher-config.h"

G_BEGIN_DECLS

#define LAUNCHER_PROJECT_PROPERTIES_TYPE            (launcher_project_properties_get_type ())
#define LAUNCHER_PROJECT_PROPERTIES(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), LAUNCHER_PROJECT_PROPERTIES_TYPE, LauncherProjectProperties))
#define LAUNCHER_PROJECT_PROPERTIES_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LAUNCHER_PROJECT_PROPERTIES_TYPE, LauncherProjectPropertiesClass))
#define IS_LAUNCHER_PROJECT_PROPERTIES(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LAUNCHER_PROJECT_PROPERTIES_TYPE))
#define IS_LAUNCHER_PROJECT_PROPERTIES_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LAUNCHER_PROJECT_PROPERTIES_TYPE))

typedef struct _LauncherProjectProperties LauncherProjectProperties;
typedef struct _LauncherProjectPropertiesClass LauncherProjectPropertiesClass;

struct _LauncherProjectProperties
{
  GtkVBox parent_instance;
};

struct _LauncherProjectPropertiesClass
{
  GtkVBoxClass parent_class;
  
  void (*save_config) (LauncherProjectProperties *project_properties);  
};

GType launcher_project_properties_get_type (void) G_GNUC_CONST;
     
GtkWidget*  launcher_project_properties_new  (void);

void launcher_project_properties_opened      (LauncherProjectProperties *project_properties,
                                            LauncherConfig   *config, 
                                            CodeSlayerProject       *project);

void launcher_project_properties_saved       (LauncherProjectProperties *project_properties,
                                            LauncherConfig   *config, 
                                            CodeSlayerProject       *project);

G_END_DECLS

#endif /* __LAUNCHER_PROJECT_PROPERTIES_H__ */
