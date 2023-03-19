#include <glib.h>
#include <gio/gio.h>

#include "file-mdp.h"

#ifndef __MDP_PARSE_H__
#define __MDP_PARSE_H__

gint                    mdp_read_header_block  (MDPimage       *img_a,
                                                GInputStream   *input,
                                                GError        **error);

gint                    mdp_read_mdi_block     (MDPimage       *img_a,
                                                GInputStream   *input,
                                                GError        **error);

gint                    mdp_read_mdibin_block  (MDPimage       *img_a,
                                                GInputStream   *input,
                                                GError        **error);

#endif /* __MDP_PARSE_H__ */
