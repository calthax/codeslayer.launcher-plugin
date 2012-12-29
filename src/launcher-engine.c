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
#include "launcher-config.h"

#define MAIN "main"
#define EXECUTABLE "executable"
#define PARAMETERS "parameters"
#define TERMINAL "terminal"
#define LAUNCHER_CONF "launcher.conf"

static void launcher_engine_class_init            (LauncherEngineClass *klass);
static void launcher_engine_init                  (LauncherEngine      *engine);
static void launcher_engine_finalize              (LauncherEngine      *engine);

static void run_action                            (LauncherEngine      *engine);
static void project_run_action                    (LauncherEngine      *engine, 
                                                   GList               *selections);
static LauncherConfig* get_config_by_project      (LauncherEngine      *engine, 
                                                   CodeSlayerProject   *project);
static void project_properties_opened_action      (LauncherEngine      *engine,
                                                   CodeSlayerProject   *project);
static void project_properties_saved_action       (LauncherEngine      *engine,
                                                   CodeSlayerProject   *project);                                                                        
static void save_config_action                    (LauncherEngine      *engine,
                                                   LauncherConfig      *config);                                                  
static CodeSlayerProject* get_selections_project  (GList               *selections);
static void launch_executable                     (LauncherEngine      *engine, 
                                                   CodeSlayerProject   *project);

#define LAUNCHER_ENGINE_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LAUNCHER_ENGINE_TYPE, LauncherEnginePrivate))

typedef struct _LauncherEnginePrivate LauncherEnginePrivate;

struct _LauncherEnginePrivate
{
  CodeSlayer *codeslayer;
  GtkWidget  *menu;
  GtkWidget  *project_properties;
  GtkWidget  *projects_menu;
  GList      *configs;
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
  priv->configs = NULL;
}

static void
launcher_engine_finalize (LauncherEngine *engine)
{
  LauncherEnginePrivate *priv;
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);  
  if (priv->configs != NULL)
    {
      g_list_foreach (priv->configs, (GFunc) g_object_unref, NULL);
      g_list_free (priv->configs);
      priv->configs = NULL;    
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

  g_signal_connect_swapped (G_OBJECT (project_properties), "save-config",
                            G_CALLBACK (save_config_action), engine);

  return engine;
}

static LauncherConfig*
get_config_by_project (LauncherEngine    *engine, 
                       CodeSlayerProject *project)
{
  LauncherEnginePrivate *priv;
  LauncherConfig *config;
  GKeyFile *keyfile;
  gchar *folder_path;
  gchar *file_path;
  gchar *executable;
  gchar *parameters;
  gboolean terminal;
  
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);

  folder_path = codeslayer_get_project_config_folder_path (priv->codeslayer, project);
  file_path = g_build_filename (folder_path, LAUNCHER_CONF, NULL);
  
  if (!codeslayer_utils_file_exists (file_path))
    {
      g_free (folder_path);
      g_free (file_path);
      return NULL;
    }

  keyfile = codeslayer_utils_get_keyfile (file_path);
  executable = g_key_file_get_string (keyfile, MAIN, EXECUTABLE, NULL);
  parameters = g_key_file_get_string (keyfile, MAIN, PARAMETERS, NULL);
  terminal = g_key_file_get_boolean (keyfile, MAIN, TERMINAL, NULL);
  
  config = launcher_config_new ();
  launcher_config_set_project (config, project);
  launcher_config_set_executable (config, executable);
  launcher_config_set_parameters (config, parameters);
  launcher_config_set_terminal (config, terminal);
  
  g_free (folder_path);
  g_free (file_path);
  g_free (executable);
  g_free (parameters);
  g_key_file_free (keyfile);
  
  return config;
}

static void
project_properties_opened_action (LauncherEngine    *engine,
                                  CodeSlayerProject *project)
{
  LauncherEnginePrivate *priv;
  LauncherConfig *config;
  
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);

  config = get_config_by_project (engine, project);
  launcher_project_properties_opened (LAUNCHER_PROJECT_PROPERTIES (priv->project_properties),
                                      config, project);
  if (config != NULL)
    g_object_unref (config);                                       
}

static void
project_properties_saved_action (LauncherEngine    *engine,
                                 CodeSlayerProject *project)
{
  LauncherEnginePrivate *priv;
  LauncherConfig *config;
  
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);

  config = get_config_by_project (engine, project);
  launcher_project_properties_saved (LAUNCHER_PROJECT_PROPERTIES (priv->project_properties),
                                     config, project);
  if (config != NULL)
    g_object_unref (config);                                       
}

static void
save_config_action (LauncherEngine *engine,
                    LauncherConfig *config)
{
  LauncherEnginePrivate *priv;
  CodeSlayerProject *project;
  gchar *folder_path;
  gchar *file_path;
  const gchar *executable;
  const gchar *parameters;
  gboolean terminal;
  GKeyFile *keyfile;
 
  priv = LAUNCHER_ENGINE_GET_PRIVATE (engine);

  project = launcher_config_get_project (config);  
  folder_path = codeslayer_get_project_config_folder_path (priv->codeslayer, project);
  file_path = codeslayer_utils_get_file_path (folder_path, LAUNCHER_CONF);
  keyfile = codeslayer_utils_get_keyfile (file_path);

  executable = launcher_config_get_executable (config);
  parameters = launcher_config_get_parameters (config);
  terminal = launcher_config_get_terminal (config);
  g_key_file_set_string (keyfile, MAIN, EXECUTABLE, executable);
  g_key_file_set_string (keyfile, MAIN, PARAMETERS, parameters);
  g_key_file_set_boolean (keyfile, MAIN, TERMINAL, terminal);

  codeslayer_utils_save_keyfile (keyfile, file_path);  
  g_key_file_free (keyfile);
  g_free (file_path); 
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
  LauncherConfig *config;
  const gchar *executable;
  const gchar *parameters;

  config = get_config_by_project (engine, project);
  
  if (config == NULL)
    {
      GtkWidget *dialog;
      gchar *msg;
      const gchar *project_name;
      
      project_name = codeslayer_project_get_name (project);
      
      msg = g_strconcat ("There is no launch config for project ", 
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
  
  executable = launcher_config_get_executable (config);
  parameters = launcher_config_get_parameters (config);

  if (launcher_config_get_terminal (config))
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
