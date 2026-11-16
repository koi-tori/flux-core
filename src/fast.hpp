#ifndef FAST_HPP
#define FAST_HPP

#include <string>
#include <vector>
#include <cstdio>

#include "fastnode.hpp"
#include "ftokenizer.hpp"

namespace fluxc {

#ifndef FLUXC_TYPES_I8
#define FLUXC_TYPES_I8
union i8 {
    u_int8_t ui8;
    char c8;
    bool b8;
};
#endif

#ifndef FLUXC_TYPES_I16
#define FLUXC_TYPES_I16
union i16
{
    u_int16_t ui16;
    short i16;
};
#endif

#ifndef FLUXC_TYPES_I32
#define FLUXC_TYPES_I32
union i32
{
    u_int32_t ui32;
    int i32;
    float f32;
};
#endif

#ifndef FLUXC_TYPES_I64
#define FLUXC_TYPES_I64
union i64
{
    u_int64_t ui64;
    long long i64;
    double f64;
};
#endif

class FAST {
private:
    std::vector<std::string> identifiers;
    std::vector<FASTNode> astnodes;
    std::vector<std::string> strings;
    std::vector<i8> i8s;
    std::vector<i16> i16s;
    std::vector<i32> i32s;
    std::vector<i64> i64s;
    void parse();
    void print_recursively(NODE_INDEX idx, int indent, FILE* fp = stderr);

public:
    explicit FAST(std::vector<Token>& tokens);
    explicit FAST(TokenizedCode& code);
    ~FAST();
    void print_tree(FILE* fp = stderr);

};

} // namespace fluxc

#endif