#pragma once
template<class... Ts> struct overload : Ts... { using Ts::operator()...; }; // (1)
template<class... Ts> overload(Ts...) -> overload<Ts...>;  // (2)

