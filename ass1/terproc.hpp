#ifndef TERGEN_TERPROC_H
#define TERGEN_TERPROC_H
/* terproc: terrain processing functions */
#include <string>
#include "tertype.h"

/* reads a file specified by filename and parses the data
 * into a terrain struct.
 */
struct terrain ter_read(std::string filename);

/* deallocates any memory used my ter */
void ter_destroy(struct terrain& ter);

#endif
