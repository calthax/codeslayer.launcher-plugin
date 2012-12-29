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

#include "launcher-config.h"

static void launcher_config_class_init  (LauncherConfigClass *klass);
static void launcher_config_init        (LauncherConfig      *config);
static void launcher_config_finalize    (LauncherConfig      *config);

#define LAUNCHER_CONFIG_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LAUNCHER_CONFIG_TYPE, LauncherConfigPrivate))

typedef struct _LauncherConfigPrivate LauncherConfigPrivate;

struct _LauncherConfigPrivate
{
  CodeSlayerProject *project;
  gchar             *executable;
  gchar             *parameters;
  gboolean           terminal;
};

enum
{
  PROP_0,
  PROP_PROJECT_KEY,
  PROP_EXECUTABLE,
  PROP_PARAMETERS,
  PROP_TERMINAL
};

G_DEFINE_TYPE (LauncherConfig, launcher_config, G_TYPE_OBJECT)
     
static void 
launcher_config_class_init (LauncherConfigClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = (GObjectFinalizeFunc) launcher_config_finalize;
  g_type_class_add_private (klass, sizeof (LauncherConfigPrivate));
}

static void
launcher_config_init (LauncherConfig *config)
{
  LauncherConfigPrivate *priv;
  priv = LAUNCHER_CONFIG_GET_PRIVATE (config);
  priv->project = NULL;
  priv->executable = NULL;
  priv->parameters = NULL;
  priv->terminal = FALSE;
}

static void
launcher_config_finalize (LauncherConfig *config)
{
  LauncherConfigPrivate *priv;
  priv = LAUNCHER_CONFIG_GET_PRIVATE (config);
  if (priv->executable)
    {
      g_free (priv->executable);
      priv->executable = NULL;
    }
  if (priv->parameters)
    {
      g_free (priv->parameters);
      priv->parameters = NULL;
    }
  G_OBJECT_CLASS (launcher_config_parent_class)->finalize (G_OBJECT (config));
}

LauncherConfig*
launcher_config_new (void)
{
  return LAUNCHER_CONFIG (g_object_new (launcher_config_get_type (), NULL));
}

CodeSlayerProject*  
launcher_config_get_project (LauncherConfig    *config)
{
  LauncherConfigPrivate *priv;
  priv = LAUNCHER_CONFIG_GET_PRIVATE (config);
  return priv->project;
}

void
launcher_config_set_project (LauncherConfig    *config,
                             CodeSlayerProject *project)
{
  LauncherConfigPrivate *priv;
  priv = LAUNCHER_CONFIG_GET_PRIVATE (config);
  priv->project = project;
}                             

const gchar*
launcher_config_get_executable (LauncherConfig *config)
{
  return LAUNCHER_CONFIG_GET_PRIVATE (config)->executable;
}

void
launcher_config_set_executable (LauncherConfig *config,
                                       const gchar           *executable)
{
  LauncherConfigPrivate *priv;
  priv = LAUNCHER_CONFIG_GET_PRIVATE (config);
  if (priv->executable)
    {
      g_free (priv->executable);
      priv->executable = NULL;
    }
  priv->executable = g_strdup (executable);
}

const gchar*
launcher_config_get_parameters (LauncherConfig *config)
{
  return LAUNCHER_CONFIG_GET_PRIVATE (config)->parameters;
}

void
launcher_config_set_parameters (LauncherConfig *config,
                                       const gchar           *parameters)
{
  LauncherConfigPrivate *priv;
  priv = LAUNCHER_CONFIG_GET_PRIVATE (config);
  if (priv->parameters)
    {
      g_free (priv->parameters);
      priv->parameters = NULL;
    }
  priv->parameters = g_strdup (parameters);
}

const gboolean
launcher_config_get_terminal (LauncherConfig *config)
{
  return LAUNCHER_CONFIG_GET_PRIVATE (config)->terminal;
}

void
launcher_config_set_terminal (LauncherConfig *config,
                                     const gboolean         terminal)
{
  LauncherConfigPrivate *priv;
  priv = LAUNCHER_CONFIG_GET_PRIVATE (config);
  priv->terminal = terminal;
}
