#include <glib.h>
#include <gio/gio.h>
#include <libgimp/gimp.h>

#include "file-mdp.h"

#ifndef __MDP_LOAD_H__
#define __MDP_LOAD_H__

GimpImage * load_image  (GFile        *file,
                         gboolean      interactive,
                         GError      **error);

#endif /* __MDP_LOAD_H__ */
