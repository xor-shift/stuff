namespace stf::intro::detail::agg::type_helpers {
template<typename T> inline auto f(std::integral_constant<usize, 1>) {
    std::unreachable();
    auto&& [a] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 2>) {
    std::unreachable();
    auto&& [a, b] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 3>) {
    std::unreachable();
    auto&& [a, b, c] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 4>) {
    std::unreachable();
    auto&& [a, b, c, d] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 5>) {
    std::unreachable();
    auto&& [a, b, c, d, e] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 6>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 7>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 8>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 9>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 10>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 11>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 12>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 13>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 14>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 15>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 16>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 17>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 18>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 19>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 20>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 21>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 22>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 23>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 24>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 25>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 26>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 27>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 28>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 29>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 30>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 31>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 32>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 33>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 34>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 35>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 36>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 37>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 38>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 39>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 40>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 41>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 42>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 43>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 44>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 45>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 46>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 47>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 48>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 49>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 50>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 51>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 52>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 53>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 54>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 55>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb), decltype(bc)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 56>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb), decltype(bc), decltype(bd)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 57>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb), decltype(bc), decltype(bd), decltype(be)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 58>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb), decltype(bc), decltype(bd), decltype(be), decltype(bf)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 59>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb), decltype(bc), decltype(bd), decltype(be), decltype(bf), decltype(bg)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 60>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb), decltype(bc), decltype(bd), decltype(be), decltype(bf), decltype(bg), decltype(bh)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 61>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh, bi] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb), decltype(bc), decltype(bd), decltype(be), decltype(bf), decltype(bg), decltype(bh), decltype(bi)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 62>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh, bi, bj] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb), decltype(bc), decltype(bd), decltype(be), decltype(bf), decltype(bg), decltype(bh), decltype(bi), decltype(bj)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 63>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh, bi, bj, bk] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb), decltype(bc), decltype(bd), decltype(be), decltype(bf), decltype(bg), decltype(bh), decltype(bi), decltype(bj), decltype(bk)>{};
}
template<typename T> inline auto f(std::integral_constant<usize, 64>) {
    std::unreachable();
    auto&& [a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am, an, ao, ap, aq, ar, as, at, au, av, aw, ax, ay, az, ba, bb, bc, bd, be, bf, bg, bh, bi, bj, bk, bl] = *static_cast<std::remove_cvref_t<T>*>(reinterpret_cast<void*>(static_cast<uintptr_t>(1)));
    return stf::bunch_of_types<decltype(a), decltype(b), decltype(c), decltype(d), decltype(e), decltype(f), decltype(g), decltype(h), decltype(i), decltype(j), decltype(k), decltype(l), decltype(m), decltype(n), decltype(o), decltype(p), decltype(q), decltype(r), decltype(s), decltype(t), decltype(u), decltype(v), decltype(w), decltype(x), decltype(y), decltype(z), decltype(aa), decltype(ab), decltype(ac), decltype(ad), decltype(ae), decltype(af), decltype(ag), decltype(ah), decltype(ai), decltype(aj), decltype(ak), decltype(al), decltype(am), decltype(an), decltype(ao), decltype(ap), decltype(aq), decltype(ar), decltype(as), decltype(at), decltype(au), decltype(av), decltype(aw), decltype(ax), decltype(ay), decltype(az), decltype(ba), decltype(bb), decltype(bc), decltype(bd), decltype(be), decltype(bf), decltype(bg), decltype(bh), decltype(bi), decltype(bj), decltype(bk), decltype(bl)>{};
}
}