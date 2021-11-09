#include <iostream>
#include <string>
#include <algorithm>
#include <exception>


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::exception;

#define SIZE 100
#define EMPTY -1

template <class type>
// Array Based Implementation of Stack
class Stack
{
    private:
    int top = EMPTY;

    // Character stack
    type array[SIZE];

    public:
    bool isFull()
    {
        return (SIZE - 1 == top);
    }

    bool isEmpty()
    {
        return (top == EMPTY);
    }

    // Insert a new element at the top of the stack.
    bool push(type ch)
    {
        if (isFull()) return false;

        array[++top] = ch; 
        return true; 
    }

    // Delete the top most element of the stack.
    bool pop()
    {
        if (isEmpty()) return false;

        top--;
        return true;

    }
    
    // Returns top most element of the stack. Returns NULL if stack is empty.
    type* peak()
    {
        if (isEmpty())
            return NULL;
        return &array[top];
    }

    // Display stack
    void print()
    {
        if (isEmpty()) return;
        
        for (int i = top, j = 1; i >= 0; i--, j++)
        {
            if (i == top)
            {
                cout << j << ". " << array[i] << "  <--- (top) " << endl;
            } 
            else cout  << j << ". " << array[i] << endl;
        }
        return;   
    }
    
};

class Overflow : public exception
{  
    public:  
        const char * what() const throw()  
        {  
            string temp = "Stack overflow. String length is greater than " + std::to_string(SIZE) + "\n";
            char const *num_char = temp.c_str();
            return num_char;  
        }  
};  


// Function prototypes
string convertToPostfix(string infix);


int main(void)
{
    
    string expression = "(23 + 3) * (5 + 7) * 2 ^ 3";
    cout << "Expression: " << expression << "\n\n";

    string postfix = convertToPostfix(expression);
    cout << "Postfix form: " << postfix << endl;

       
}


// Function prototypes
bool isInfix(string expression);
bool isBracketCorrect(string expression);
string trim(string str);
bool allowedChar(char ch);
bool isNum(char ch);
bool isOperator(char ch);
bool isBracket(char ch);
bool isCloseBracket(char ch);
bool isOpenBracket(char ch);
int precedence(char _operator);
void popUntilOpenBracket(string* str, char bracket, Stack<char>* stk, char sep);
char separator = ' ';



string convertToPostfix(string infix)
{
    // Removing whitespaces from the infix expression.
    infix = trim(infix);
    
    // Check stack overflow
    if (infix.length() > 2 * SIZE) throw Overflow();

    // Check syntax of infix.
    if (!isInfix(infix)) return "";

    Stack <char> stack;
    string postfix;

    // Infix to postfix algorithm
    int j = 0;
    for (int i = 0; i < infix.length() + 1; i++)
    {
        if (stack.isFull()) throw Overflow;
        
        if (isNum(infix[i]))
            // Print number directly to output.
            postfix += infix[i];
        
        else if (isBracket(infix[i]))
        {
            if (infix[i] == '(' || infix[i] == '{' || infix[i] == '[')
                stack.push(infix[i]);
            else
            {
                char bracket = infix[i];
                popUntilOpenBracket(&postfix, bracket, &stack, separator);
            }
        }
        else if (isOperator(infix[i]))
        {
            if (isNum(infix[i - 1]))
                postfix += separator;
            

            // Incoming operator is of higher precedence.
            if (stack.peak() == NULL) stack.push(infix[i]);
            else
            { 
                if (precedence(infix[i]) > precedence(*stack.peak()))
                    stack.push(infix[i]);

                else
                {
                    while (precedence(infix[i]) <= precedence(*stack.peak()))
                    {
                        // Pop the operator with lower precendence and write it into the output.
                        postfix += *stack.peak();
                        stack.pop();
                        if (stack.peak() == NULL)
                            break;
                    }
                    stack.push(infix[i]);
                }
            }
        }
        // Reached end of string
        else if (infix[i] == '\0')
        {
            while (!stack.isEmpty())
            {
                postfix += *stack.peak();
                stack.pop(); 
            }
        }
            
    }
    
    return postfix;
}


// Pop items from stack and write them into output until matching open bracket is found.
void popUntilOpenBracket(string* str, char bracket, Stack <char>* stk, char sep)
{
    char open_bracket;
    if (bracket == ')') open_bracket = '(';
    else if (bracket == '}') open_bracket = '{';
    else open_bracket = '[';

    if (stk -> peak() != NULL)
    {

        while (*stk -> peak() != open_bracket)
        {
            *str += *stk -> peak();
            stk -> pop();
        }
        // Pop the bracket from stack as well.
        stk -> pop();

    }

    return;
}

bool isNum(char ch)
{
    return (ch >= '0' && ch <= '9');
}

bool isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '^');
}

bool isCloseBracket(char ch)
{
    return (ch == ')' || ch == '}' || ch == ']');
}

bool isOpenBracket(char ch)
{
    return (ch == '(' || ch == '{' || ch == '[');
}

bool isBracket(char ch)
{
    return (isCloseBracket(ch) || isOpenBracket(ch));
}

bool allowedChar(char ch)
{
    return (isNum(ch) || isOperator(ch) || isBracket(ch));
}

int precedence(char _operator)
{
    if (_operator == '+' || _operator == '-') return 1;
    if (_operator == '*' || _operator == '/') return 2;
    if (_operator == '^') return 3;
    return 0;
}


// Checks whether the given expression is correct infix or not.
bool isInfix(string expression)
{
    if (!isBracketCorrect(expression)) return false;

    for (int i = 0; i < expression.length(); i++)
    {
        if (!allowedChar(expression[i]))
        {
            cout << "Syntax Error. Unrecognized character '" << expression[i] 
            << "' at character # " << (i + 1) << endl;
            return false;
        }

        if (isOperator(expression[i]))
        {
            char left = expression[i - 1];
            char right = expression[i + 1];

            if ( !((isCloseBracket(left) || isNum(left) ) && (isOpenBracket(right) || isNum(right))) )
            {
                cout << "Syntax Error. Error at character # " << (i + 1)
                << ". \nIncorrect Infix Notation." << endl;
                return false;
            }
        }

    }

    return true;
}

// Remove whitespaces from a string.
string trim(string str)
{
    str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
    return str;
}

// Checks whether brackets are used correctly or not.
bool isBracketCorrect(string expression)
{
    // Stack for maintaining opening brackets
    Stack <char> bracket_stack;

    // Variable stores location in string of possible erroneous bracket
    int location = 1;

    // Variable for storing type of bracket i.e., round, curly etc
    char bracket = expression[0];

    for (int i = 0; i < expression.length(); i++)
    {
        // Pushing opening brackets and their respective character number into the stack
        if (isOpenBracket(expression[i]))
        {
            bracket = expression[i];
            bracket_stack.push(bracket);
            location = i + 1;
        }

        // Once a closing bracket is encountered
        else if (isCloseBracket(expression[i]))
        {
            // In case stack is already empty
            if (bracket_stack.isEmpty()) 
            {
                cout << "Error at character #" << location << ". Closing bracket '" << bracket
                << "' has no opening bracket." << endl;
                return false;
            }
            
            char close_bracket = expression[i];
            char open_bracket = *bracket_stack.peak();


            // If closing bracket matches with its opening pair, we pop the bracket from the stack i.e., the brackets are valid
            if  ( (close_bracket == ')' && open_bracket == '(') ||
                  (close_bracket == '}' && open_bracket == '{') ||
                  (close_bracket == ']' && open_bracket == '[')   )
            {
                bracket_stack.pop();
            }
            
            // If opening and closing brackets do not form a valid pair
            else 
            {
                cout << "Error at character # " << location << " '" << bracket
                << "'." << " -Not closed" << endl;
                return false;
            }   
        }
    }
    if (!bracket_stack.isEmpty())
    {
        cout << "Error at character # " << location << " '" << bracket 
            << "'." << " -Not closed" << endl;
        return false;
    }
        
    return true;
}


// Check for correct brackets  --- DONE
// Check whether correct infix expression --- DONE
// Convert to postfix --- DONE
// Check for stack overflow
// Evaluate expression.