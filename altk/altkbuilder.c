/* altkbuilder.c -
 */

#include "altk/private.h"
#include "altk/altkbuilder.h"
#include "altk/altkbuilder.inl"



static void _start_element ( GMarkupParseContext *ctxt,
                             const gchar *name,
                             const gchar **atnames,
                             const gchar **atvalues,
                             gpointer data,
                             GError **error );
static void _end_element ( GMarkupParseContext *ctxt,
                           const gchar *name,
                           gpointer data,
                           GError **error );
static void _text ( GMarkupParseContext *ctxt,
                    const gchar *text,
                    gsize len,
                    gpointer data,
                    GError **error );
static void _passthrough ( GMarkupParseContext *ctxt,
                           const gchar *text,
                           gsize len,
                           gpointer data,
                           GError **error );
static void _error ( GMarkupParseContext *ctxt,
                     GError *error,
                     gpointer data );



/* altk_builder_new:
 */
AltkBuilder *altk_builder_new ( void )
{
  static GMarkupParser parser =
    {
      _start_element,
      _end_element,
      _text,
      _passthrough,
      _error,
    };
  AltkBuilder *b = ALTK_BUILDER(l_object_new(ALTK_CLASS_BUILDER, NULL));
  b->context = g_markup_parse_context_new(&parser, 0, b, NULL);
  return b;
}



/* altk_builder_parse_text:
 */
gboolean altk_builder_parse_text ( AltkBuilder *builder,
                                   const gchar *text,
                                   GError **error )
{
  return TRUE;
}



/* [REMOVEME] */
#include "altk/altkdialog.h"

/* altk_builder_get_object:
 */
LObject *altk_builder_get_object ( AltkBuilder *builder,
                                   const gchar *name )
{
  return L_OBJECT(altk_dialog_new(NULL));
}



/* _start_element:
 */
static void _start_element ( GMarkupParseContext *ctxt,
                             const gchar *name,
                             const gchar **atnames,
                             const gchar **atvalues,
                             gpointer data,
                             GError **error )
{
  CL_ERROR("[TODO] %s", G_STRFUNC);
}



/* _end_element:
 */
static void _end_element ( GMarkupParseContext *ctxt,
                           const gchar *name,
                           gpointer data,
                           GError **error )
{
  CL_ERROR("[TODO] %s", G_STRFUNC);
}



/* _text:
 */
static void _text ( GMarkupParseContext *ctxt,
                    const gchar *text,
                    gsize len,
                    gpointer data,
                    GError **error )
{
  CL_ERROR("[TODO] %s", G_STRFUNC);
}



/* _passthrough:
 */
static void _passthrough ( GMarkupParseContext *ctxt,
                           const gchar *text,
                           gsize len,
                           gpointer data,
                           GError **error )
{
  CL_ERROR("[TODO] %s", G_STRFUNC);
}



/* _error:
 */
static void _error ( GMarkupParseContext *ctxt,
                     GError *error,
                     gpointer data )
{
  CL_ERROR("[TODO] %s", G_STRFUNC);
}
