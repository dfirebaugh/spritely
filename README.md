## Spritely
Spritely compiles to web assembly.  To do this you will need to run `make web` in the root of the repo.  This compiles the project into `index.wasm` and the glue code `index.js`.

To run the app in a browser, it can be served up by a standard web server.  Below you will see a few options that you let you run this locally.

### Docker
```
docker run --name my-nginx -p 8000:80 -v $PWD:/usr/share/nginx/html:ro --rm nginx
```

### NPM
```
npx es-dev-server --node-resolve --watch
```

### python 2

```
python2 -m SimpleHTTPServer 8080
```

### python 3
```
python -m http.server 8080
```
