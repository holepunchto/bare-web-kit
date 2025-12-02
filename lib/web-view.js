const EventEmitter = require('bare-events')
const binding = require('../binding')

module.exports = exports = class WebKitWebView extends EventEmitter {
  constructor(opts = {}) {
    super()

    const { x = 0, y = 0, width = 0, height = 0 } = opts

    this._handle = binding.webViewInit(x, y, width, height, this)
  }

  loadHTMLString(html, baseURL = 'about:blank') {
    binding.webViewLoadHTMLString(this._handle, html, baseURL)
    return this
  }

  reload() {
    binding.webViewReload(this._handle)
    return this
  }

  reloadFromOrigin() {
    binding.webViewReloadFromOrigin(this._handle)
    return this
  }

  stopLoading() {
    binding.webViewStopLoading(this._handle)
    return this
  }

  [Symbol.for('bare.inspect')]() {
    return {
      __proto__: { constructor: WebKitWebView }
    }
  }
}
