#include "mdp-parse.h"
#include "mdp-util.h"

gint
mdp_read_header_block (MDPimage       *img_a,
                       GInputStream   *input,
                       GError        **error)
{
  // TODO: Look into using g_input_stream_read_bytes
  gchar    sig[8];

  if (mdp_read (input, sig,                8, error) < 8 ||
      mdp_read (input, &img_a->version,    4, error) < 4 ||
      mdp_read (input, &img_a->mdiSize,    4, error) < 4 ||
      mdp_read (input, &img_a->mdibinSize, 4, error) < 4)
    {
      mdp_set_error (error);
      return -1;
    }

  img_a->version = GUINT32_FROM_LE (img_a->version);
  img_a->mdiSize = GUINT32_FROM_LE (img_a->mdiSize);
  img_a->mdibinSize = GUINT32_FROM_LE (img_a->mdibinSize);

  if (memcmp (sig, "mdipack\0", 8) != 0)
    {
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                  "Not a valid MDIPack document file");
      return -1;
    }

  if (img_a->version != 0)
    {
      g_set_error (error, G_FILE_ERROR, G_FILE_ERROR_FAILED,
                  "Unsupported file format version: %d", img_a->version);
      return -1;
    }

  return 0;
}

gint
mdp_read_mdi_block (MDPimage       *img_a,
                    GInputStream   *input,
                    GError        **error)
{
  // TODO: Implement Zero(ish) Copy
  // - Customize rsvg_create_xml_parser_from_stream
  // - Add explicit max size

  gpointer _mdi = g_malloc (img_a->mdiSize);

  if (mdp_read (input, _mdi, img_a->mdiSize, error) < img_a->mdiSize)
    {
      mdp_set_error (error);
      g_free (_mdi);
      return -1;
    }

  img_a->mdi = xmlReadDoc(_mdi, "mdi.xml", NULL, 0);

  // Unclear if it's ok to free the data after xmlReadDoc
  // If it isn't, then this is probably what broke things
  g_free (_mdi);

  return 0;
}

gint
mdp_read_mdibin_block (MDPimage       *img_a,
                       GInputStream   *input,
                       GError        **error)
{
  GInputStream* _mdibin_i;
  guint32 mdibinSize = img_a->mdibinSize;

  gpointer _mdibin = g_malloc (mdibinSize);

  if (mdp_read (input, _mdibin, mdibinSize, error) < mdibinSize)
    {
      mdp_set_error (error);
      g_free (_mdibin);
      return -1;
    }

  _mdibin_i = g_memory_input_stream_new_from_data (_mdibin,
                                                   mdibinSize,
                                                   NULL);

  // We no longer need to free _mdibin... I think?
  _mdibin = NULL;

  img_a->mdibin = g_hash_table_new (g_string_hash, g_string_equal);

  // TODO: read piece by piece

  g_object_unref (_mdibin_i);

  return 0;
}
