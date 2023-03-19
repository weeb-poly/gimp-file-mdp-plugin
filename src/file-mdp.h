#include <glib.h>
#include <libxml/tree.h>

#ifndef __MDP_H__
#define __MDP_H__

#define LOAD_PROC "file-mdp-load"
#define LOAD_THUMB_PROC "file-mdp-load-thumb"
#define PLUG_IN_BINARY "file-mdp"
#define PLUG_IN_ROLE "gimp-file-mdp"

/* MDP Chunk structure */
#define PAC_MAX_NAME 64
#define PAC_RESERVED 48
typedef struct
{
  guint32 chunkSize;
  guint32 streamType;
  guint32 streamSize;
  guint32 outSize;
  GString* archiveName;                 /* Max Length is PAC_MAX_NAME-1 (null terminator) */
  GBytes* streamData;                   /* May be Compressed */
  GBytes* chunkData;                    /* Decompressed */
} MDPchunk;


/* MDP File data structures */
typedef struct
{
  // From Header
  guint32 version;
  guint32 mdiSize;
  guint32 mdibinSize;

  // MDI Section
  xmlDocPtr mdi;

  // MDIBIN Section
  GHashTable* mdibin;
} MDPimage;


#endif /* __MDP_H__ */
