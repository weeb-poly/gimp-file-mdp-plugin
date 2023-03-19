#include "mdp-util.h"

/* Utility function */
void
mdp_set_error (GError  **error)
{
  if (! error || ! *error)
    g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                 "Error reading data. Most likely unexpected end of file.");

  return;
}

gint
mdp_read (GInputStream  *input,
          gconstpointer  data,
          gint           count,
          GError       **error)
{
  gsize bytes_read = 0;

  /* we allow for 'data == NULL && count == 0', which g_input_stream_read_all()
   * rejects.
   */
  if (count > 0)
    {
      /* We consider reading less bytes than we want an error. */
      if (g_input_stream_read_all (input, (void *) data, count,
                                   &bytes_read, NULL, error) &&
          bytes_read < count)
        g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                    "Unexpected end of file");
    }

  return bytes_read;
}

gboolean
mdp_seek (GInputStream  *input,
          goffset        offset,
          GSeekType      type,
          GError       **error)
{
  return g_seekable_seek (G_SEEKABLE (input),
                          offset, type,
                          NULL, error);
}

