#include <glib.h>
#include <gio/gio.h>

#include "file-mdp.h"

#ifndef __MDP_UTIL_H__
#define __MDP_UTIL_H__

#define MDP_TELL(f)     g_seekable_tell (G_SEEKABLE (input))

/*
 *  Set file read error
 */
void                    mdp_set_error          (GError             **error);

gint                    mdp_read               (GInputStream        *input,
                                                gconstpointer        data,
                                                gint                 count,
                                                GError             **error);

gboolean                mdp_seek               (GInputStream        *input,
                                                goffset              offset,
                                                GSeekType            type,
                                                GError             **error);

#endif /* __MDP_UTIL_H__ */
