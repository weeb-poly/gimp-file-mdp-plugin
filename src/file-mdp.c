#include <libgimp/gimp.h>

#include "file-mdp.h"
#include "file-mdp-load.h"
#include "file-mdp-thumb-load.h"

typedef struct _Mdp      Mdp;
typedef struct _MdpClass MdpClass;

struct _Mdp
{
  GimpPlugIn      parent_instance;
};

struct _MdpClass
{
  GimpPlugInClass parent_class;
};


#define MDP_TYPE  (mdp_get_type ())
#define MDP (obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MDP_TYPE, Mdp))

GType                   mdp_get_type         (void) G_GNUC_CONST;

static GList          * mdp_query_procedures (GimpPlugIn           *plug_in);
static GimpProcedure  * mdp_create_procedure (GimpPlugIn           *plug_in,
                                              const gchar          *name);

static GimpValueArray * mdp_load             (GimpProcedure        *procedure,
                                              GimpRunMode           run_mode,
                                              GFile                *file,
                                              const GimpValueArray *args,
                                              gpointer              run_data);
static GimpValueArray * mdp_load_thumb       (GimpProcedure        *procedure,
                                              GFile                *file,
                                              gint                  size,
                                              const GimpValueArray *args,
                                              gpointer              run_data);

G_DEFINE_TYPE (Mdp, mdp, GIMP_TYPE_PLUG_IN)


#ifdef _MSC_VER
#pragma comment(linker, "/ENTRY:mainCRTStartup")
#endif

GIMP_MAIN (MDP_TYPE)


static void
mdp_class_init (MdpClass *klass)
{
  GimpPlugInClass *plug_in_class = GIMP_PLUG_IN_CLASS (klass);

  plug_in_class->query_procedures = mdp_query_procedures;
  plug_in_class->create_procedure = mdp_create_procedure;
  plug_in_class->set_i18n = NULL;
}

static void
mdp_init (Mdp *mdp)
{
}

static GList *
mdp_query_procedures (GimpPlugIn *plug_in)
{
  GList *list = NULL;

  list = g_list_append (list, g_strdup (LOAD_THUMB_PROC));
  list = g_list_append (list, g_strdup (LOAD_PROC));

  return list;
}

static GimpProcedure *
mdp_create_procedure (GimpPlugIn  *plug_in,
                      const gchar *name)
{
  GimpProcedure *procedure = NULL;

  if (! strcmp (name, LOAD_PROC))
    {
      procedure = gimp_load_procedure_new (plug_in, name,
                                           GIMP_PDB_PROC_TYPE_PLUGIN,
                                           mdp_load, NULL, NULL);

      gimp_procedure_set_menu_label (procedure, "MDIPack File");

      gimp_procedure_set_documentation (procedure,
                                        "Loads images from the MDP file format.",
                                        "This plug-in loads images in "
                                        "MDIPack MDP format.",
                                        name);
      gimp_procedure_set_attribution (procedure,
                                      "Timothy Cyrus",
                                      "Timothy Cyrus",
                                      "2023");

      gimp_file_procedure_set_extensions (GIMP_FILE_PROCEDURE (procedure),
                                          "mdp");
      gimp_file_procedure_set_magics (GIMP_FILE_PROCEDURE (procedure),
                                      "0,string,mdipack");

      gimp_load_procedure_set_thumbnail_loader (GIMP_LOAD_PROCEDURE (procedure),
                                                LOAD_THUMB_PROC);
    }

  else if (! strcmp (name, LOAD_THUMB_PROC))
    {
      procedure = gimp_thumbnail_procedure_new (plug_in, name,
                                                GIMP_PDB_PROC_TYPE_PLUGIN,
                                                mdp_load_thumb, NULL, NULL);

      gimp_procedure_set_documentation (procedure,
                                        "Loads thumbnails from the "
                                        "MDP file format",
                                        "This plug-in loads thumbnail images "
                                        "from MDIPack MDP format files.",
                                        name);
      gimp_procedure_set_attribution (procedure,
                                      "Timothy Cyrus",
                                      "Timothy Cyrus",
                                      "2023");
    }

  return procedure;
}

static GimpValueArray *
mdp_load (GimpProcedure        *procedure,
          GimpRunMode           run_mode,
          GFile                *file,
          const GimpValueArray *args,
          gpointer              run_data)
{
  GimpValueArray *return_vals;
  GimpImage      *image;
  GError         *error = NULL;


  gegl_init (NULL, NULL);


  image = load_image (file, FALSE, &error);

  if (! image)
    return gimp_procedure_new_return_values (procedure,
                                             GIMP_PDB_EXECUTION_ERROR,
                                             error);

  return_vals = gimp_procedure_new_return_values (procedure,
                                                  GIMP_PDB_SUCCESS,
                                                  NULL);

  GIMP_VALUES_SET_IMAGE (return_vals, 1, image);

  return return_vals;
}


static GimpValueArray *
mdp_load_thumb (GimpProcedure        *procedure,
                GFile                *file,
                gint                  size,
                const GimpValueArray *args,
                gpointer              run_data)
{
  GimpValueArray *return_vals;
  gint            width  = 0;
  gint            height = 0;
  GimpImage      *image;
  GError         *error = NULL;

  gegl_init (NULL, NULL);

  image = load_thumbnail_image (file, &width, &height, &error);

  if (! image)
    return gimp_procedure_new_return_values (procedure,
                                             GIMP_PDB_EXECUTION_ERROR,
                                             error);

  return_vals = gimp_procedure_new_return_values (procedure,
                                                  GIMP_PDB_SUCCESS,
                                                  NULL);

  GIMP_VALUES_SET_IMAGE (return_vals, 1, image);
  GIMP_VALUES_SET_INT   (return_vals, 2, width);
  GIMP_VALUES_SET_INT   (return_vals, 3, height);

  gimp_value_array_truncate (return_vals, 4);

  return return_vals;
}
