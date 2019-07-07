#include <string.h>
#include <stdlib.h>

#include "buffer.h"

struct buffer_d {
  size_t size;
  char *ptr;
  struct buffer_d *next;
};

struct buffer {
  int size;
  struct buffer_d *start;
  struct buffer_d *last;
};

static void
buffer_d_free(struct buffer_d *cur) {
  if (cur->size > 0) {
    free(cur->ptr);
  }

  if (NULL != cur->next) {
    buffer_d_free(cur->next);
  }

  free(cur);
}

static void
buffer_free(void *p) {
  struct buffer *ptr = p;

  if (NULL != ptr->start) {
    buffer_d_free(ptr->start);
  }

  free(ptr);
}

struct buffer_d *
buffer_d_alloc(const char *content, int size) {
  struct buffer_d *ptr;

  ptr = malloc(sizeof(struct buffer_d));
  ptr->size = size;
  if (size > 0) {
    ptr->ptr  = malloc(size);
    memcpy(ptr->ptr, content, size);
  } else {
    ptr->ptr  = NULL;
  }
  ptr->next = NULL;

  return ptr;
}

static VALUE
buffer_alloc(VALUE klass) {
  struct buffer *ptr;

  return Data_Make_Struct(klass, struct buffer, NULL, buffer_free, ptr);
}

static VALUE
buffer_init(VALUE self, VALUE val) {
  struct buffer *ptr;
  struct buffer_d *start;

  Check_Type(val, T_STRING);

  char *_str = StringValueCStr(val);
  int len = strlen(_str);

  Data_Get_Struct(self, struct buffer, ptr);

  start = buffer_d_alloc(_str, len);

  ptr->size = len;
  ptr->start = ptr->last = start;

  return self;
}

static VALUE
buffer_release(VALUE self) {
  struct buffer *ptr;

  Data_Get_Struct(self, struct buffer, ptr);

  if (0 == ptr->size) {
    return self;
  }

  buffer_d_free(ptr->start);

  ptr->size = 0;
  ptr->start = NULL;
  ptr->last = NULL;

  return self;
}

static VALUE
buffer_append(VALUE self, VALUE val) {
  struct buffer *ptr;
  struct buffer_d *next;

  Check_Type(val, T_STRING);

  Data_Get_Struct(self, struct buffer, ptr);

  char *_str = StringValueCStr(val);
  int len = strlen(_str);

  next = buffer_d_alloc(_str, len);

  if (NULL == ptr->start) {
    ptr->start = next;
  }
  if (NULL != ptr->last) {
    ptr->last->next = next;
  }
  ptr->last = next;
  ptr->size += len;

  return self;
}

static VALUE
buffer_append_buffer(VALUE self, VALUE val) {
  struct buffer *ptr;
  struct buffer *target;
  struct buffer_d *cur;
  struct buffer_d *next;

  // TODO: Check type
  /* cAview = rb_const_get(rb_cObject, rb_intern("AView")); */
  /* cBuffer = rb_const_get(cAview, rb_intern("Buffer")); */
  /*  */
  /* Check_Type(val, cBuffer); */

  Data_Get_Struct(self, struct buffer, ptr);
  Data_Get_Struct(val, struct buffer, target);

  ptr->size = ptr->size + target->size;

  for (cur = target->start; cur; cur = cur->next) {
    next = buffer_d_alloc(cur->ptr, cur->size);
    ptr->last->next = next;
    ptr->last = next;
  }

  return self;
}


static VALUE
buffer_to_s(VALUE self) {
  struct buffer *ptr;
  struct buffer_d *cur;
  VALUE result = Qnil;
  char *out;
  int written = 0;

  Data_Get_Struct(self, struct buffer, ptr);

  if (ptr->size == 0) {
    return rb_str_new("", 0);
  }

  out = malloc(ptr->size);

  for (cur = ptr->start; cur; cur = cur->next) {
    memcpy(out + written, cur->ptr, cur->size);
    written += cur->size;
  }

  result = rb_str_new(out, ptr->size);
  free(out);

  return result;
}

void
Init_aview_buffer(VALUE parent) {
  VALUE cBuffer = rb_define_class_under(parent, "Buffer", rb_cObject);

  rb_define_alloc_func(cBuffer, buffer_alloc);
  rb_define_method(cBuffer, "initialize", buffer_init, 1);
  rb_define_method(cBuffer, "free", buffer_release, 0);
  rb_define_method(cBuffer, "to_s", buffer_to_s, 0);
  rb_define_method(cBuffer, "append", buffer_append, 1);
  rb_define_method(cBuffer, "append_buffer", buffer_append_buffer, 1);
}
