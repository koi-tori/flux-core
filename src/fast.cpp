#include "fast.hpp"

#include <stdexcept>

namespace fluxc {

void inline INDENT(int indent, FILE* fp) {
    for (int i = 0; i < indent; ++i) std::fprintf(fp, "\t");
}

void FAST::print_recursively(NODE_INDEX idx, int indent, FILE* fp) {
    auto& current_node = astnodes.at(idx);
    switch (current_node.m_type)
    {
    case FLUXC_ASTNODES_pass:
        INDENT(indent, fp); std::fprintf(fp, "Pass\n");
        break;
    
    case FLUXC_ASTNODES_basictype:
        INDENT(indent, fp); std::fprintf(fp, "BasicType(%s)\n", identifiers.at(current_node.m_data_index).c_str());
        break;

    case FLUXC_ASTNODES_type:
        INDENT(indent, fp); std::fprintf(fp, "Type(%s):\n", identifiers.at(current_node.m_data_index).c_str());
        for (size_t i = 0; i < current_node.inner_nodes.size(); ++i) {
            print_recursively(current_node.inner_nodes.at(i), indent + 1, fp);
        }
        break;
    
    case FLUXC_ASTNODES_identifier:
        INDENT(indent, fp); std::fprintf(fp, "Identifier(%s)\n", identifiers.at(current_node.m_data_index).c_str());
        break;
    
    case FLUXC_ASTNODES_parameters:
    {
        size_t count = current_node.inner_nodes.size() % 2;
        INDENT(indent, fp); std::fprintf(fp, "Parameters:\n");
        if (count == 0) {
            INDENT(indent, fp); std::fprintf(fp, "\tNone\n");
            break;
        }
        for (size_t i = 0; i < count; ++i) {
            INDENT(indent, fp); std::fprintf(fp, "\tParameter:\n");
            print_recursively(current_node.inner_nodes.at(i * 2), indent + 2, fp); // type
            print_recursively(current_node.inner_nodes.at(i * 2 + 1), indent + 2, fp); // identifier/value
        }
        break;
    }

    case FLUXC_ASTNODES_body:
        INDENT(indent, fp); std::fprintf(fp, "Body:");
        for (size_t i = 0; i < current_node.inner_nodes.size(); ++i) {
            print_recursively(current_node.inner_nodes.at(i), indent + 1, fp); // statements
        }
        break;

    case FLUXC_ASTNODES_def:
        INDENT(indent, fp); std::fprintf(fp, "Define(%s):\n", identifiers.at(current_node.m_data_index).c_str());
        for (size_t i = 0; i < current_node.inner_nodes.size(); ++i) {
            print_recursively(current_node.inner_nodes.at(i), indent + 1, fp); // return type, parameters, body
        }
        break;
    
    case FLUXC_ASTNODES_set:
        INDENT(indent, fp); std::fprintf(fp, "Set(%s):\n", identifiers.at(current_node.m_data_index).c_str());
        print_recursively(current_node.inner_nodes.at(0), indent + 1, fp); // target
        break;
    
    case FLUXC_ASTNODES_let:
        INDENT(indent, fp); std::fprintf(fp, "Let(%s):\n", identifiers.at(current_node.m_data_index).c_str());
        print_recursively(current_node.inner_nodes.at(0), indent + 1, fp); // value
        break;
    
    case FLUXC_ASTNODES_if:
        INDENT(indent, fp); std::fprintf(fp, "If:\n");
        INDENT(indent, fp); std::fprintf(fp, "\tCondition:\n");
        print_recursively(current_node.inner_nodes.at(0), indent + 2, fp); // condition
        print_recursively(current_node.inner_nodes.at(1), indent + 1, fp); // body
        if (current_node.inner_nodes.size() >= 3) {
            INDENT(indent, fp); std::fprintf(fp, "\tElse:\n");
            print_recursively(current_node.inner_nodes.at(2), indent + 2, fp); // else body
        }
        break;
    
    case FLUXC_ASTNODES_while:
        INDENT(indent, fp); std::fprintf(fp, "While:\n");
        INDENT(indent, fp); std::fprintf(fp, "\tCondition:\n");
        print_recursively(current_node.inner_nodes.at(0), indent + 2, fp); // condition
        print_recursively(current_node.inner_nodes.at(1), indent + 1, fp); // body
        break;
    
    case FLUXC_ASTNODES_call:
        INDENT(indent, fp); std::fprintf(fp, "Call(%s):\n", identifiers.at(current_node.m_data_index).c_str());
        print_recursively(current_node.inner_nodes.at(0), indent + 1, fp); // parameters
        break;
    
    case FLUXC_ASTNODES_input:
        INDENT(indent, fp); std::fprintf(fp, "Input:\n");
        if (current_node.inner_nodes.size() >= 1) {
            print_recursively(current_node.inner_nodes.at(0), indent + 1, fp); // parameter
        }
        break;
    
    case FLUXC_ASTNODES_print:
        INDENT(indent, fp); std::fprintf(fp, "Print:\n");
        if (current_node.inner_nodes.size() >= 1) {
            print_recursively(current_node.inner_nodes.at(0), indent + 1, fp); // parameter
        }
        break;
    
    case FLUXC_ASTNODES_binary:
        INDENT(indent, fp); std::fprintf(fp, "Binary(%s):\n", identifiers.at(current_node.m_data_index).c_str());
        INDENT(indent, fp); std::fprintf(fp, "\tLeft:\n");
        print_recursively(current_node.inner_nodes.at(0), indent + 2, fp); // LHS
        INDENT(indent, fp); std::fprintf(fp, "\tRight:\n");
        print_recursively(current_node.inner_nodes.at(1), indent + 2, fp); // RHS
        break;
    
    case FLUXC_ASTNODES_literal_32bit:
        INDENT(indent, fp); std::fprintf(fp, "32BitLiteral(%d):\n", i32s.at(current_node.m_data_index).i32);
        break;
    
    case FLUXC_ASTNODES_literal_string:
        INDENT(indent, fp); std::fprintf(fp, "StringLiteral(\"%s\")\n", strings.at(current_node.m_data_index).c_str());
        break;
    
    default:
        break;
    }
}

void FAST::print_tree(FILE* fp) {
    if (astnodes.empty()) throw std::runtime_error("Error: Blank tree.");
    print_recursively(0, 0, fp);
}

} // namespace fluxc