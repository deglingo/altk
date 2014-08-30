/* private.h - private stuff
 */

#ifndef _PRIVATE_H_
#define _PRIVATE_H_

#include <clog.h>

/* ASSERT */
#define ASSERT(expr) do {                           \
    if (!(expr)) {                                  \
      CL_ERROR("ASSERTION FAILED: `" #expr "'");    \
    }                                               \
  } while (0)

#endif
