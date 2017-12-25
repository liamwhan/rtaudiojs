# Test Node Addon

## Dependencies
- NodeJS runtime (we used v8.9.3)
- `node-gyp` (Node Native Build Tool): `npm i -g node-gyp`)
- Python (required by `node-gyp`. We used v2.7.14)


## Build
1. Create [`hello.cpp`](hello.cpp)
2. Create JSON build file [`binding.gyp`](binding.gyp)
3. Configure `node-gyp`: `node-gyp configure`
4. Build: `node-gyp build`


## Notes and Gotchas

### Windows

#### Multiple MS C++ Build Toolset Versions
`node-gyp` makes assumptions about the MS build tools versions, so if you bootstrap your Windows terminal with `vcvarsall.bat` your environment variables will be wrong and `node-gyp build` may be unable to find the v14x toolset. 

Apparently, you can specify the toolset version by setting the `GYP_MSVS_VERSION` environment variable (e.g. `GYP_MSVS_VERSION=2015`) but I haven't tried this.
