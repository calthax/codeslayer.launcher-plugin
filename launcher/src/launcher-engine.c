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

#include <codeslayer/codeslayer-utils.h>
#include "launcher-engine.h"
#include "launcher-project-properties.h"
#include "launcher-configuration.h"

static void launcher_engine_class_init                          (LauncherEngineClass   *klass);
static void launcher_engine_init                                (LauncherEngine        *engine);
static void launcher_engine_finalize                            (LauncherEngine        *engine);

static void run_action                                          (LauncherEngine        *engine);
static void project_run_action                                  (LauncherEngine        *engine, 
                                                                 GList                 *selections);

static LauncherConfiguration* get_configuration_by_project_key  (LauncherEngine        *engine, 
                                                                 const gchar           *project_key);

static void project_properties_opened_action                    (LauncherEngine        *engine,
                                                                 CodeSlayerProject     *project);
static void project_properties_saved_action                     (LauncherEngine        *engine,
                                                                 CodeSlayerProject     *project);
                                                                        
static void save_configuration_action                           (LauncherEngine        *engine,
                                                                 LauncherConfiguration *configuration);
                                                  
static CodeSlayerProject* get_selections_project                (GList                 *selections);
static void launch_executable                                   (LauncherEngine        *engine, 
                                                                 CodeSlayerProject     *project);
                                                                 
static gchar* get_configuration_file_path                       (LauncherEngine        *engine);
                                                                 

#define LAUNCHER_ENGINE_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LAUNCHER_ENGINE_TYPE, LauncherEnginePrivate))

typedef struct _LauncherEnginePrivate LauncherEnginePrivate;

struct _LauncherEnginePrivate
{
  CodeSlayer *codeslayer;
  GtkWidget  *menu;
  GtkWidget  *project_properties;
  GtkWidget  *projects_menu;
  GList      *configurations;
  gulong      properties_opened_id;
  gulong      properties_saved_id;
};

G_DEFINE_TYPE (LauncherEngine, launcher_engine, G_TYPE_OBJECT)

static void
launcher_engine_class_init (LauncherEngineClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = (GObjectFinalizeFunc) launcher_engine_finalize;
  g_type_class_add_private (klass, sizeof (LauncherEnginePrivate));
}

static void
launcher_engine_init (LauncherEngine *engine) 
{
  LauncherEnginePrivate *priv;
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);  
  priv->configurations = NULL;
}

static void
launcher_engine_finalize (LauncherEngine *engine)
{
  LauncherEnginePrivate *priv;
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);  
  if (priv->configurations != NULL)
    {
      g_list_foreach (priv->configurations, (GFunc) g_object_unref, NULL);
      g_list_free (priv->configurations);
      priv->configurations = NULL;    
    }
  
  g_signal_handler_disconnect (priv->codeslayer, priv->properties_opened_id);
  g_signal_handler_disconnect (priv->codeslayer, priv->properties_saved_id);

  G_OBJECT_CLASS (launcher_engine_parent_class)->finalize (G_OBJECT(engine));
}

LauncherEngine*
launcher_engine_new (CodeSlayer *codeslayer,
                     GtkWidget  *menu, 
                     GtkWidget  *project_properties,
                     GtkWidget  *projects_menu)
{
  LauncherEnginePrivate *priv;
  LauncherEngine *engine;

  engine = LAUNCHER_ENGINE (g_object_new (launcher_engine_get_type (), NULL));
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);

  priv->codeslayer = codeslayer;
  priv->menu = menu;
  priv->project_properties = project_properties;
  priv->projects_menu = projects_menu;
  
  g_signal_connect_swapped (G_OBJECT (menu), "run",
                            G_CALLBACK (run_action), engine);

  g_signal_connect_swapped (G_OBJECT (projects_menu), "run",
                            G_CALLBACK (project_run_action), engine);

  priv->properties_opened_id =  g_signal_connect_swapped (G_OBJECT (codeslayer), "project-properties-opened",
                                                          G_CALLBACK (project_properties_opened_action), engine);

  priv->properties_saved_id = g_signal_connect_swapped (G_OBJECT (codeslayer), "project-properties-saved",
                                                        G_CALLBACK (project_properties_saved_action), engine);

  g_signal_connect_swapped (G_OBJECT (project_properties), "save-configuration",
                            G_CALLBACK (save_configuration_action), engine);

  return engine;
}

void 
launcher_engine_load_configurations (LauncherEngine *engine)
{
  LauncherEnginePrivate *priv;
  GList *configurations;
  gchar *file_path;

  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);
  
  file_path = get_configuration_file_path (engine);
  configurations = codeslayer_utils_get_gobjects (LAUNCHER_CONFIGURATION_TYPE,
                                                  FALSE,
                                                  file_path, 
                                                  "launcher",
                                                  "project_key", G_TYPE_STRING,
                                                  "executable", G_TYPE_STRING,
                                                  "parameters", G_TYPE_STRING,
                                                  "terminal", G_TYPE_BOOLEAN, 
                                                  NULL);
  priv->configurations = configurations;
  g_free (file_path);
}

static LauncherConfiguration*
get_configuration_by_project_key (LauncherEngine *engine, 
                                  const gchar    *project_key)
{
  LauncherEnginePrivate *priv;
  GList *list;

  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);

  list = priv->configurations;
  while (list != NULL)
    {
      LauncherConfiguration *configuration = list->data;
      const gchar *key;
      
      key = launcher_configuration_get_project_key (configuration);
      
      if (g_strcmp0 (project_key, key) == 0)
        return configuration;

      list = g_list_next (list);
    }

  return NULL;
}

static void
project_properties_opened_action (LauncherEngine      *engine,
                                  CodeSlayerProject *project)
{
  LauncherEnginePrivate *priv;
  const gchar *project_key;
  LauncherConfiguration *configuration;
  
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);

  project_key = codeslayer_project_get_key (project);
  configuration = get_configuration_by_project_key (engine, project_key);
  
  launcher_project_properties_opened (LAUNCHER_PROJECT_PROPERTIES (priv->project_properties),
                                      configuration, project);
}

static void
project_properties_saved_action (LauncherEngine      *engine,
                                 CodeSlayerProject *project)
{
  LauncherEnginePrivate *priv;
  const gchar *project_key;
  LauncherConfiguration *configuration;
  
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);

  project_key = codeslayer_project_get_key (project);
  configuration = get_configuration_by_project_key (engine, project_key);
  
  launcher_project_properties_saved (LAUNCHER_PROJECT_PROPERTIES (priv->project_properties),
                                     configuration, project);
}

static void
save_configuration_action (LauncherEngine        *engine,
                           LauncherConfiguration *configuration)
{
  LauncherEnginePrivate *priv;
  GList *list;
  GList *tmp;
  gchar *file_path;
    
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);
  
  if (configuration)
    priv->configurations = g_list_prepend (priv->configurations, configuration);

  list = g_list_copy (priv->configurations);
  tmp = list;
  
  while (tmp != NULL)
    {
      LauncherConfiguration *configuration = tmp->data;
      const gchar *executable;
      const gchar *parameters;

      executable = launcher_configuration_get_executable (configuration);
      parameters = launcher_configuration_get_parameters (configuration);
      
      if (g_utf8_strlen (executable, -1) == 0 &&
          g_utf8_strlen (parameters, -1) == 0)
      priv->configurations = g_list_remove (priv->configurations, configuration);
      tmp = g_list_next (tmp);
    }
    
  g_list_free (list);
  
  file_path = get_configuration_file_path (engine);  
  codeslayer_utils_save_gobjects (priv->configurations,
                                  file_path, 
                                  "launcher",
                                  "project_key", G_TYPE_STRING,
                                  "executable", G_TYPE_STRING,
                                  "parameters", G_TYPE_STRING,
                                  "terminal", G_TYPE_BOOLEAN, 
                                  NULL);  
  g_free (file_path);
}

static gchar*
get_configuration_file_path (LauncherEngine *engine)
{
  LauncherEnginePrivate *priv;
  gchar *folder_path;
  gchar *file_path;
  
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);

  folder_path = codeslayer_get_active_group_folder_path (priv->codeslayer);  
  file_path = g_build_filename (folder_path, "launchers.xml", NULL);
  g_free (folder_path);
  
  return file_path;
}

static void
run_action (LauncherEngine *engine)
{
  LauncherEnginePrivate *priv;
  CodeSlayerProject *project;
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);
  project = codeslayer_get_active_editor_project (priv->codeslayer);
  
  if (project == NULL)
    {
      GtkWidget *dialog;
      dialog =  gtk_message_dialog_new (NULL, 
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                        "There are no open editors. Not able to determine what program to run.");
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);
      return;
    }
  
  launch_executable (engine, project);
}

static void
project_run_action (LauncherEngine *engine, 
                    GList          *selections)
{
  CodeSlayerProject *project;
  project = get_selections_project (selections);
  launch_executable (engine, project);
}

static void
launch_executable (LauncherEngine    *engine, 
                   CodeSlayerProject *project)
{
  GAppInfo *appinfo;
  LauncherConfiguration *configuration;
  const gchar *project_key;
  const gchar *executable;
  const gchar *parameters;

  project_key = codeslayer_project_get_key (project);
  
  configuration = get_configuration_by_project_key (engine, project_key);
  
  if (configuration == NULL)
    {
      GtkWidget *dialog;
      gchar *msg;
      const gchar *project_name;
      
      project_name = codeslayer_project_get_name (project);
      
      msg = g_strconcat ("There is no launch configuration for project ", 
                         project_name, ".", NULL);      
      dialog =  gtk_message_dialog_new (NULL, 
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                        msg, 
                                        NULL);
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);
      g_free (msg);
      return;
    }
  
  executable = launcher_configuration_get_executable (configuration);
  parameters = launcher_configuration_get_parameters (configuration);

  if (launcher_configuration_get_terminal (configuration))
    {
      gchar *command;              
      command = g_strconcat (executable, " ", parameters, NULL);
      appinfo = g_app_info_create_from_commandline (command, NULL,
                                                    G_APP_INFO_CREATE_NEEDS_TERMINAL, NULL);
      g_free (command);
    }
  else
    {
      gchar *command;              
      command = g_strconcat (executable, " ", parameters, NULL);
      appinfo = g_app_info_create_from_commandline (executable, NULL,
                                                    G_APP_INFO_CREATE_NONE, NULL);
      g_free (command);
    }

  g_app_info_launch (appinfo, NULL, NULL, NULL);
  g_object_unref (appinfo);
}

static CodeSlayerProject*
get_selections_project (GList *selections)
{
  if (selections != NULL)
    {
      CodeSlayerProjectsSelection *selection = selections->data;
      return codeslayer_projects_selection_get_project (CODESLAYER_PROJECTS_SELECTION (selection));
    }
  return NULL;  
}
