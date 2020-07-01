Summary of actions from https://emscripten.org/docs/getting_started/Tutorial.html#tutorial

- <EMCC_DIR>/emcc hello_world.c
- node a.out.js // This will print output on command line.


- <EMCC_DIR>/emcc hello_world.c -o hello.html // https://emscripten.org/docs/getting_started/FAQ.html#faq-local-webserver
- python3 -m http.server // Lauches in current directory as web root.
- Go to browser http://0.0.0.0:8080  (and open the html from the list of files)
