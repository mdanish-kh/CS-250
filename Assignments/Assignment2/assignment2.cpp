#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

#define SIZE 100
#define NOT_FOUND -1
#define EMPTY -1

// Array Based Implementation of Stack
class Stack
{
    private:
    int top = EMPTY;

    // Character stack
    char array[SIZE];

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
    bool push(char ch)
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
    
    // Returns top most element of the stack.
    char peak()
    {
        if (isEmpty())
            return NOT_FOUND;
        return array[top];
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