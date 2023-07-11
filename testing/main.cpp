#include <iostream>
#include <fstream>
#include <string>
#include <regex>
/*
<nginx_conf>         ::= <config_statement>*
<config_statement>   ::= <directive_statement> | <block_statement>
<directive_statement>::= <directive> <parameters> ";"
<block_statement>    ::= <directive> "{" <config_statement>* "}"
<directive>          ::= <identifier>
<parameters>         ::= <parameter>*
<parameter>          ::= <string> | <number> | <boolean> | <variable> | <block_statement>
<identifier>         ::= <alpha> <alpha_numeric>*
<string>             ::= "\"" <char>* "\""
<number>             ::= <integer> | <float>
<boolean>            ::= "on" | "off"
<variable>           ::= "$" <identifier>
<char>               ::= <non_special_char> | "\\" <special_char>
<non_special_char>   ::= <letter> | <digit> | "_" | "-" | "." | "/"
<special_char>       ::= "\"" | "\\" | "n" | "r" | "t" | "b" | "f"
<alpha>              ::= "a" | "b" | ... | "z" | "A" | "B" | ... | "Z"
<alpha_numeric>      ::= <alpha> | <digit>
<integer>            ::= <digit>+
<float>              ::= <digit>+ "." <digit>+
*/







// Token types
enum class TokenType {
    Directive,
    Parameter,
    Comment,
    Other
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
};

// Lexer
class Lexer {
public:
    Lexer(const std::string& filePath) : file(filePath) {}

    Token getNextToken() {
        std::string line;
        if (std::getline(file, line)) {
            if (std::regex_match(line, std::regex("^\\s*#"))) {
                return { TokenType::Comment, line };
            } else if (std::regex_match(line, std::regex("\\s*\\w+"))) {
                return { TokenType::Directive, line };
            } else {
                return { TokenType::Parameter, line };
            }
        } else {
            return { TokenType::Other, "" };
        }
    }

private:
    std::ifstream file;
};

// Parser
class Parser {
public:
    void parse(const std::string& filePath) {
        Lexer lexer(filePath);
        Token token;
        while ((token = lexer.getNextToken()).type != TokenType::Other) {
            switch (token.type) {
                case TokenType::Directive:
                    std::cout << "Directive: " << token.value << std::endl;
                    break;
                case TokenType::Parameter:
                    std::cout << "Parameter: " << token.value << std::endl;
                    break;
                case TokenType::Comment:
                    std::cout << "Comment: " << token.value << std::endl;
                    break;
                default:
                    std::cout << "Other: " << token.value << std::endl;
                    break;
            }
        }
    }
};

int main() {
    Parser parser;
    parser.parse("../simple.conf");
    return 0;
}
