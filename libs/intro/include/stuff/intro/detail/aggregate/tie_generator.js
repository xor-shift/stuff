function excel_column(i) {
    if(typeof i === "undefined")
        return "";

    i += 1;

    let ret_chars = [];
    do{
        let char_index = i % 26;
        char_index = char_index !== 0 ? char_index : 26;

        i = (i - char_index) / 26;

        ret_chars.push(char_index);
    } while (i > 0)

    return ret_chars.map(v=>String.fromCharCode('a'.charCodeAt()+v-1)).reverse().join('')
}

function produce_helper(n) {
    let binding_text = "";
    let decltype_text = "";

    for (let i = 0; i < n; i++) {
        if (i !== 0) {
            binding_text += ", ";
            decltype_text += ", ";
        }

        const excel_text = excel_column(i);
        binding_text += excel_text;
        decltype_text += `FWD(${excel_text})`;
    }

    return "" +
`template<typename T>
constexpr auto f(T&& _v, std::integral_constant<usize, ${n}>) {
    auto&& [${binding_text}] = _v;
    return std::forward_as_tuple(${decltype_text});
}`
}

console.log("namespace stf::intro::detail::agg::tie_helpers {")

for (let n = 1; n <= 64; n++) {
    console.log(produce_helper(n));
}

console.log("}")
