import EventEmitter from 'bare-events'
import { tag, handle, Handle } from 'bare-foundation-registry'

/**
 * A view that shows web content with WebKit, as a `WKWebView`. Add it to a view from another
 * addon, such as `bare-app-kit` or `bare-ui-kit`, to show it.
 */
interface WebKitWebView extends EventEmitter {
  /** The position and size of the view in its superview. Missing fields are 0. */
  get frame(): WebKitWebView.Rect
  set frame(frame: Partial<WebKitWebView.Rect>)

  /**
   * Whether Safari's Web Inspector can inspect the view. Needs macOS 13.3 or iOS 16.4. Before
   * that, it reads as `false` and setting it does nothing.
   */
  inspectable: boolean

  /** Remove the view from its superview. */
  removeFromSuperview(): this

  /** Load `url`. */
  loadRequest(url: string): this

  /**
   * Load `html` as the page. Relative links resolve against `baseURL`, which defaults to
   * `about:blank`.
   */
  loadHTMLString(html: string, baseURL?: string): this

  /** Load the current page again. */
  reload(): this

  /** Load the current page again, without using the cache. */
  reloadFromOrigin(): this

  /** Stop loading the current page. */
  stopLoading(): this

  readonly [tag]: number

  readonly [handle]: Handle
}

declare class WebKitWebView {
  /** Create a web view with `frame`. Missing fields are 0. */
  constructor(frame?: Partial<WebKitWebView.Rect>)
}

declare namespace WebKitWebView {
  export interface Rect {
    x: number
    y: number
    width: number
    height: number
  }
}

export = WebKitWebView
