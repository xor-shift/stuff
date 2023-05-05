# Stuff, formerly LibStuff

<sub><s>formerly chuck's</s></sub>

Stuff© Libraries™ is a poorly made set of utilities inspired by boost.

### Libraries

- `bit`: Bit and byte manipulation utilities
- `blas`: A barebones linear algebra library
- `core`: A dumping ground for uncategorized utilities
- `expected`: A `std::expected` implementation (not fully standards compliant, made to be simple
  and [to compile with clang 15](https://github.com/llvm/llvm-project/issues/45614))
- `intro`: A customisable™ type introspection library with OOB aggregate introspection.
- `paeno`: Compile time Paeno arithmetic
- `qoi`: A QOI en/decoder, not very fast (includes a lossy encoder)
- `random`: Random number generation utilities (all constexpr, to hell with erand48)
- `ranvec`: A combination of `random` and `blas`; random vector generation utilities
- `scope`: Scope utilities from
  [ibrary fundamentals TS v3](https://cplusplus.github.io/fundamentals-ts/v3.html#scopeguard.exit)
- `sfloat`: Soft-float library, very WIP, no arithmetic
- `thread`: Concurrency utilities. Includes `sync.WaitGroup` and `chan` analogues from Go (`stf::channel` is pretty
  close to Go's spec (`select(...)` isn't randomised but that's about it))
