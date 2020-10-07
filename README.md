# spritely
Spritely is a sprite editor with intention on making tiny sprites.

Ideally this will make a 64 sprite sprite sheet.  Each sprite will be 8x8 pixels with 16 colors.

![spritely](./docs/spritely.png)

## Build Requirements

```
$ sudo apt-get install libsdl2-ttf-dev
```

## Webassembly build

runing `make web` will build spritely using emscripten. So in order to run it, you need to download it using instruction from : https://emscripten.org/docs/getting_started/downloads.html

`make web` will create two file: `index.js` and `index.wasm`. Using the given `index.html` and a simple web server (Chrome doesn't support file:// XHR request), you will be able to use spritely in your web browser !

(you can have a simple web server by going in the `web` folder using python2 with `python2 -m SimpleHTTPServer 8080` or python3 with `python -m http.server 8080`)

# Shortcuts and bindings

Here are some shortcuts/bindings:-

`Ctrl+C` -> Copy\
`Ctrl+V` -> Paste\
`Ctrl+Z` -> Undo\
`Ctrl+Shift+Z` OR `Ctrl+Y`-> Redo\
`Ctrl+S` -> Save the spritesheet\
`Ctrl+Shift+S` -> Save the spritesheet and images for each sprite\
`Ctrl+O` -> Load a spritesheet from an image file\
`Left click` -> to draw pixel\
`Right click` -> to select a colour that is on the  canvas\n"
`Arrow Keys` -> to move sprite selection"\
`F`-> fill tool\
`Space` -> pen tool