/* altkbuilder.c -
 */

#include "altk/private.h"
#include "altk/altkbuilder.h"
#include "altk/altkcontainer.h" /* [removeme] */
#include "altk/altkbuilder.inl"



typedef enum _ElemType ElemType;
typedef union _Elem Elem;
typedef struct _ElemAny ElemAny;
typedef struct _ElemUnknown ElemUnknown;
typedef struct _ElemRoot ElemRoot;
typedef struct _ElemObject ElemObject;
typedef struct _ElemChild ElemChild;
typedef struct _ElemProperty ElemProperty;
typedef struct _ElemValue ElemValue;

#define ELEM_HEADER \
  ElemType type; \
  Elem *parent; \
  gchar *name



/* ElemType:
 */
enum _ElemType
  {
    ELEM_ROOT,
    ELEM_UNKNOWN,
    ELEM_OBJECT,
    ELEM_CHILD,
    ELEM_PROPERTY,
    ELEM_VALUE,
  };



/* ElemAny:
 */
struct _ElemAny
{
  ELEM_HEADER;
};



/* ElemUnknown:
 */
struct _ElemUnknown
{
  ELEM_HEADER;
};



/* ElemRoot:
 */
struct _ElemRoot
{
  ELEM_HEADER;

  GList *objects;
};



/* ElemObject:
 */
struct _ElemObject
{
  ELEM_HEADER;

  LObject *instance;
  gchar *id;
  gchar *obj_type;
  GList *properties;
  GList *children;
};



/* ElemChild:
 */
struct _ElemChild
{
  ELEM_HEADER;

  Elem *object;
};



/* ElemProperty:
 */
struct _ElemProperty
{
  ELEM_HEADER;

  Elem *value;
};



/* ElemValue:
 */
struct _ElemValue
{
  ELEM_HEADER;
};



/* Elem:
 */
union _Elem
{
  ElemType type;
  ElemAny any;
  ElemRoot root;
  ElemObject object;
  ElemChild child;
  ElemProperty property;
};



static Elem *elem_push ( AltkBuilder *builder,
                         ElemType type,
                         const gchar *name );
static Elem *elem_peek ( AltkBuilder *builder );
static Elem *elem_pop ( AltkBuilder *builder );
static LObject *elem_instantiate ( Elem *elem );



/* AltkBuilderPrivate:
 */
typedef struct _AltkBuilderPrivate
{
  GMarkupParseContext *context;
  Elem *elem;
  GHashTable *idmap;
}
  AltkBuilderPrivate;

#define PRIVATE(b) ((AltkBuilderPrivate *)((ALTK_BUILDER(b)->private)))



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
  AltkBuilderPrivate *priv;
  AltkBuilder *b = ALTK_BUILDER(l_object_new(ALTK_CLASS_BUILDER, NULL));
  priv = b->private = g_new0(AltkBuilderPrivate, 1);
  priv->context = g_markup_parse_context_new(&parser, 0, b, NULL);
  priv->idmap = g_hash_table_new(g_str_hash, g_str_equal);
  return b;
}



/* altk_builder_parse_text:
 */
gboolean altk_builder_parse_text ( AltkBuilder *builder,
                                   const gchar *text,
                                   gssize len,
                                   GError **error )
{
  AltkBuilderPrivate *priv = PRIVATE(builder);
  /* Elem *root; */
  /* [FIXME] free first */
  priv->elem = NULL;
  elem_push(builder, ELEM_ROOT, "");
  if (!g_markup_parse_context_parse(priv->context,
                                    text,
                                    len,
                                    error))
    return FALSE;
  if (!g_markup_parse_context_end_parse(priv->context,
                                        error))
    return FALSE;
  return TRUE;
}



/* altk_builder_get_object:
 */
LObject *altk_builder_get_object ( AltkBuilder *builder,
                                   const gchar *name )
{
  AltkBuilderPrivate *priv = PRIVATE(builder);
  Elem *elem;
  if (!(elem = g_hash_table_lookup(priv->idmap, name))) {
    CL_DEBUG("ERROR: id not found: '%s'", name);
    return NULL;
  }
  return l_object_ref(elem_instantiate(elem));
}



/* elem_push:
 */
static Elem *elem_push ( AltkBuilder *builder,
                         ElemType type,
                         const gchar *name )
{
  AltkBuilderPrivate *priv = PRIVATE(builder);
  Elem *elem = g_new0(Elem, 1);
  /* grow the stack */
  elem->type = type;
  elem->any.parent = priv->elem;
  elem->any.name = g_strdup(name);
  priv->elem = elem;
  return elem;
}



/* elem_pop:
 */
static Elem *elem_pop ( AltkBuilder *builder )
{
  AltkBuilderPrivate *priv = PRIVATE(builder);
  Elem *elem = priv->elem;
  ASSERT(elem);
  priv->elem = elem->any.parent;
  return elem;
}



/* elem_peek:
 */
static Elem *elem_peek ( AltkBuilder *builder )
{
  AltkBuilderPrivate *priv = PRIVATE(builder);
  return priv->elem;
}



/* elem_instantiate:
 */
static LObject *elem_instantiate ( Elem *elem )
{
  LObjectClass *cls;
  ElemObject *obj;
  GList *l;
  ASSERT(elem->type == ELEM_OBJECT);
  obj = (ElemObject *) elem;
  if (obj->instance)
    return obj->instance;
  /* find class */
  if (!(cls = l_object_class_from_name(obj->obj_type)))
    CL_ERROR("object class not found: '%s'", obj->obj_type);
  obj->instance = l_object_new(cls, NULL);
  /* create children */
  for (l = obj->children; l; l = l->next)
    {
      Elem *child_elem = l->data;
      LObject *child;
      ASSERT(child_elem->type == ELEM_CHILD);
      child = elem_instantiate(child_elem->child.object);
      /* [FIXME] find some generic child handling */
      ASSERT(ALTK_IS_CONTAINER(obj->instance));
      ASSERT(ALTK_IS_WIDGET(child));
      altk_container_add(ALTK_CONTAINER(obj->instance), ALTK_WIDGET(child));
    }
  return obj->instance;
}



/* get_object_attrs:
 */
static void get_object_attrs ( AltkBuilder *builder,
                               const gchar **names,
                               const gchar **values )
{
  AltkBuilderPrivate *priv = PRIVATE(builder);
  ElemObject *elem = (ElemObject *) elem_peek(builder);
  gint i;
  ASSERT(elem);
  ASSERT(elem->type == ELEM_OBJECT);
  for (i = 0; names[i]; i++)
    {
      if (!strcmp(names[i], "type")) {
        ASSERT(!elem->obj_type);
        elem->obj_type = g_strdup(values[i]);
      } else if (!strcmp(names[i], "id")) {
        ASSERT(!elem->id);
        elem->id = g_strdup(values[i]);
        if (g_hash_table_lookup(priv->idmap, elem->id))
          CL_ERROR("duplicated id: '%s'", elem->id);
        else
          g_hash_table_insert(priv->idmap, elem->id, elem);
      } else {
        CL_DEBUG("unknwon 'object' attrs: '%s' = '%s'", names[i], values[i]);
      }
    }
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
  AltkBuilder *builder = data;
  Elem *elem = elem_peek(builder);
  if (elem->type == ELEM_UNKNOWN)
    {
      CL_DEBUG("ignored element: '%s'", name);
      elem_push(builder, ELEM_UNKNOWN, name);
    }
  else if (!strcmp(name, "object"))
    {
      if (elem->type == ELEM_ROOT)
        {
          elem->root.objects =
            g_list_append(elem->root.objects,
                          elem_push(builder, ELEM_OBJECT, name));
          get_object_attrs(builder, atnames, atvalues);
        }
      else if (elem->type == ELEM_CHILD)
        {
          ASSERT(!elem->child.object);
          elem->child.object = elem_push(builder, ELEM_OBJECT, name);
          get_object_attrs(builder, atnames, atvalues);
        }
      else
        {
          CL_ERROR("unexpected 'object'");
        }
    }
  else if (!strcmp(name, "child"))
    {
      if (elem->type != ELEM_OBJECT)
        CL_ERROR("unexpected child");
      elem->object.children =
        g_list_append(elem->object.children,
                      elem_push(builder, ELEM_CHILD, name));
    }
  else if (!strcmp(name, "property"))
    {
      ASSERT(elem->type == ELEM_OBJECT);
      elem->object.properties =
        g_list_append(elem->object.properties,
                      elem_push(builder, ELEM_PROPERTY, name));
    }
  else if (!strcmp(name, "value"))
    {
      ASSERT(elem->type == ELEM_PROPERTY);
      ASSERT(!elem->property.value);
      elem->property.value = elem_push(builder, ELEM_VALUE, name);
    }
  else
    {
      CL_DEBUG("unknown element: '%s'", name);
      elem_push(builder, ELEM_UNKNOWN, name);
    }
}



/* _end_element:
 */
static void _end_element ( GMarkupParseContext *ctxt,
                           const gchar *name,
                           gpointer data,
                           GError **error )
{
  AltkBuilder *builder = data;
  Elem *elem = elem_peek(builder);
  ASSERT(!strcmp(elem->any.name, name));
  elem_pop(builder);
}



/* _text:
 */
static void _text ( GMarkupParseContext *ctxt,
                    const gchar *text_,
                    gsize len,
                    gpointer data,
                    GError **error )
{
  gchar *text = g_strdup(text_);
  gchar *strip = g_strstrip(text);
  if (strip[0])
    CL_DEBUG("[TODO] text: '%s'", strip);
  g_free(text);
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
