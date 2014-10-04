/* ex_label.c -
 */

#include "altk/private.h"
#include "altk/altk.h"



/* main:
 */
gint main ()
{
  AltkSizeDistrib distrib;
  gint sfact;
  GString *str = g_string_new("");
  const gchar *sep = "----------------------------------------------------------------------\n";
  fprintf(stderr, sep);
  for (sfact = 0; sfact <= 10; sfact++)
    {
      gint size = sfact * 10;
      gint n_items, i;
      for (n_items = 0; n_items <= 10; n_items++)
        {
          g_string_truncate(str, 0);
          altk_size_distrib_init(&distrib, size, n_items);
          g_string_append_printf(str, " | %3d %2d | ", size, n_items);
          for (i = 0; i <= 10; i++)
            {
              gint item_size = altk_size_distrib_next(&distrib);
              g_string_append_printf(str, "%s%3d", (i == 0 ? "" : " : "), item_size);
            }
          g_string_append_c(str, '\n');
          fprintf(stderr, "%s", str->str);
        }
      fprintf(stderr, sep);
    }
  return 0;
}
