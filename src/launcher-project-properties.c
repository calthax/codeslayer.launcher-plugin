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

#include "launcher-project-properties.h"

static void launcher_project_properties_class_init  (LauncherProjectPropertiesClass *klass);
static void launcher_project_properties_init        (LauncherProjectProperties      *project_properties);
static void launcher_project_properties_finalize    (LauncherProjectProperties      *project_properties);

static void add_form                                (LauncherProjectProperties      *project_properties);

static void executable_icon_action                  (GtkEntry                       *executable_entry, 
                                                     GtkEntryIconPosition            icon_pos, 
                                                     GdkEvent                       *event,
                                                     LauncherProjectProperties      *project_properties);

static gboolean entry_has_text                      (GtkWidget                      *entry);

#define LAUNCHER_PROJECT_PROPERTIES_GET_PRIVATE(obj) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LAUNCHER_PROJECT_PROPERTIES_TYPE, LauncherProjectPropertiesPrivate))

typedef struct _LauncherProjectPropertiesPrivate LauncherProjectPropertiesPrivate;

struct _LauncherProjectPropertiesPrivate
{
  CodeSlayerProject *project;
  GtkWidget         *executable_entry;
  GtkWidget         *parameters_entry;
  GtkWidget         *terminal_check_button;
};

enum
{
  SAVE_CONFIGURATION,
  LAST_SIGNAL
};

static guint launcher_project_properties_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (LauncherProjectProperties, launcher_project_properties, GTK_TYPE_VBOX)

static void
launcher_project_properties_class_init (LauncherProjectPropertiesClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  launcher_project_properties_signals[SAVE_CONFIGURATION] =
    g_signal_new ("save-config", 
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                  G_STRUCT_OFFSET (LauncherProjectPropertiesClass, save_config), 
                  NULL, NULL,
                  g_cclosure_marshal_VOID__OBJECT, G_TYPE_NONE, 1, LAUNCHER_CONFIG_TYPE);

  gobject_class->finalize = (GObjectFinalizeFunc) launcher_project_properties_finalize;
  g_type_class_add_private (klass, sizeof (LauncherProjectPropertiesPrivate));
}

static void
launcher_project_properties_init (LauncherProjectProperties *project_properties) {}

static void
launcher_project_properties_finalize (LauncherProjectProperties *project_properties)
{
  G_OBJECT_CLASS (launcher_project_properties_parent_class)->finalize (G_OBJECT(project_properties));
}

GtkWidget*
launcher_project_properties_new (void)
{
  GtkWidget *project_properties;
  project_properties = g_object_new (launcher_project_properties_get_type (), NULL);
  add_form (LAUNCHER_PROJECT_PROPERTIES (project_properties));
  return project_properties;
}

static void 
add_form (LauncherProjectProperties *project_properties)
{
  LauncherProjectPropertiesPrivate *priv;
  GtkWidget *grid;

  GtkWidget *executable_label;
  GtkWidget *executable_entry;

  GtkWidget *parameters_label;
  GtkWidget *parameters_entry;

  GtkWidget *spacer;
  GtkWidget *hbox;
  GtkWidget *terminal_check_button;

  priv = LAUNCHER_PROJECT_PROPERTIES_GET_PRIVATE (project_properties);

  grid = gtk_grid_new ();
  gtk_grid_set_row_spacing (GTK_GRID (grid), 2);

  executable_label = gtk_label_new (_("Executable:"));
  gtk_misc_set_alignment (GTK_MISC (executable_label), 1, .5);
  gtk_misc_set_padding (GTK_MISC (executable_label), 4, 0);
  gtk_grid_attach (GTK_GRID (grid), executable_label, 0, 0, 1, 1);
                    
  executable_entry = gtk_entry_new ();
  priv->executable_entry = executable_entry;
  gtk_entry_set_width_chars (GTK_ENTRY (executable_entry), 50);
  gtk_entry_set_icon_from_stock (GTK_ENTRY (executable_entry), 
                                 GTK_ENTRY_ICON_SECONDARY, GTK_STOCK_FILE);
  gtk_grid_attach_next_to (GTK_GRID (grid), executable_entry, executable_label, 
                           GTK_POS_RIGHT, 1, 1);
                      
  parameters_label = gtk_label_new (_("Parameters:"));
  gtk_misc_set_alignment (GTK_MISC (parameters_label), 1, .5);
  gtk_misc_set_padding (GTK_MISC (parameters_label), 4, 0);
  gtk_grid_attach (GTK_GRID (grid), parameters_label, 0, 1, 1, 1);
  
  parameters_entry = gtk_entry_new ();
  priv->parameters_entry = parameters_entry;
  gtk_entry_set_width_chars (GTK_ENTRY (parameters_entry), 50);
  gtk_grid_attach_next_to (GTK_GRID (grid), parameters_entry, parameters_label, 
                           GTK_POS_RIGHT, 1, 1);

  spacer = gtk_label_new ("");
  gtk_grid_attach (GTK_GRID (grid), spacer, 0, 2, 1, 1);

  terminal_check_button = gtk_check_button_new_with_label (_("Run In Terminal"));
  priv->terminal_check_button = terminal_check_button;
  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);                      
  gtk_box_pack_start (GTK_BOX (hbox), terminal_check_button, FALSE, FALSE, 0);
  gtk_grid_attach_next_to (GTK_GRID (grid), hbox, spacer, GTK_POS_RIGHT, 1, 1);
                      
  gtk_box_pack_start (GTK_BOX (project_properties), grid, FALSE, FALSE, 3);

  g_signal_connect (G_OBJECT (executable_entry), "icon-press",
                    G_CALLBACK (executable_icon_action), project_properties);
}

static void 
executable_icon_action (GtkEntry                   *executable_entry,
                        GtkEntryIconPosition        icon_pos,
                        GdkEvent                   *event,
                        LauncherProjectProperties  *project_properties)
{
  LauncherProjectPropertiesPrivate *priv;
  GtkWidget *dialog;
  gint response;
  const gchar *folder_path;
  
  priv = LAUNCHER_PROJECT_PROPERTIES_GET_PRIVATE (project_properties);

  dialog = gtk_file_chooser_dialog_new ("Select Executable", 
                                        NULL,
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        GTK_STOCK_CANCEL,
                                        GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OPEN,
                                        GTK_RESPONSE_OK, 
                                        NULL);

  gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);

  folder_path = codeslayer_project_get_folder_path (priv->project);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(dialog), folder_path);

  response = gtk_dialog_run (GTK_DIALOG (dialog));
  if (response == GTK_RESPONSE_OK)
    {
      GFile *file;
      char *file_path;
      file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (dialog));
      file_path = g_file_get_path (file);
      gtk_entry_set_text (executable_entry, file_path);
      g_free (file_path);
      g_object_unref (file);
    }

  gtk_widget_destroy (GTK_WIDGET (dialog));
  
}                        

void 
launcher_project_properties_opened (LauncherProjectProperties *project_properties,
                                    LauncherConfig     *config, 
                                    CodeSlayerProject         *project)
{
  LauncherProjectPropertiesPrivate *priv;

  priv = LAUNCHER_PROJECT_PROPERTIES_GET_PRIVATE (project_properties);  
  priv->project = project;
  
  if (config)
    {
      const gchar *executable;
      const gchar *parameters;
      gboolean terminal;
    
      executable = launcher_config_get_executable (config);
      parameters = launcher_config_get_parameters (config);
      terminal = launcher_config_get_terminal (config);

      gtk_entry_set_text (GTK_ENTRY (priv->executable_entry), executable);
      gtk_entry_set_text (GTK_ENTRY (priv->parameters_entry), parameters);
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (priv->terminal_check_button),
                                    terminal);
    }
  else
    {
      gtk_entry_set_text (GTK_ENTRY (priv->executable_entry), "");
      gtk_entry_set_text (GTK_ENTRY (priv->parameters_entry), "");
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (priv->terminal_check_button),
                                    FALSE);
    }
}

void 
launcher_project_properties_saved (LauncherProjectProperties *project_properties,
                                   LauncherConfig     *config, 
                                   CodeSlayerProject         *project)
{
  LauncherProjectPropertiesPrivate *priv;
  gchar *executable;
  gchar *parameters;
  gboolean terminal;

  priv = LAUNCHER_PROJECT_PROPERTIES_GET_PRIVATE (project_properties);
  
  executable = g_strdup (gtk_entry_get_text (GTK_ENTRY (priv->executable_entry)));
  parameters = g_strdup (gtk_entry_get_text (GTK_ENTRY (priv->parameters_entry)));
  terminal = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (priv->terminal_check_button));
  
  g_strstrip (executable);
  g_strstrip (parameters);
  
  if (config != NULL)
    {
      if (g_strcmp0 (executable, launcher_config_get_executable (config)) == 0 &&
          g_strcmp0 (parameters, launcher_config_get_parameters (config)) == 0 &&
          terminal == launcher_config_get_terminal (config))
        {
          g_free (executable);
          g_free (parameters);
          return;
        }

      launcher_config_set_executable (config, executable);
      launcher_config_set_parameters (config, parameters);
      launcher_config_set_terminal (config, terminal);
      g_signal_emit_by_name((gpointer)project_properties, "save-config", config);
    }
  else if (entry_has_text (priv->executable_entry))
    {
      LauncherConfig *config;
      config = launcher_config_new ();
      launcher_config_set_project (config, project);
      launcher_config_set_executable (config, executable);
      launcher_config_set_parameters (config, parameters);
      launcher_config_set_terminal (config, terminal);
      g_signal_emit_by_name((gpointer)project_properties, "save-config", config);
      g_object_unref (config);
    }
    
  g_free (executable);
  g_free (parameters);
}

static gboolean
entry_has_text (GtkWidget *entry)
{
  return gtk_entry_buffer_get_length (gtk_entry_get_buffer (GTK_ENTRY (entry))) > 0;
}
