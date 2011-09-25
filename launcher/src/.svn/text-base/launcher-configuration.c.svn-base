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

#include "launcher-configuration.h"

static void launcher_configuration_class_init    (LauncherConfigurationClass *klass);
static void launcher_configuration_init          (LauncherConfiguration      *configuration);
static void launcher_configuration_finalize      (LauncherConfiguration      *configuration);
static void launcher_configuration_get_property  (GObject                    *object, 
                                                  guint                       prop_id,
                                                  GValue                     *value,
                                                  GParamSpec                 *pspec);
static void launcher_configuration_set_property  (GObject                    *object, 
                                                  guint                       prop_id,
                                                  const GValue               *value,
                                                  GParamSpec                 *pspec);

#define LAUNCHER_CONFIGURATION_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LAUNCHER_CONFIGURATION_TYPE, LauncherConfigurationPrivate))

typedef struct _LauncherConfigurationPrivate LauncherConfigurationPrivate;

struct _LauncherConfigurationPrivate
{
  gchar    *project_key;
  gchar    *executable;
  gchar    *parameters;
  gboolean  terminal;
};

enum
{
  PROP_0,
  PROP_PROJECT_KEY,
  PROP_EXECUTABLE,
  PROP_PARAMETERS,
  PROP_TERMINAL
};

G_DEFINE_TYPE (LauncherConfiguration, launcher_configuration, G_TYPE_OBJECT)
     
static void 
launcher_configuration_class_init (LauncherConfigurationClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->finalize = (GObjectFinalizeFunc) launcher_configuration_finalize;

  gobject_class->get_property = launcher_configuration_get_property;
  gobject_class->set_property = launcher_configuration_set_property;

  g_type_class_add_private (klass, sizeof (LauncherConfigurationPrivate));

  g_object_class_install_property (gobject_class, 
                                   PROP_PROJECT_KEY,
                                   g_param_spec_string ("project_key", 
                                                        "Project Key",
                                                        "Project Key", 
                                                        "",
                                                        G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, 
                                   PROP_EXECUTABLE,
                                   g_param_spec_string ("executable",
                                                        "Executable",
                                                        "Executable",
                                                        "",
                                                        G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, 
                                   PROP_PARAMETERS,
                                   g_param_spec_string ("parameters",
                                                        "Parameters",
                                                        "Parameters",
                                                        "",
                                                        G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, 
                                   PROP_TERMINAL,
                                   g_param_spec_boolean ("terminal", 
                                                         "Terminal",
                                                         "Terminal", FALSE,
                                                         G_PARAM_READWRITE));
}

static void
launcher_configuration_init (LauncherConfiguration *configuration)
{
  LauncherConfigurationPrivate *priv;
  priv = LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration);
  priv->project_key = NULL;
  priv->executable = NULL;
  priv->parameters = NULL;
  priv->terminal = FALSE;
}

static void
launcher_configuration_finalize (LauncherConfiguration *configuration)
{
  LauncherConfigurationPrivate *priv;
  priv = LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration);
  if (priv->project_key)
    {
      g_free (priv->project_key);
      priv->project_key = NULL;
    }
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
  G_OBJECT_CLASS (launcher_configuration_parent_class)->finalize (G_OBJECT (configuration));
}

static void
launcher_configuration_get_property (GObject    *object, 
                                     guint       prop_id,
                                     GValue     *value, 
                                     GParamSpec *pspec)
{
  LauncherConfiguration *configuration;
  LauncherConfigurationPrivate *priv;
  
  configuration = LAUNCHER_CONFIGURATION (object);
  priv = LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration);

  switch (prop_id)
    {
    case PROP_PROJECT_KEY:
      g_value_set_string (value, priv->project_key);
      break;
    case PROP_EXECUTABLE:
      g_value_set_string (value, priv->executable);
      break;
    case PROP_PARAMETERS:
      g_value_set_string (value, priv->parameters);
      break;
    case PROP_TERMINAL:
      g_value_set_boolean (value, priv->terminal);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
launcher_configuration_set_property (GObject      *object, 
                                     guint         prop_id,
                                     const GValue *value, 
                                     GParamSpec   *pspec)
{
  LauncherConfiguration *configuration;
  configuration = LAUNCHER_CONFIGURATION (object);

  switch (prop_id)
    {
    case PROP_PROJECT_KEY:
      launcher_configuration_set_project_key (configuration, g_value_get_string (value));
      break;
    case PROP_EXECUTABLE:
      launcher_configuration_set_executable (configuration, g_value_get_string (value));
      break;
    case PROP_PARAMETERS:
      launcher_configuration_set_parameters (configuration, g_value_get_string (value));
      break;
    case PROP_TERMINAL:
      launcher_configuration_set_terminal (configuration, g_value_get_boolean (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

LauncherConfiguration*
launcher_configuration_new (void)
{
  return LAUNCHER_CONFIGURATION (g_object_new (launcher_configuration_get_type (), NULL));
}

const gchar*
launcher_configuration_get_project_key (LauncherConfiguration *configuration)
{
  return LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration)->project_key;
}

void
launcher_configuration_set_project_key (LauncherConfiguration *configuration, 
                                        const gchar           *project_key)
{
  LauncherConfigurationPrivate *priv;
  priv = LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration);
  if (priv->project_key)
    {
      g_free (priv->project_key);
      priv->project_key = NULL;
    }
  priv->project_key = g_strdup (project_key);
}

const gchar*
launcher_configuration_get_executable (LauncherConfiguration *configuration)
{
  return LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration)->executable;
}

void
launcher_configuration_set_executable (LauncherConfiguration *configuration,
                                       const gchar           *executable)
{
  LauncherConfigurationPrivate *priv;
  priv = LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration);
  if (priv->executable)
    {
      g_free (priv->executable);
      priv->executable = NULL;
    }
  priv->executable = g_strdup (executable);
}

const gchar*
launcher_configuration_get_parameters (LauncherConfiguration *configuration)
{
  return LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration)->parameters;
}

void
launcher_configuration_set_parameters (LauncherConfiguration *configuration,
                                       const gchar           *parameters)
{
  LauncherConfigurationPrivate *priv;
  priv = LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration);
  if (priv->parameters)
    {
      g_free (priv->parameters);
      priv->parameters = NULL;
    }
  priv->parameters = g_strdup (parameters);
}

const gboolean
launcher_configuration_get_terminal (LauncherConfiguration *configuration)
{
  return LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration)->terminal;
}

void
launcher_configuration_set_terminal (LauncherConfiguration *configuration,
                                     const gboolean         terminal)
{
  LauncherConfigurationPrivate *priv;
  priv = LAUNCHER_CONFIGURATION_GET_PRIVATE (configuration);
  priv->terminal = terminal;
}
