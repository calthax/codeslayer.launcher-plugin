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

#ifndef __LAUNCHER_CONFIG_H__
#define	__LAUNCHER_CONFIG_H__

#include <gtk/gtk.h>
#include <codeslayer/codeslayer.h>

G_BEGIN_DECLS

#define LAUNCHER_CONFIG_TYPE            (launcher_config_get_type ())
#define LAUNCHER_CONFIG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), LAUNCHER_CONFIG_TYPE, LauncherConfig))
#define LAUNCHER_CONFIG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LAUNCHER_CONFIG_TYPE, LauncherConfigClass))
#define IS_LAUNCHER_CONFIG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LAUNCHER_CONFIG_TYPE))
#define IS_LAUNCHER_CONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LAUNCHER_CONFIG_TYPE))

typedef struct _LauncherConfig LauncherConfig;
typedef struct _LauncherConfigClass LauncherConfigClass;

struct _LauncherConfig
{
  GObject parent_instance;
};

struct _LauncherConfigClass
{
  GObjectClass parent_class;
};

GType launcher_config_get_type (void) G_GNUC_CONST;

LauncherConfig*     launcher_config_new                       (void);

CodeSlayerProject*  launcher_config_get_project      (LauncherConfig    *config);
void                launcher_config_set_project      (LauncherConfig    *config,
                                                      CodeSlayerProject *project);
const gchar*        launcher_config_get_executable   (LauncherConfig    *config);
void                launcher_config_set_executable   (LauncherConfig    *config,
                                                      const gchar       *executable);
const gchar*        launcher_config_get_parameters   (LauncherConfig    *config);
void                launcher_config_set_parameters   (LauncherConfig    *config,
                                                      const gchar       *parameters);
const gboolean      launcher_config_get_terminal     (LauncherConfig    *config);
void                launcher_config_set_terminal     (LauncherConfig    *config,
                                                      gboolean           terminal);

G_END_DECLS

#endif /* __LAUNCHER_CONFIG_H__ */
