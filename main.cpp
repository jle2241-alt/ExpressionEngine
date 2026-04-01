#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>

#include "ArrayStack"

using namespace std;

// Token

struct Token {
    string value;   // number, operator, or parenthesis
};

// Tokenize

vector<Token> tokenize(const string& line) {
    vector<Token> tokens;
    int i = 0;

    while (i < static_cast<int>(line.length())) {
        if (isspace(line[i])) {
            i++; // space skip
        }
        else if (isdigit(line[i])) {
            string number;
            while (i < static_cast<int>(line.length()) && isdigit(line[i])) {
                number += line[i];
                i++;
            }
            tokens.push_back(Token(number));
        }
        else {
            // invalid character, still store it (will be caught later)
            string bad(1, line[i]);
            tokens.push_back(Token(bad));
            i++;
        }
    }
    return tokens;
}

// Helpers

bool isOperator(const string& s) {
    return s == "+" || s == "-" || s == "*" || s == "/";
}

int precedence(const string& op) {
   if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

// Detection

bool isValidPostfix(const vector<Token>& tokens) {
    if (tokens.empty()) {
        return false;
    }

    int depth= 0;

    for ( const auto & token : tokens ) {
        bool isNum = true;

        if (token.value.empty()) {
            isNum = false;
        }
        else {
            for (char c : token.value) {
                if (!isdigit(c)) {
                    isNum = false;
                    break;
                }
            }
        }
        if (isNum) {
            depth++;
        }
        else if (isOperator(token.value)) {
            if (depth < 2) {
                return false;
            }
            depth--;
        }
        else {
            return false;
        }
        return depth ==1;
    }
}
bool isValidInfix(const vector<Token>& tokens) {
        if (tokens.empty()) {
            return false;
        }
        int depth= 0;

        for (const auto & token : tokens) {
            bool isNum = true;

            if (token.value.empty()) {
                isNum = false;
            } else {
                for (char c : token.value) {
                    if (!isdigit(c)) {
                        isNum = false;
                        break;
                    }
                }
            }
            if (isNum) {
                depth++;
            }
            else if (isOperator(token.value)) {
                if (depth < 2) {
                    return false;
                }
                depth--;
            }
            else {
                return false;
            }
        }
        return depth ==1;
    }

// Conversion

vector<Token> infixToPostfix(const vector<Token>& tokens) {
    vector<Token> output;
    ArrayStack<Token> ops;

        for (const auto & token : tokens) {
            bool isNum = true;

            if (token.value.empty()) {
                isNum = false;
            } else {
                for (char c : token.value) {
                    if (!isdigit(c)) {
                        isNum = false;
                        break;
                    }
                }
            }
            if (isNum) {
                output.push_back(token);
            }
            else if (token.value == ")") {
                ops.push(token);
            }
            else if (token.value == "(") {
                while (!ops.empty() && ops.top().value != "("){
                    output.push_back(ops.top());
                    ops.pop();
                }if (!ops.empty() && ops.top().value == "(") {
                    ops.pop();
                }
            }
            else if (isOperator(token.value)) {
                while (!ops.empty() &&
                       isOperator(ops.top().value) &&
                       precedence(ops.top().value) >= precedence(token.value)) {
                    output.push_back(ops.top());
                    ops.pop();
                       }

                ops.push(token);
            }
        }

        while (!ops.empty()) {
            output.push_back(ops.top());
            ops.pop();
        }
    return output;
}

// Evaluation

double evalPostfix(const vector<Token>& tokens) {
    ArrayStack<double> stack;
    // TODO
    return 0.0;
}

// Main

int main() {
    string line;
    getline(cin, line);

    vector<Token> tokens = tokenize(line);

    if (isValidPostfix(tokens)) {
        cout << "FORMAT: POSTFIX\n";
        cout << "RESULT: " << evalPostfix(tokens) << "\n";
    }
    else if (isValidInfix(tokens)) {
        vector<Token> postfix = infixToPostfix(tokens);
        cout << "FORMAT: INFIX\n";
        cout << "POSTFIX: ";
        for (const auto& t : postfix) {
            cout << t.value << " ";
        }
        cout << "\n";
        cout << "RESULT: " << evalPostfix(postfix) << "\n";
    }
    else {
        cout << "FORMAT: NEITHER\n";
        cout << "ERROR: invalid expression\n";
    }

    return 0;
}
