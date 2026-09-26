const EventEmitter = require('bare-events')
const binding = require('../binding')
const registry = require('bare-foundation-registry')

module.exports = exports = class WebKitWebView extends EventEmitter {
  constructor(opts = {}) {
    super()

    const { x = 0, y = 0, width = 0, height = 0 } = opts

    this._tag = binding.webViewInit(x, y, width, height, this)

    this._token = binding.claim(this._tag, this)
  }

  get [registry.tag]() {
    return this._tag
  }

  get [registry.handle]() {
    return binding.handle(this._tag)
  }

  get frame() {
    return binding.webViewFrame(this._tag)
  }

  set frame(frame) {
    const { x = 0, y = 0, width = 0, height = 0 } = frame

    binding.webViewFrame(this._tag, x, y, width, height)
  }

  get inspectable() {
    return binding.webViewInspectable(this._tag)
  }

  set inspectable(value) {
    binding.webViewInspectable(this._tag, value)
  }

  removeFromSuperview() {
    binding.webViewRemoveFromSuperview(this._tag)
    return this
  }

  loadRequest(url) {
    binding.webViewLoadRequest(this._tag, url)
    return this
  }

  loadHTMLString(html, baseURL = 'about:blank') {
    binding.webViewLoadHTMLString(this._tag, html, baseURL)
    return this
  }

  reload() {
    binding.webViewReload(this._tag)
    return this
  }

  reloadFromOrigin() {
    binding.webViewReloadFromOrigin(this._tag)
    return this
  }

  stopLoading() {
    binding.webViewStopLoading(this._tag)
    return this
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: WebKitWebView }
    }
  }
}
