# Emehcs - an ~Scheme interpreter/so-called language

English | [简体中文](./readme_CN.md)

Depend on pure C++20 and its STL.

- C++17 for `std::varient`, fold expression, structured bind, initializers for `if`, and etc.
- C++20 for `contains` of `std::unordered_set`, `starts_with` and `ends_with` of `std::string`

## how to compile

```shell
mkdir -p _build && cd _build
cmake ..
make
```
