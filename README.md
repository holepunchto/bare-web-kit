# bare-web-kit

WebKit for Bare on macOS and iOS. It gives you a web view that you can put in a window next to your other views.

The web view works with views from other addons, such as `bare-app-kit` on macOS and `bare-ui-kit` on iOS.

```
npm i bare-web-kit
```

## Usage

```js
const { Window } = require('bare-app-kit')
const { WebView } = require('bare-web-kit')

const window = new Window({ width: 800, height: 600 })

const webView = new WebView({ width: 800, height: 600 })

window.contentView.addSubview(webView)
window.makeKeyAndOrderFront()

webView.loadRequest('https://example.com')
```

## License

Apache-2.0
