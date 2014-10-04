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

#define ELEM_HEADER \
  ElemType type; \
  Elem *parent; \
  gchar *element_name



/* ElemType:
 */
enum _ElemType
  {
    ELEM_ROOT,
    ELEM_UNKNOWN,
    ELEM_OBJECT,
    ELEM_CHILD,
    ELEM_PROPERTY,
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

  gchar *text;
  gchar *name;
  LObject *value;
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
static LObject *elem_property_instantiate ( Elem *elem,
                                            LParamSpec *pspec );



/* AltkBuilderPrivate:
 */
typedef struct _AltkBuilderPrivate
{
  GMarkupParseContext *context;
  Elem *elem;
  GHashTable *idmap;
  GString *text_buffer;
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



/* altk_builder_quick_parse:
 */
LObject *altk_builder_quick_parse ( const gchar *text,
                                    gssize len,
                                    const gchar *item_id,
                                    GError **error )
{
  AltkBuilder *builder;
  LObject *object = NULL;
  l_trash_push();
  builder = L_TRASH_OBJECT(altk_builder_new());
  if (!altk_builder_parse_text(builder, text, len, error))
    goto end;
  if (!(object = altk_builder_get_object(builder, item_id)))
    goto end;
  end:
    l_trash_pop();
    return object;
}



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
  priv->text_buffer = g_string_new("");
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
  if (len < 0)
    len = strlen(text);
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
  elem->any.element_name = g_strdup(name);
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
  /* set properties ([fixme] should be given to constructor) */
  for (l = obj->properties; l; l = l->next)
    {
      Elem *prop_elem = l->data;
      LObject *value;
      LParamSpec *pspec;
      ASSERT(prop_elem->type == ELEM_PROPERTY);
      if (!(pspec = l_object_class_lookup_property(cls, prop_elem->property.name)))
        CL_ERROR("property not found: %s.%s",
                 l_object_class_name(cls),
                 prop_elem->property.name);
      value = elem_property_instantiate(prop_elem, pspec);
      l_object_set_property(obj->instance, prop_elem->property.name, value);
    }
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



/* elem_property_instantiate:
 */
static LObject *elem_property_instantiate ( Elem *elem,
                                            LParamSpec *pspec )
{
  ASSERT(elem);
  ASSERT(elem->type == ELEM_PROPERTY);
  if (elem->property.value)
    return elem->property.value;
  if (pspec->value_type == L_CLASS_STRING)
    {
      elem->property.value = L_OBJECT
        (l_string_new(elem->property.text));
    }
  else
    {
      CL_ERROR("[TODO] prop type: %s", l_object_class_name(pspec->value_type));
    }
  return elem->property.value;
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



/* get_property_attrs:
 */
static void get_property_attrs ( AltkBuilder *builder,
                               const gchar **names,
                               const gchar **values )
{
  ElemProperty *elem = (ElemProperty *) elem_peek(builder);
  gint i;
  ASSERT(elem);
  ASSERT(elem->type == ELEM_PROPERTY);
  for (i = 0; names[i]; i++)
    {
      if (!strcmp(names[i], "name")) {
        ASSERT(!elem->name);
        elem->name = g_strdup(values[i]);
      } else {
        CL_DEBUG("unknwon 'object' attrs: '%s' = '%s'", names[i], values[i]);
      }
    }
  if (!elem->name)
    CL_ERROR("property name required");
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
      get_property_attrs(builder, atnames, atvalues);
    }
  else
    {
      CL_DEBUG("unknown element: '%s'", name);
      elem_push(builder, ELEM_UNKNOWN, name);
    }
}



/* _split_text:
 */
static gchar *_split_text ( gchar *text )
{
  static const gchar *delim = " \t\n\f\r";
  gchar *start, *end, *c;
  gint ofs, inspc;
  /* strip leading whites */
  for (start = text; (*start) && strchr(delim, *start); start++);
  if (!(*start)) return NULL;
  /* split words */
  ofs = 0;
  inspc = FALSE;
  end = start;
  for (c = start; *c; c++)
    {
      if (strchr(delim, *c))
        {
          if (inspc)
            {
              ofs++;
            }
          else
            {
              *(c-ofs) = ' ';
              inspc = TRUE;
            }
        }
      else
        {
          inspc = FALSE;
          end = c;
          if (ofs > 0)
            *(c-ofs) = *c;
        }
    }
  *(end-ofs+1) = 0;
  return start;
}



/* _end_element:
 */
static void _end_element ( GMarkupParseContext *ctxt,
                           const gchar *name,
                           gpointer data,
                           GError **error )
{
  AltkBuilder *builder = data;
  AltkBuilderPrivate *priv = PRIVATE(builder);
  Elem *elem = elem_peek(builder);
  ASSERT(!strcmp(elem->any.element_name, name));
  if (priv->text_buffer->len > 0)
    {
      gchar *text;
      /* just in case ? */
      ASSERT(!memchr(priv->text_buffer->str, 0, priv->text_buffer->len));
      text = _split_text(priv->text_buffer->str);
      if (text)
        {
          ASSERT(elem->type == ELEM_PROPERTY);
          ASSERT(!elem->property.text);
          elem->property.text = g_strdup(text);
        }
      g_string_truncate(priv->text_buffer, 0);
    }
  elem_pop(builder);
}



/* _text:
 */
static void _text ( GMarkupParseContext *ctxt,
                    const gchar *text,
                    gsize len,
                    gpointer data,
                    GError **error )
{
  AltkBuilder *builder = data;
  AltkBuilderPrivate *priv = PRIVATE(builder);
  g_string_append_len(priv->text_buffer, text, len);
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
