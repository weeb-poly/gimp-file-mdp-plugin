#include <libgimp/gimp.h>
#include <libxml/xpath.h>

#include "file-mdp.h"
#include "file-mdp-thumb-load.h"
#include "mdp-parse.h"
#include "mdp-util.h"

GimpImage*
load_thumbnail_image (GFile   *file,
                      gint    *width,
                      gint    *height,
                      GError **load_error)
{

  GInputStream  *input;
  MDPimage       img_a;
  GimpImage     *image = NULL;
  GError        *error = NULL;

  /* ----- Open MDP file ----- */

  gimp_progress_init_printf ("Opening thumbnail for '%s'",
                             gimp_file_get_utf8_name (file));

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

  // TODO:
  // - Don't parse mdibin yet
  // - Search for the 'Thumb' element in mdi
  // - Set 'width' and 'height' pointers to attr values
  // - Use 'bin' attr value as mdibin index
  // - Find Thumbnail Chunk and skip everything else
  // - Find Chunk and zlib decompress if necessary
  // - Use gegl to convert BGRA to RGBA
  // ... profit?

  xmlXPathContextPtr xPathCtx = xmlXPathNewContext(img_a.mdi);
  xPathCtx->node = xmlDocGetRootElement(img_a.mdi);

  xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)"//Thumb", xPathCtx);

  if (xpathObj == NULL || xmlXPathNodeSetIsEmpty(xpathObj->nodesetval)) {
    // ERROR
    goto load_error;
  }

  xmlNodePtr node = xmlXPathNodeSetItem(xpathObj->nodesetval, 0);

  xmlChar* _width = xmlGetProp(node, (const xmlChar*)"width");
  *width = atoi((const char*)_width);

  xmlChar* _height = xmlGetProp(node, (const xmlChar*)"height");
  *height = atoi((const char*)_height);

  xmlChar* _bin = xmlGetProp(node, (const xmlChar*)"bin");

  GString* bin = g_string_new ((const gchar*)_bin);

  xmlXPathFreeObject(xpathObj);
  xmlXPathFreeContext(xPathCtx);

  // TODO:
  // - Use 'bin' attr value as mdibin index
  // - Find Thumbnail Chunk and skip everything else
  // - Find Chunk and zlib decompress if necessary
  // - Use gegl to convert BGRA to RGBA
  // ... profit?


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
