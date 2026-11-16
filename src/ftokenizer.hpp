#ifndef FTOKENIZER_HPP
#define FTOKENIZER_HPP

#include <string>
#include <string_view>
#include <vector>
#include <utility>

namespace fluxc {

#ifndef FLUXC_TOKENKIND
#define FLUXC_TOKENKIND
enum class TokenKind {
    IDENTIFIER,
    SYMBOL,
    SUFFIX,
    INDENT,
    DEDENT,
    NEWLINE,
    ENDOFFILE,
    NUMBER,
    DECIMAL,
    STRING,
};
#endif

class Token;

class TokenizedCode {
private:
    const std::string m_raw_code;
    std::vector<Token> m_tokens;
    void tokenize();

public:
    explicit TokenizedCode(std::string&& raw_code)
     : m_raw_code(std::move(raw_code)) { tokenize(); }
    ~TokenizedCode() = default;
    TokenizedCode(TokenizedCode&) = delete;
    TokenizedCode& operator=(TokenizedCode&) = delete;
    std::vector<Token>& get_all_tokens() { return m_tokens; }

};

class Token {
private:
    TokenKind m_kind;
    std::string_view m_sv;

public:
    explicit Token(TokenKind kind, std::string_view&& substr) : m_kind(kind), m_sv(substr) {}
    ~Token() = default;
    TokenKind kind() const { return m_kind; }
    std::string_view str_view() const { return m_sv; }

};

} // namespace fluxc

#endif