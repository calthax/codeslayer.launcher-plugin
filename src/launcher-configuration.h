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

#ifndef __LAUNCHER_CONFIGURATION_H__
#define	__LAUNCHER_CONFIGURATION_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define LAUNCHER_CONFIGURATION_TYPE            (launcher_configuration_get_type ())
#define LAUNCHER_CONFIGURATION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), LAUNCHER_CONFIGURATION_TYPE, LauncherConfiguration))
#define LAUNCHER_CONFIGURATION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LAUNCHER_CONFIGURATION_TYPE, LauncherConfigurationClass))
#define IS_LAUNCHER_CONFIGURATION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LAUNCHER_CONFIGURATION_TYPE))
#define IS_LAUNCHER_CONFIGURATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LAUNCHER_CONFIGURATION_TYPE))

typedef struct _LauncherConfiguration LauncherConfiguration;
typedef struct _LauncherConfigurationClass LauncherConfigurationClass;

struct _LauncherConfiguration
{
  GObject parent_instance;
};

struct _LauncherConfigurationClass
{
  GObjectClass parent_class;
};

GType launcher_configuration_get_type (void) G_GNUC_CONST;

LauncherConfiguration*  launcher_configuration_new                       (void);

const gchar*             launcher_configuration_get_project_key  (LauncherConfiguration *configuration);
void                     launcher_configuration_set_project_key  (LauncherConfiguration *configuration,
                                                                  const gchar           *project_key);
const gchar*             launcher_configuration_get_executable   (LauncherConfiguration *configuration);
void                     launcher_configuration_set_executable   (LauncherConfiguration *configuration,
                                                                  const gchar           *executable);
const gchar*             launcher_configuration_get_parameters   (LauncherConfiguration *configuration);
void                     launcher_configuration_set_parameters   (LauncherConfiguration *configuration,
                                                                  const gchar           *parameters);
const gboolean           launcher_configuration_get_terminal     (LauncherConfiguration *configuration);
void                     launcher_configuration_set_terminal     (LauncherConfiguration *configuration,
                                                                  gboolean               terminal);

G_END_DECLS

#endif /* __LAUNCHER_CONFIGURATION_H__ */
