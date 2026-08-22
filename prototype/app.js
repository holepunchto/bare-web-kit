// Throwaway. One question: when WebKit delivers a script message, can the
// handler enter JS directly on the Bare loop, or does it need a hop?
const fs = require('bare-fs')

const { Window } = require('bare-app-kit')
const { WebView } = require('bare-web-kit')

const LOG = '/tmp/webview-bridge-prototype.log'
const notes = []

function note(line) {
  notes.push(line)
  console.error(line)
  fs.writeFileSync(LOG, notes.join('\n') + '\n')
}

note('[js] start')

const window = new Window({ x: 100, y: 100, width: 480, height: 320, styleMask: Window.STYLE_MASK.TITLED | Window.STYLE_MASK.CLOSABLE | Window.STYLE_MASK.RESIZABLE })
const webView = new WebView({ x: 0, y: 0, width: 480, height: 320 })

webView.on('message', (data) => {
  note(`[js] handler entered, data: ${JSON.stringify(data)}`)

  // Is the event loop live from inside a WebKit callback?
  queueMicrotask(() => note('[js] microtask from inside the handler ran'))
  setTimeout(() => note('[js] timer from inside the handler ran'), 50)

  // Reentrancy: call back into the WebView from inside its own callback.
  try {
    webView.loadHTMLString('<!doctype html><title>reentered</title><h1>reentered</h1>')
    note('[js] reentrant loadHTMLString returned')
  } catch (err) {
    note(`[js] reentrant call threw: ${err.message}`)
  }

  setTimeout(() => {
    note('[js] done')
    Bare.exit()
  }, 500)
})

window.contentView = webView
window.makeKeyWindow().orderFront().center()

webView.loadHTMLString(`<!doctype html>
<title>prototype</title>
<script>
  window.webkit.messageHandlers.bare.postMessage('hello from the page')
</script>
<h1>posted</h1>`)

note('[js] loaded, waiting for the page to post')

setTimeout(() => {
  note('[js] TIMEOUT - no message arrived in 8s')
  Bare.exit()
}, 8000)
