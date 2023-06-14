#pragma once

namespace stf {

template<typename... Funcs>
struct multi_visitor : public Funcs... {
    using Funcs::operator()...;
};

template<typename... Funcs>
multi_visitor(Funcs&&...) -> multi_visitor<Funcs...>;

}
