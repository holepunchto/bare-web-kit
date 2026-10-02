#import <assert.h>
#import <js.h>
#import <utf.h>

#import <Foundation/Foundation.h>

#import "registry.h"

static inline NSString *
bare_web_kit__to_string_no_copy(utf8_t *string, size_t len) {
  return [[[NSString alloc] initWithBytesNoCopy:string
                                         length:len
                                       encoding:NSUTF8StringEncoding
                                   freeWhenDone:YES] autorelease];
}

static inline NSURL *
bare_web_kit__to_url_no_copy(utf8_t *string, size_t len) {
  return [NSURL URLWithString:bare_web_kit__to_string_no_copy(string, len)];
}

static bool
bare_web_kit__read_double(js_env_t *env, js_value_t *value, const char *name, double *result) {
  int err;

  js_value_type_t type;
  err = js_typeof(env, value, &type);
  assert(err == 0);

  if (type != js_number) {
    err = js_throw_type_errorf(env, NULL, "Expected a number for '%s'", name);
    assert(err == 0);

    return false;
  }

  err = js_get_value_double(env, value, result);
  assert(err == 0);

  return true;
}

static js_value_t *
bare_web_kit__from_rect(js_env_t *env, CGRect rect) {
  int err;

  js_value_t *result;
  err = js_create_object(env, &result);
  assert(err == 0);

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_double(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, result, name, val); \
    assert(err == 0); \
  }

  V("x", rect.origin.x)
  V("y", rect.origin.y)
  V("width", rect.size.width)
  V("height", rect.size.height)
#undef V

  return result;
}
