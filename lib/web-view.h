#import <assert.h>
#import <bare.h>
#import <js.h>

#import <WebKit/WebKit.h>

#import "bridging.h"

@interface BareWebView : WKWebView <WKUIDelegate, WKScriptMessageHandler> {
@public
  js_env_t *env;
  js_ref_t *ctx;
  js_ref_t *on_message;
}

@end

@implementation BareWebView

- (void)dealloc {
  int err;

  err = js_delete_reference(env, on_message);
  assert(err == 0);

  err = js_delete_reference(env, ctx);
  assert(err == 0);

  [super dealloc];
}

- (void)userContentController:(WKUserContentController *)userContentController
      didReceiveScriptMessage:(WKScriptMessage *)message {
  int err;

  // PROTOTYPE: the question this branch exists to answer - which thread WebKit
  // hands us, and whether entering JS from here is safe on the Bare loop.
  fprintf(stderr, "[handler] main thread: %s\n", [NSThread isMainThread] ? "yes" : "no");

  js_handle_scope_t *scope;
  err = js_open_handle_scope(env, &scope);
  assert(err == 0);

  js_value_t *receiver;
  err = js_get_reference_value(env, ctx, &receiver);
  assert(err == 0);

  js_value_t *callback;
  err = js_get_reference_value(env, on_message, &callback);
  assert(err == 0);

  NSString *body = [message.body isKindOfClass:[NSString class]] ? message.body : [message.body description];

  js_value_t *argv[1];
  err = js_create_string_utf8(env, (const utf8_t *) [body UTF8String], -1, &argv[0]);
  assert(err == 0);

  err = js_call_function(env, receiver, callback, 1, argv, NULL);
  (void) err;

  err = js_close_handle_scope(env, scope);
  assert(err == 0);
}

@end

static js_value_t *
bare_web_kit_web_view_init(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 6;
  js_value_t *argv[6];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 6);

  double x;
  err = js_get_value_double(env, argv[0], &x);
  assert(err == 0);

  double y;
  err = js_get_value_double(env, argv[1], &y);
  assert(err == 0);

  double width;
  err = js_get_value_double(env, argv[2], &width);
  assert(err == 0);

  double height;
  err = js_get_value_double(env, argv[3], &height);
  assert(err == 0);

  js_value_t *result;

  @autoreleasepool {
    WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];

    BareWebView *handle = [[BareWebView alloc]
      initWithFrame:CGRectMake(x, y, width, height)
      configuration:configuration];

    handle.UIDelegate = handle;

    // PROTOTYPE: one hardcoded handler name, no frame or origin filtering, and
    // the content controller retains us - all of which the real contract has to
    // deal with. Enough to answer the threading question.
    [configuration.userContentController addScriptMessageHandler:handle name:@"bare"];

    err = js_create_external(env, (void *) CFBridgingRetain(handle), bare_web_kit__on_bridged_release, NULL, &result);
    assert(err == 0);

    handle->env = env;

    err = js_create_reference(env, argv[4], 1, &handle->ctx);
    assert(err == 0);

    err = js_create_reference(env, argv[5], 1, &handle->on_message);
    assert(err == 0);
  }

  return result;
}

static js_value_t *
bare_web_kit_web_view_inspectable(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1 || argc == 2);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  js_value_t *result = NULL;

  @autoreleasepool {
    BareWebView *web_view = (__bridge BareWebView *) handle;

    if (argc == 1) {
      bool inspectable = false;

      if ([web_view respondsToSelector:@selector(isInspectable)]) {
        inspectable = web_view.inspectable;
      }

      err = js_get_boolean(env, inspectable, &result);
      assert(err == 0);
    } else {
      bool inspectable;
      err = js_get_value_bool(env, argv[1], &inspectable);
      assert(err == 0);

      if ([web_view respondsToSelector:@selector(setInspectable:)]) {
        web_view.inspectable = inspectable;
      }
    }
  }

  return result;
}

static js_value_t *
bare_web_kit_web_view_load_request(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 2;
  js_value_t *argv[2];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 2);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  size_t url_len;
  err = js_get_value_string_utf8(env, argv[1], NULL, 0, &url_len);
  assert(err == 0);

  utf8_t *url = malloc(url_len);
  err = js_get_value_string_utf8(env, argv[1], url, url_len, NULL);
  assert(err == 0);

  @autoreleasepool {
    BareWebView *web_view = (__bridge BareWebView *) handle;

    [web_view loadRequest:[[NSURLRequest alloc] initWithURL:bare_web_kit__to_url_no_copy(url, url_len)]];
  }

  return NULL;
}

static js_value_t *
bare_web_kit_web_view_load_html_string(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 3;
  js_value_t *argv[3];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 3);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  size_t html_len;
  err = js_get_value_string_utf8(env, argv[1], NULL, 0, &html_len);
  assert(err == 0);

  utf8_t *html = malloc(html_len);
  err = js_get_value_string_utf8(env, argv[1], html, html_len, NULL);
  assert(err == 0);

  size_t base_url_len;
  err = js_get_value_string_utf8(env, argv[2], NULL, 0, &base_url_len);
  assert(err == 0);

  utf8_t *base_url = malloc(base_url_len);
  err = js_get_value_string_utf8(env, argv[2], base_url, base_url_len, NULL);
  assert(err == 0);

  @autoreleasepool {
    BareWebView *web_view = (__bridge BareWebView *) handle;

    [web_view loadHTMLString:bare_web_kit__to_string_no_copy(html, html_len)
                     baseURL:bare_web_kit__to_url_no_copy(base_url, base_url_len)];
  }

  return NULL;
}

static js_value_t *
bare_web_kit_web_view_reload(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  @autoreleasepool {
    BareWebView *web_view = (__bridge BareWebView *) handle;

    [web_view reload];
  }

  return NULL;
}

static js_value_t *
bare_web_kit_web_view_reload_from_origin(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  @autoreleasepool {
    BareWebView *web_view = (__bridge BareWebView *) handle;

    [web_view reloadFromOrigin];
  }

  return NULL;
}

static js_value_t *
bare_web_kit_web_view_stop_loading(js_env_t *env, js_callback_info_t *info) {
  int err;

  size_t argc = 1;
  js_value_t *argv[1];

  err = js_get_callback_info(env, info, &argc, argv, NULL, NULL);
  assert(err == 0);

  assert(argc == 1);

  void *handle;
  err = js_get_value_external(env, argv[0], &handle);
  assert(err == 0);

  @autoreleasepool {
    BareWebView *web_view = (__bridge BareWebView *) handle;

    [web_view stopLoading];
  }

  return NULL;
}
