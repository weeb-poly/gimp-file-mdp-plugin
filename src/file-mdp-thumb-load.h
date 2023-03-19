#include <glib.h>
#include <gio/gio.h>
#include <libgimp/gimp.h>

#include "file-mdp.h"

#ifndef __MDP_THUMB_LOAD_H__
#define __MDP_THUMB_LOAD_H__

GimpImage * load_thumbnail_image (GFile   *file,
                                  gint    *width,
                                  gint    *height,
                                  GError **error);

#endif /* __MDP_THUMB_LOAD_H__ */
