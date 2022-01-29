#include <iostream>
#include <ios>
#include <limits>
#include <string>

using std::cin;
using std::cout;
using std::string;
using std::numeric_limits;
using std::streamsize;
using std::endl;


// Student struct for storing student's information.
struct Student
{
    long long id;
    string name;
    float CGPA;
    string address;
};

// Vector class to store student structs in a dynamic array.
class Vector
{
    private:
    // Initializing variables.
    int const INITIAL_SIZE = 2;
    int const GROWTH_FACTOR = 2;
    int length = 0;
    int capacity = INITIAL_SIZE;
    Student* array = new Student[INITIAL_SIZE];

    // Helper functions for Vector Class.

    // Helper Function for Insert. Grows array when needed.
    void GrowArray()
    {
        capacity *= GROWTH_FACTOR;

        // Dynamically create a new array.
        Student* new_array = new Student[capacity];

        // Copy all previous data from old array into the new array.
        for (int i = 0; i < length; i++)
        { 
            new_array[i] = array[i];
        }

        // Release the memory occupied by old array. delete[] used for deleting dynamic array of structs.
        delete[] array;

        // Give the same name to the new array as the previous one.
        array = new_array;
    }
    
    // Helper fucntion for Insert. Inserts data into the student array.
    void AppendToArray(long long id, string name, float CGPA, string address)
    {
        array[length].id = id;
        array[length].name = name;
        array[length].CGPA = CGPA;
        array[length].address = address;
    }
    
    // Helper function for delete. Shifts elements when needed after deletion.
    void FillRoom(int index)
    {
        for (int i = index; i < length - 1; i++)
        {
            array[i] = array[i + 1];
        }
    }

    // Helper function for delete. Shrinks array when needed.
    void ShrinkArray()
    {
        capacity /= GROWTH_FACTOR;
        
        // Dynamically create a new array.
        Student* new_array = new Student[capacity];
        
        // Copy over all data from old array into the new array.
        for (int i = 0; i < length; i++)
        { 
            new_array[i] = array[i];
        }

        // Release the memory occupied by old array. delete[] used for deleting dynamic array of structs.
        delete[] array;

        // Give the same name to the new array as the previous one.
        array = new_array;
    } 
    
    public:

    /* Insert a student into the array. 
    Array dynamically grows by initial size if needed.
    Insertion is always at the end of tha array. */

    int Insert(long long id, string name, float CGPA, string address)
    {
        // ID must be a positive integer.
        if (id < 0) return 1;
        
        // ID must always be unique. Check for whether a student with same id already exists or not.
        if (Search(id) != -1) return 2;

        // CGPA must be within the range 1-4 inclusive.
        if (CGPA < 1.0 || CGPA > 4.0) return 3;

        // If the array has capacity to store new structs.
        if (length < capacity)
        {
            AppendToArray(id, name, CGPA, address);
            length++;
        }
        // If array runs out of space to store new incoming data.
        else
        {
            // Dynamically grow array.
            GrowArray();

            // Insert new data into the new array.
            AppendToArray(id, name, CGPA, address);
            length++;
        }

        return 0;
    }

    /* Delete a student from the array. Array dynamically shrinks if needed.

    Dynamically shrinking array when length reaches quarter (1/4) of capacity 
    instead of half (1/2) to avoid worst case O(n^2) operations. Consider edge case 
    in which insertion forces array growth and an immidiate delete operation after it 
    forces shrink of dynamic array. If we continue these operations n times (i.e. consider 
    the case dynamic array is used as a stack in which such scenario is plausible), we
    reach O(n^2) worst case. So, an optimized resizing scheme is to grow twice the size
    at full capacity and shrink array in half only when size(length) drops to 1/4 of its 
    max size or capacity so we never encounter such a case. This also helps us achieve 
    amortized O(1) time complexity instead of average O(1).

    */
    int Delete(long long id)
    {
        // Error codes.
        if (length == 0) return 1;
        if (id < 0) return 2;

        // Search returns location with respect to list not w.r.t array.
        int index = Search(id) - 1;

        // Deleting from end of list
        if (index + 1 == length)
        {
            // Condition for shrinking array.
            if ((length <= capacity / 4))
            {
                length--;
                ShrinkArray();
            }

            // Shrink not required
            else length--;
        }

        // Deletion from middle and dynamic shrinking if required.
        else
        {
            FillRoom(index);
            length--;
            if (length <= capacity / 4) 
                ShrinkArray();          
        }
        return 0;
    }


    // Search with student id.
    int Search(long long id)
    {
        // ID can only be a positive integer.
        if (length == 0 || id < 0) return -1;

        for (int i = 0; i < length; i++)
        {
            if (array[i].id == id) return i + 1;
        }

        return -1;
    }
    
    // Replace data of an existing student.
    int Replace(long long id, int new_id, string name, float CGPA, string address)
    {
        if (length == 0) return 1;
        if (id < 0) return 2;
        // Search returns location with respect to list not w.r.t array.
        int index = Search(id) - 1;
        
        // In case a student with this id does not exist.
        if (index == -2) return 3;

        // In case the new ID is already taken.
        if (Search(new_id) != -1) return 4;

        // Replace existing data with new data.
        else
        {
            array[index].id = new_id;
            array[index].name = name;
            array[index].CGPA = CGPA;
            array[index].address = address;
        }
        return 0;
    }

    // Replace data at a specific index.
    int ReplaceAtIndex(int location, int new_id, string name, float CGPA, string address)
    {
        // Error codes.
        if (length == 0) return 1;
        if (location > length || location < 1) return 2;
        
        // In case the new ID is already taken.
        if (Search(new_id) != -1) return 4;
        else
        {
            array[location - 1].id = new_id;
            array[location - 1].name = name;
            array[location - 1].CGPA = CGPA;
            array[location - 1].address = address;
        }
        return 0;
    }

    // Display the vector as a list to the user.
    void Print()
    {
        cout << "[";
        for(int i = 0; i < length; i++)
        {
            if (i % 2 == 0 && i != 0) cout << endl;
            if (i == 0) 
                cout << " " << (i + 1) << ". " <<" { " << array[i].id << ", " 
                << array[i].name << ", " << array[i].CGPA << ", " << array[i].address << "}";
            else 
                cout << ", " << (i + 1) << ". {" << array[i].id  << ", " 
                << array[i].name << ", " << array[i].CGPA  << ", " << array[i].address << "}";  
        }
        cout << " ]" << endl;
        return;
    }

    // Print one element of vector.
    void PrintElement(int location)
    {
        cout << "[ {" ;
        cout << array[location - 1].id << ", " << array[location - 1].name << ", " << array[location - 1].CGPA; 
        cout << ", " << array[location - 1].address << "} ]" << endl;
        return;
    }

    // Getter methods.
    int getLength()
    {
        return length;
    }

    int getCapacity()
    {
        // For showing it in a logical manner to user.
        if (capacity == length)
        {
            return capacity * GROWTH_FACTOR;
        }
        return capacity;
    }
   

}; // end Vector Class


// Function prototypes.
float get_number(string prompt);
string get_string(string prompt);
void validateCGPA(float* CGPA);
void validateID(long long* id);
void validateLocation(int* location, Vector* object);

int main(void)
{
    // Main menu being shown to user.

    cout << "\n----MAIN MENU------\n" << endl;

    cout << "\n---Operations----\n";
    cout << "1. Insert a new student." << endl;
    cout << "2. Delete a student." << endl;
    cout << "3. Search for an existing student." << endl; 
    cout << "4. Replace an existing student." << endl;
    cout << "5. Replace a student at a specific location from the list." << endl;
    cout << "6. Terminate program." << endl;
    
    Vector list;


    while (true)
    {
        // Getting user input.
        int user_choice;
        cout << "\n------------------------------------------------" << endl;
        cout << "LIST INFO\n";
        cout << "List Format = {<Student ID>, <Student Name>, <CGPA>, <Address>}" << endl;
        cout << "\nList = "; list.Print();
        cout << "Length: " << list.getLength() << endl;
        cout << "Capacity: " << list.getCapacity() << endl;
        cout << "------------------------------------------------" << endl;
        user_choice = (int) get_number("\nSelect an operation from main menu: ");
        
        // Creating variables.
        long long id = -1;
        string name;
        float CGPA = -1;
        string address;
        int status;
        int location;
        long long new_id;

        // Switch case structure to perform user-inputted actions.
        switch (user_choice)
        {
        
        // Insertion
        case 1:
            cout << "\n---INSERTION---\n" << endl;
            cout << "----Student Information----" << endl;
            
            id = get_number("ID: ");
            validateID(&id);
            
            name = get_string("Name: ");
            
            CGPA = get_number("CGPA: ");
            validateCGPA(&CGPA);
            
            address = get_string("Address: ");
            
            status = list.Insert(id, name, CGPA, address);
            if (status == 2) cout << "Insertion not possible. A student with entered ID already exists." << endl;
            break;

        // Deletion
        case 2:
            if (list.getLength() == 0)
            {
                cout << "\nDeletion not possible. List is currently empty.\n" << endl;
                break;
            }

            cout << "\n---DELETE---\n" << endl;
            id = get_number("Enter Student ID: ");
            if (list.Search(id) == -1) cout << "\nDeletion not possible. No such student exists.\n";
            else list.Delete(id);
            break;
        
        // Search
        case 3:
            if (list.getLength() == 0)
            {
                cout << "\nSearch not possible. List is currently empty.\n" << endl;
                break;
            }
            cout << "\n---SEARCH---\n" << endl;
            id = get_number("Enter Student ID: ");
            location = list.Search(id);
            if (location == -1) cout << "\nNo such student exists.\n";

            else
            {
                cout << "\nStudent Found." << endl;
                cout << "Location in list: " << location << endl;
                cout << "Details = ";
                list.PrintElement(location);
            }
            break;
        
        // Replace
        case 4:
            if (list.getLength() == 0)
            {
                cout << "\nReplace not possible. List is currently empty.\n" << endl;
                break;
            }
            cout << "\n---REPLACE---\n" << endl;
            id = get_number("Enter Old Student ID: ");
            cout << "-New Student-\n" << endl;
            
            new_id = get_number("New ID: ");
            validateID(&new_id);

            name = get_string("Name: "); 

            CGPA = get_number("CGPA: ");
            validateCGPA(&CGPA);

            address = get_string("Address: ");

            status = list.Replace(id, new_id, name, CGPA, address);
            if (status == 3) cout << "\nReplace not possible. No student with entered ID exists.\n";
            else if (status == 4) cout << "\nReplace not possible. A student with entered new ID already exists.\n";
            break;
        
        // Replace At
        case 5:
            if (list.getLength() == 0)
            {
                cout << "\nReplace not possible. List is currently empty.\n" << endl;
                break;
            }
            
            cout << "\n---REPLACE AT LOCATION---\n" << endl;
            location = get_number("Enter list location: ");
            validateLocation(&location, &list);

            cout << "Old student = ";
            list.PrintElement(location);

            cout << "-New Student-\n" << endl;
            
            new_id = get_number("New ID: ");
            validateID(&new_id);

            name = get_string("Name: "); 

            CGPA = get_number("CGPA: ");
            validateCGPA(&CGPA);

            address = get_string("Address: ");

            status = list.ReplaceAtIndex(location, new_id, name, CGPA, address);
            if (status == 0)
            {
            cout << "Replaced with = ";
            list.PrintElement(location);
            }
            else if (status == 4) cout << "\nReplace not possible. A student with entered new ID already exists.\n";
            break;

        // End Program
        case 6:
            cout << "\nProgram Terminated.\n" << endl;
            return 0;

        // Validate user choice
        default:
            cout << "\nSelect an appropriate option from the menu.";
            break;
        }

    }
    return 0;
} // end Main


// Helper Functions for Main.

// Getting a valid number from user.
float get_number(string prompt)
{
    float number;
    cout << prompt;
    
    // In case user does not enter a number.
    while(!(cin  >> number))
    {
        // Clearing error flags on cin object.
        cin.clear();

        // Ignoring all of the characters user entered up until the delimiter '\n'.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Wrong Input Type. Please enter a numeric value." << endl;
        cout << prompt;
    }

    // Clearing out the '\n' from input stream so that it does not become a problem for future inputs.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return number;
}

// String input from user.
string get_string(string prompt)
{
    string str;
    cout << prompt;
    getline(cin , str);
    return str;
}

// Validating list location
void validateLocation(int* location, Vector* object)
{
    while (*location > object -> getLength() || *location < 1)
    {
        cout << "Please enter a valid list location." << endl;
        *location = get_number("List Location: ");
    }
}

// Validate student ID
void validateID(long long* id)
{
    while (*id < 0)
    {
        cout << "Student ID must be a positive whole number." << endl;
        *id = get_number("ID: ");
    }
}

// Validate CGPA
void validateCGPA(float* CGPA)
{
    while (*CGPA < 1.0 || *CGPA > 4.0)
    {
        cout << "CGPA must lie in the range 1-4 inclusive." << endl;
        *CGPA = get_number("CGPA: ");
    }
}