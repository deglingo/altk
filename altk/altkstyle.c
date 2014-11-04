/* altkstyle.c -
 */

#include "altk/private.h"
#include "altk/altkstyle.h"
#include "altk/altkdrawable.h"
#include "altk/altkgc.h"

#include "altk/altkstyle.inl"



/* altk_style_new:
 */
AltkStyle *altk_style_new ( void )
{
  /* [FIXME] */
  static AltkStyle *style = NULL;
  if (!style)
    style = ALTK_STYLE(l_object_new(ALTK_CLASS_STYLE, NULL));
  return l_object_ref(style);
}
