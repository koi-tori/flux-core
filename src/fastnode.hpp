#ifndef FASTNODE_HPP
#define FASTNODE_HPP

#include <string>

namespace fluxc {

#ifndef FLUXC_KEYWORDS
#define FLUXC_KEYWORDS
constexpr std::string keywords[] = {
"def", "set", "let", "if", "while", "input", "print", "pass",
};
#endif

#ifndef FLUXC_AST_KIND
#define FLUXC_AST_KIND
enum ASTNodes {
    FLUXC_ASTNODES_pass = 0,
    FLUXC_ASTNODES_basictype,
    FLUXC_ASTNODES_type,
    FLUXC_ASTNODES_identifier,
    FLUXC_ASTNODES_parameters,
    FLUXC_ASTNODES_body,
    FLUXC_ASTNODES_def,
    FLUXC_ASTNODES_set,
    FLUXC_ASTNODES_let,
    FLUXC_ASTNODES_if,
    FLUXC_ASTNODES_while,
    FLUXC_ASTNODES_call,
    FLUXC_ASTNODES_input,
    FLUXC_ASTNODES_print,
    FLUXC_ASTNODES_binary,
    FLUXC_ASTNODES_literal_32bit,
    FLUXC_ASTNODES_literal_string,
};
#endif

#ifndef FLUXC_FASTNODE_TYPE_T
#define FLUXC_FASTNODE_TYPE_T
typedef int FASTNODE_TYPE_T;
#endif

#ifndef FLUXC_DATA_INDEX
#define FLUXC_DATA_INDEX
typedef size_t DATA_INDEX;
#endif

#ifndef FLUXC_NODE_INDEX
#define FLUXC_NODE_INDEX
typedef size_t NODE_INDEX;
#endif

struct FASTNode {
    FASTNODE_TYPE_T m_type;
    DATA_INDEX m_data_index;
    std::vector<NODE_INDEX> inner_nodes;
};

} // namespace fluxc

#endif