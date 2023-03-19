#include <libgimp/gimp.h>

#include "file-mdp.h"
#include "file-mdp-load.h"
#include "mdp-parse.h"
#include "mdp-util.h"

GimpImage * load_image  (GFile        *file,
                         gboolean      interactive,
                         GError      **load_error)
{
  GInputStream  *input;
  MDPimage       img_a;
  GimpImage     *image = NULL;
  GError        *error = NULL;

  /* ----- Open MDP file ----- */

  gimp_progress_init_printf ("Opening '%s'",
                             gimp_file_get_utf8_name (file));

  g_debug ("Open file %s", gimp_file_get_utf8_name (file));

  input = G_INPUT_STREAM (g_file_read (file, NULL, &error));
  if (! input)
    {
      if (! error)
        g_set_error (load_error, G_FILE_ERROR, g_file_error_from_errno (errno),
                     "Could not open '%s' for reading: %s",
                     gimp_file_get_utf8_name (file), g_strerror (errno));
      else
        {
          g_set_error (load_error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                       "Could not open '%s' for reading: %s",
                       gimp_file_get_utf8_name (file), error->message);
          g_error_free (error);
        }

      return NULL;
    }


  /* ----- Read the MDP file Header block ----- */
  g_debug ("Read header block at offset %" G_GOFFSET_FORMAT,
           MDP_TELL(input));
  if (mdp_read_header_block (&img_a, input, &error) < 0)
    goto load_error;
  gimp_progress_update (0.1);


  /* ----- Read the MDI block ----- */
  g_debug ("Read MDI block at offset %" G_GOFFSET_FORMAT,
           MDP_TELL(input));
  if (mdp_read_mdi_block (&img_a, input, &error) < 0)
    goto load_error;
  gimp_progress_update (0.2);


  /* ----- Read the MDIBIN block ----- */
  g_debug ("Read MDIBIN block at offset %" G_GOFFSET_FORMAT,
           MDP_TELL(input));
  if (mdp_read_mdibin_block (&img_a, input, &error) < 0)
    goto load_error;
  gimp_progress_update (0.3);

  // TODO: Convert MDP Data to GIMP Image

  gimp_image_clean_all (image);
  gimp_image_undo_enable (image);
  g_object_unref (input);
  return image;

  /* ----- Process load errors ----- */
 load_error:
  if (error)
    {
      g_set_error (load_error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                   "Error loading MDP file: %s", error->message);
      g_error_free (error);
    }

  /* Delete partially loaded image */
  if (image)
    gimp_image_delete (image);

  /* Close file if Open */
  g_object_unref (input);

  return NULL;
}