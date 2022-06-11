# Organic-namer

## Building instruction
### Native:
* `mkdir build`
* `cd build`
* `cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..`
* `ninja`
### Web:
* Set emscripten env variables
* `mkdir embuild`
* `cd embuild`
* `emcmake cmake -G Ninja ..`
* `ninja`
