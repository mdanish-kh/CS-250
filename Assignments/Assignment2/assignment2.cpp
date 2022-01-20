#include <iostream>
#include <string>
#include <algorithm>
#include <exception>
#include <bits/stdc++.h>

using std::cout;
using std::endl;
using std::string;


#define INITIAL_SIZE 10
#define EMPTY -1
#define GROWTH_FACTOR 2

class InfixEvaluation
{
    public:

    // ADT exposes following functions to end user.
    /*
    double showPostfixForm(string infix);
    double evaluateInfix(string infix);
    double getResult();
    */

    InfixEvaluation(){}
    InfixEvaluation(string expression)
    {
        string postfix = convertToPostfix(expression);
        if (postfix != "")
        {
            this->expression = expression;
            result = evaluatePostfix(postfix);
        }
    }   
    
    double evaluateInfix(string infix)
    {
        string postfix = convertToPostfix(infix);
        if (postfix != "")
        {
            this->expression = infix;
            evaluatePostfix(postfix);
            return result;
        }
        throw std::runtime_error(string("Given expression is not correct."));
    }

    string showPostfixForm(string infix)
    {
        return convertToPostfix(infix);
    }

    double getResult()
    {
        if (this->expression != "")
            return result;
        throw std::runtime_error(string("Cannot display result. No / Incorrect expression provided."));
    }


    private:
    double result;
    char separator = ' ';
    string expression = "";


    // Array Based Implementation of Stack ADT
    template <class type>
    class Stack
    {
        private:
        int top = EMPTY;
        int capacity = INITIAL_SIZE;

        // Dynamic array
        type* array = new type[INITIAL_SIZE];

        // Copy elements from previous array to new bigger array.
        void Copy(type from_arr[], type to_arr[])
        {
            int length = top + 1;

            for (int i = 0; i < length; i++)
                to_arr[i] = from_arr[i];
        }

        // Dynamically grow the array if capacity is full.
        void Grow()
        {
            // Increase capacity by growth factor
            capacity *= GROWTH_FACTOR;

            // Dynamically create a bigger array.
            type* new_array = new type[capacity];

            // Copy old contents to new array.
            Copy(array, new_array);

            // Release memory occupied by previous array.
            delete[] array;

            // Point the old array pointer to the newly created array.
            array = new_array;
        }

        // Class exposes following functions to the end user.
        public:
        bool isFull()
        {
            return (capacity - 1 == top);
        }

        bool isEmpty()
        {
            return (top == EMPTY);
        }

        // Insert a new element at the top of the stack.
        void push(type item)
        {
            if (isFull()) Grow();

            array[++top] = item;

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
                    cout << j << ". " << array[i] << "  <--- (top) " << endl;
        
                else cout  << j << ". " << array[i] << endl;
            }
            return;   
        }
        
    };

    // ------------------------- HELPER FUNCTIONS ---------------------------------

    // Helper Function List
    /*
    string convertToPostfix(string infix);
    double evaluatePostfix(string postfix);
    bool isInfix(string* expression);
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
    double Compute(double left, char op, double right);
    */
    
    // Convert a given infix string to postfix form. Uses separators to distinguish multi-digit numbers.
    string convertToPostfix(string infix)
    {
        // Removing whitespaces from the infix expression.
        infix = trim(infix);
        
        // Check syntax of infix.
        if (!isInfix(&infix)) return "";

        Stack <char> stack;
        string postfix;

        // Infix to postfix algorithm
        int j = 0;
        for (int i = 0; i < infix.length() + 1; i++)
        {
            
            if (isNum(infix[i]) || infix[i] == '.')

                // Print number directly to output.
                postfix += infix[i];
            

            else if (isBracket(infix[i]))
            {

                if (infix[i] == '(' || infix[i] == '{' || infix[i] == '[')
                    stack.push(infix[i]);

                // Closing bracket is found
                else
                {
                    char bracket = infix[i];
                    popUntilOpenBracket(&postfix, bracket, &stack, separator);
                }
            }
            else if (isOperator(infix[i]))
            {
                // In infix notation, two numbers are separated by an operator.
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

    // Evaluate a postfix expression.
    double evaluatePostfix(string postfix)
    {
        Stack <double> stack;
        
        char _operator;
        string left;
        string right;
        double left_operand;
        double right_operand;

        // Handling the case when user types only a single number. i.e. 2
        result = std::stod(postfix);

        for (int i = 0; i < postfix.length(); i++)
        {
            left = "", right = "";
            if (isNum(postfix[i]) || postfix[i] == '.')
            {
                // Right operand begins after a separator.
                if (postfix[i - 1] == separator)
                {
                    // Right operator precedes an operator. Scan until an operator is found.
                    while (!isOperator(postfix[i]))
                    {
                        right += postfix[i++];
            
                        if (postfix[i] == separator)
                            break;
                    }
                    // Convert string to double and push into the stack.
                    stack.push(std::stod(right));
                }
                
                else
                {
                    // Left operator precedes a separator. Scan until separator is found.
                    while (postfix[i] != separator && !isOperator(postfix[i]) && postfix[i] != '\0')    
                        left += postfix[i++];
                    stack.push(std::stod(left));
                }
            }

            if (isOperator(postfix[i]))
            {
                _operator = postfix[i];
                
                // Pop two times for left and right operands and compute the result.
                right_operand = *stack.peak();
                stack.pop();
                
                left_operand = *stack.peak();
                stack.pop();

                result = Compute(left_operand, _operator, right_operand);
                
                // Push the result back into the stack.
                stack.push(result);
            }
        }

        return result;
    }



    // Checks whether the given expression is correct infix or not.
    bool isInfix(string* expression)
    {
        // Keeps track of how many times we've inserted '0' to correct the string.
        int count = 0;

        // Location where possible error might occur.
        int location = 0;

        // Firstly checks whether the brackets used are balanced or not.
        if (!isBracketCorrect(expression[0])) return false;

        for (int i = 0; i < expression[0].length(); i++)
        {
            // String size increases when we append '0's, so we subtract count to keep track of current location.
            location = (i + 1) - count;

            // If an unknown character is encountered.
            if (!allowedChar(expression[0][i]))
            {
                cout << "Syntax Error. Unrecognized character '" << expression[0][i] 
                << "' at character # " << location << endl;
                return false;
            }

            // Case when expression contains empty brackets i.e. () without any value in between.
            if (isOpenBracket(expression[0][i]) && isCloseBracket(expression[0][i + 1])) 
            {
                cout << "Syntax Error. Error at character # " << location
                << ". \nBrackets don't contain a value." << endl;
                return false;
            }

            // Checks for correct use of decimal point. Valid decimal point appears before a number. i.e., .234 or 0.234
            if (expression[0][i] == '.')
            {
                // If not valid decimal point.
                if ( !isNum(expression[0][i + 1])  )
                {
                    cout << "Syntax Error. Error at character # " << location
                        << ". \nIncorrect use of decimal point." << endl;
                        return false;
                }
            }

            // Checks whether an operator conforms with infix notation (i.e. Left Root Right)
            if (isOperator(expression[0][i]))
            {
                
                char right = expression[0][i + 1];
                char left = expression[0][i - 1];
                
                // Allowing + and - to be used as unary operators.
                if ((expression[0][i] == '+' || expression[0][i] == '-'))
                {
                    // Insert 0 if there is no operand to the left of '+' or '-' operators.
                    // i.e. "-2" becomes "0 - 2" for postfix evaluation.
                    if ( ( !isNum(left) && !isCloseBracket(left) && isNum(right)) )
                    {
                        expression -> insert(i, "0");
                        count++;

                        left = expression[0][i];
                        right = expression[0][i + 2];
                    }
                }

                // Case when such expressions appear "23+" , "4*" , "+-" that do not conform with infix notation.
                if ( !((isCloseBracket(left) || isNum(left) ) && (isOpenBracket(right) || isNum(right))) )
                {
                    cout << "Syntax Error. Error at character # " << location
                    << ". \nIncorrect Infix Notation." << endl;
                    return false;
                }
            }

        }

        return true;
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

    // Remove whitespaces from a string.
    string trim(string str)
    {
        str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
        return str;
    }
    
    bool isNum(char ch)
    {
        return (ch >= '0' && ch <= '9');
    }

    bool isOperator(char ch)
    {
        return (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '^' || ch == '%');
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

    // Only numbers, brackets, operators and decimal points are allowed in an algebraic expression.
    bool allowedChar(char ch)
    {
        return (isNum(ch) || isOperator(ch) || isBracket(ch) || ch == '.');
    }

    // Returns precedence/priority of operators.
    int precedence(char _operator)
    {
        if (_operator == '+' || _operator == '-') return 1;
        if (_operator == '*' || _operator == '/' || _operator == '%') return 2;
        if (_operator == '^') return 3;
        return 0;
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
            // Pop until we find a matching opening bracket.
            while (*stk -> peak() != open_bracket)
            {
                *str += *stk -> peak();
                stk -> pop();
            }
            // Pop the final bracket from stack as well.
            stk -> pop();

        }

        return;
    }


    // Calculates and returns the result depending upon the required operation.
    double Compute(double left, char op, double right)
    {
        double result;

        result = ((op == '+') ? (left + right) : (op == '-') ? (left - right) : (op == '*') ? (left * right) :
        (op == '/') ? (left / right) : (op == '%') ? (fmod(left, right)) : (op == '^') ? (pow(left, right)) : 0);

        return result;
    }
};


// Prototype required for main.
string get_string(string prompt);

// Main function
int main(void)
{
    
    string expression = get_string("Enter Infix expression: ");
    
    cout << "\n-------Infix Evaluation--------\n" << endl;
    cout << "Expression: " << expression << endl;
    
    InfixEvaluation obj(expression);

    cout << "Postfix form: " << obj.showPostfixForm(expression) << endl;
    cout << "Result: " << std::fixed << std::setprecision(3) << obj.getResult() << endl;
    
}

// Displays a prompt and inputs a string from user.
string get_string(string prompt)
{
    string str;
    cout << prompt;
    getline(std::cin , str);
    return str;
}


/* Notes to self:

Possible bottleneck in performance:

    Currently isInfix method contains two loops that traverse the whole expression.
    - One loop traverses the expression and looks for bracket imbalance.
    - The other one traverses and looks for any unknown symbols, correct infix notation etc.

    The two can be merged into a single loop that performs both of these tasks. However to maintain
    readability, this is currently avoided. The time complexity is linear in either case as both loops
    are independent.

*/