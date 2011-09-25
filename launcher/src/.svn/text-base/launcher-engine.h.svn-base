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

#ifndef __LAUNCHER_ENGINE_H__
#define	__LAUNCHER_ENGINE_H__

#include <gtk/gtk.h>
#include <codeslayer/codeslayer.h>

G_BEGIN_DECLS

#define LAUNCHER_ENGINE_TYPE            (launcher_engine_get_type ())
#define LAUNCHER_ENGINE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), LAUNCHER_ENGINE_TYPE, LauncherEngine))
#define LAUNCHER_ENGINE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LAUNCHER_ENGINE_TYPE, LauncherEngineClass))
#define IS_LAUNCHER_ENGINE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LAUNCHER_ENGINE_TYPE))
#define IS_LAUNCHER_ENGINE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LAUNCHER_ENGINE_TYPE))

typedef struct _LauncherEngine LauncherEngine;
typedef struct _LauncherEngineClass LauncherEngineClass;

struct _LauncherEngine
{
  GObject parent_instance;
};

struct _LauncherEngineClass
{
  GObjectClass parent_class;
};

GType launcher_engine_get_type (void) G_GNUC_CONST;

LauncherEngine*  launcher_engine_new (CodeSlayer *codeslayer,
                                  GtkWidget  *menu,
                                  GtkWidget  *project_properties,
                                  GtkWidget  *projects_menu);
                                        
void launcher_engine_load_configurations (LauncherEngine *engine);

G_END_DECLS

#endif /* _LAUNCHER_ENGINE_H */
