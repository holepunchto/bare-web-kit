#import <js.h>
#import <utf.h>

#import <Foundation/Foundation.h>

static void
bare_web_kit__on_bridged_release(js_env_t *env, void *data, void *finalize_hint) {
  CFBridgingRelease(data);
}

static inline NSString *
bare_web_kit__to_string_no_copy(utf8_t *string, size_t len) {
  return [[NSString alloc] initWithBytesNoCopy:string
                                        length:len
                                      encoding:NSUTF8StringEncoding
                                  freeWhenDone:YES];
}

static inline NSURL *
bare_web_kit__to_url_no_copy(utf8_t *string, size_t len) {
  return [[NSURL alloc] initWithString:bare_web_kit__to_string_no_copy(string, len)];
}
