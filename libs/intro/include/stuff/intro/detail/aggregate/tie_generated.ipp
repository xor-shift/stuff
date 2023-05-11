namespace stf::intro::detail::tie_helpers {
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 1>) {
    auto&& [a] = _v;
    return std::forward_as_tuple(FWD(a));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 2>) {
    auto&& [a, b] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 3>) {
    auto&& [a, b, c] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 4>) {
    auto&& [a, b, c, d] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 5>) {
    auto&& [a, b, c, d, e] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 6>) {
    auto&& [a, b, c, d, e, f] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 7>) {
    auto&& [a, b, c, d, e, f, g] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 8>) {
    auto&& [a, b, c, d, e, f, g, h] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 9>) {
    auto&& [a, b, c, d, e, f, g, h, i] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 10>) {
    auto&& [a, b, c, d, e, f, g, h, i, j] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 11>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 12>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 13>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 14>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 15>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 16>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 17>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 18>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 19>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 20>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 21>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 22>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 23>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 24>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 25>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 26>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 27>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 28>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 29>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 30>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 31>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 32>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 33>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 34>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 35>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 36>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 37>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 38>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 39>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 40>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 41>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 42>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 43>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 44>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 45>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 46>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 47>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 48>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 49>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 50>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 51>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 52>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 53>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 54>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 55>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb), FWD(bc));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 56>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb), FWD(bc), FWD(bd));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 57>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb), FWD(bc), FWD(bd), FWD(be));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 58>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb), FWD(bc), FWD(bd), FWD(be), FWD(bf));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 59>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb), FWD(bc), FWD(bd), FWD(be), FWD(bf), FWD(bg));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 60>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb), FWD(bc), FWD(bd), FWD(be), FWD(bf), FWD(bg), FWD(bh));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 61>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh, bi] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb), FWD(bc), FWD(bd), FWD(be), FWD(bf), FWD(bg), FWD(bh), FWD(bi));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 62>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh, bi, bj] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb), FWD(bc), FWD(bd), FWD(be), FWD(bf), FWD(bg), FWD(bh), FWD(bi), FWD(bj));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 63>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh, bi, bj, bk] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb), FWD(bc), FWD(bd), FWD(be), FWD(bf), FWD(bg), FWD(bh), FWD(bi), FWD(bj), FWD(bk));
}
template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, 64>) {
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh, bi, bj, bk, bl] = _v;
    return std::forward_as_tuple(FWD(a), FWD(b), FWD(c), FWD(d), FWD(e), FWD(f), FWD(g), FWD(h), FWD(i), FWD(j), FWD(k), FWD(l), FWD(m), FWD(n), FWD(o), FWD(p), FWD(q), FWD(r), FWD(s), FWD(t), FWD(u), FWD(v), FWD(w), FWD(x), FWD(y), FWD(z), FWD(aa), FWD(ab), FWD(ac), FWD(ad), FWD(ae), FWD(af), FWD(ag), FWD(ah), FWD(ai), FWD(aj), FWD(ak), FWD(al), FWD(am), FWD(an), FWD(ao), FWD(ap), FWD(aq), FWD(ar), FWD(as), FWD(at), FWD(au), FWD(av), FWD(aw), FWD(ax), FWD(ay), FWD(az), FWD(ba), FWD(bb), FWD(bc), FWD(bd), FWD(be), FWD(bf), FWD(bg), FWD(bh), FWD(bi), FWD(bj), FWD(bk), FWD(bl));
}
}
