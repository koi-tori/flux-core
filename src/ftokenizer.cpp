#include "ftokenizer.hpp"

#include <deque>
#include <stdexcept>

namespace fluxc {

void TokenizedCode::tokenize() {
    size_t token_begin = 0;
    bool is_after_newline = false;
    bool is_in_comment = false;
    bool is_recording = false;
    bool is_after_slash = false;
    bool is_after_dot = false;
    std::deque<int> indents;
    indents.push_back(0);
    int current_indent = 0;
    TokenKind current_kind = TokenKind::IDENTIFIER;
    for (size_t i = 0; i < m_raw_code.size(); ++i) {
        char current = m_raw_code[i];
        if (current == '\n') {
            is_after_newline = true;
            is_in_comment = false;
            is_after_dot = false;
            current_indent = 0;
            if (is_recording) {
                if (current_kind == TokenKind::STRING) throw std::runtime_error("Error: Expect < \" > at the end of line.");
                m_tokens.emplace_back(current_kind, std::string_view(m_raw_code.data() + token_begin, i - token_begin));
                is_recording = false;
            }
            continue;
        }
        if (is_in_comment) continue;
        if (is_after_newline) {
            if (current == ' ') {
                ++current_indent;
                continue;
            }
            if (current_indent > indents.back()) {
                indents.push_back(current_indent);
                m_tokens.emplace_back(TokenKind::INDENT, "{");
            } else if (current_indent < indents.back()) {
                bool has_matched = false;
                for (auto indent : indents) {
                    if (indent == current_indent) {
                        has_matched = true;
                        break;
                    }
                }
                if (!has_matched) throw std::runtime_error("Error: No matched indent.");
                do {
                    indents.pop_back();
                    m_tokens.emplace_back(TokenKind::DEDENT, "}");
                } while (current_indent != indents.back());
            }
            m_tokens.emplace_back(TokenKind::NEWLINE, "\\n\n");
            is_after_newline = false;
        }
        if (is_recording) {
            if (current_kind == TokenKind::IDENTIFIER) {
                if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')
                 || (current >= '0' && current <= '9') || current == '_') {
                    continue;
                }
                if (current == '\"') {
                    m_tokens.emplace_back(current_kind, std::string_view(m_raw_code.data() + token_begin, i - token_begin));
                    current_kind = TokenKind::STRING;
                    token_begin = i + 1;
                    continue;
                }
            }
            if (current_kind == TokenKind::NUMBER) {
                if (current >= '0' && current <= '9') {
                    continue;
                }
                if (current == '.') {
                    if (is_after_dot) throw std::runtime_error("Error: Multiple decimal points.");
                    is_after_dot = true;
                    continue;
                }
                if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z'))
                    throw std::runtime_error("Error: Undefined literal.");
                if (current == '\"') {
                    m_tokens.emplace_back(current_kind, std::string_view(m_raw_code.data() + token_begin, i - token_begin));
                    current_kind = TokenKind::STRING;
                    token_begin = i + 1;
                    continue;
                }
                if (is_after_dot) {
                    is_after_dot = false;
                    current_kind = TokenKind::DECIMAL;
                }
            }
            if (current_kind == TokenKind::STRING) {
                if (is_after_slash) {
                    is_after_slash = false;
                    continue;
                }
                if (current != '\"') {
                    if (current == '\\') is_after_slash = true;
                    continue;
                }
            }
            m_tokens.emplace_back(current_kind, std::string_view(m_raw_code.data() + token_begin, i - token_begin));
            is_recording = false;
        } else {
            if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || current == '_') {
                current_kind = TokenKind::IDENTIFIER;
                token_begin = i;
                is_recording = true;
                continue;
            }
            if (current >= '0' && current <= '9') {
                current_kind = TokenKind::NUMBER;
                token_begin = i;
                is_recording = true;
                continue;
            }
            if (current == '\"') {
                current_kind = TokenKind::STRING;
                token_begin = i + 1;
                is_recording = true;
                continue;
            }
        }
        if (current == ' ' || current == '\"') {
            continue;
        }
        if (current == '=') {
            if (i < m_raw_code.size() - 1 && m_raw_code[i + 1] == '=') {
                m_tokens.emplace_back(TokenKind::SYMBOL, "==");
                ++i;
                continue;
            }
            m_tokens.emplace_back(TokenKind::SYMBOL, "=");
            continue;
        }
        if (current == '+') {
            if (i < m_raw_code.size() - 1 && m_raw_code[i + 1] == '=') {
                m_tokens.emplace_back(TokenKind::SYMBOL, "+=");
                ++i;
                continue;
            }
            m_tokens.emplace_back(TokenKind::SYMBOL, "+");
            continue;
        }
        if (current == '#') {
            is_in_comment = true;
            continue;
        }
        m_tokens.emplace_back(TokenKind::SYMBOL, std::string_view(m_raw_code.data() + i, 1));
    }
    if (is_recording) {
        if (current_kind == TokenKind::STRING) throw std::runtime_error("Error: Expect < \" > at the end of file.");
        m_tokens.emplace_back(current_kind, std::string_view(m_raw_code.data() + token_begin, m_raw_code.size() - token_begin));
    }
    while (indents.size() > 1) {
        indents.pop_back();
        m_tokens.emplace_back(TokenKind::DEDENT, "}");
    } 
    m_tokens.emplace_back(TokenKind::ENDOFFILE, "EOF");
}

} // namespace fluxc