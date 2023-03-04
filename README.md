# Stuff, formerly LibStuff

<sub><s>formerly chuck's</s></sub>

Stuff© Libraries™ is a poorly made set of utilities inspired by boost.

### Layout

#### Namespaces

- stf::lib -> the API of `lib` with which a user should intract with.
- stf::lib::concepts -> the concepts associated with `lib`.
- stf::lib::detail -> private names related to `lib`. should not be accessed by the user.
- stf::lib::concepts::detail -> private names (usually helper concepts and functions) related to the concepts of `lib`.
  This should not be accessed by the user

### Libraries

- `bit`: bit and byte manipulation utilities
- `blas`: a barebones linear algebra library
- `core`: a dumping ground for uncategorized utilities
- `expected`: a `std::expected` implementation (not fully standards compliant, made to be simple
  and [to compile with clang 15](https://github.com/llvm/llvm-project/issues/45614))
- `paeno`: compile time paeno arithmetic
- `qoi`: a qoi en/decoder, not very fast
- `scope`: scope utilities
  from [ibrary fundamentals TS v3](https://cplusplus.github.io/fundamentals-ts/v3.html#scopeguard.exit)
- `sfloat`: soft-float library, very WIP, no arithmetic
