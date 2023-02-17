#pragma once

namespace stf::paeno {

struct null {};

template<typename T> struct succ { using of = T; };
template<typename T> struct pred { using of = T; };

namespace constants {

using zero = null;
using one = succ<zero>;
using two = succ<one>;
using three = succ<two>;
using four = succ<three>;
using five = succ<four>;
using six = succ<five>;
using seven = succ<six>;
using eight = succ<seven>;
using nine = succ<eight>;

}

}
