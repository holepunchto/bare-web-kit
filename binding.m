#import <assert.h>
#import <bare.h>
#import <js.h>
#import <utf.h>

#import <WebKit/WebKit.h>

#import "lib/web-view.h"

static js_value_t *
bare_web_kit_exports(js_env_t *env, js_value_t *exports) {
  int err;

  bare_foundation_registry_t *registry = bare_foundation_registry_create(env, exports);

#define V(name, fn) \
  { \
    js_value_t *val; \
    err = js_create_function(env, name, -1, fn, registry, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }

  V("claim", bare_foundation_claim)
  V("wrapper", bare_foundation_wrapper)
  V("registrySize", bare_foundation_registry_size)
  V("handle", bare_foundation_handle)
  V("adopt", bare_foundation_adopt)

  V("webViewInit", bare_web_kit_web_view_init)
  V("webViewInspectable", bare_web_kit_web_view_inspectable)
  V("webViewFrame", bare_web_kit_web_view_frame)
  V("webViewRemoveFromSuperview", bare_web_kit_web_view_remove_from_superview)
  V("webViewLoadRequest", bare_web_kit_web_view_load_request)
  V("webViewLoadHTMLString", bare_web_kit_web_view_load_html_string)
  V("webViewReload", bare_web_kit_web_view_reload)
  V("webViewReloadFromOrigin", bare_web_kit_web_view_reload_from_origin)
  V("webViewStopLoading", bare_web_kit_web_view_stop_loading)
#undef V

#define V(name, n) \
  { \
    js_value_t *val; \
    err = js_create_int32(env, n, &val); \
    assert(err == 0); \
    err = js_set_named_property(env, exports, name, val); \
    assert(err == 0); \
  }
#undef V

  return exports;
}

BARE_MODULE(bare_web_kit, bare_web_kit_exports)
