#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LETTERS 10  // Maximum number of unique letters (digits 0-9)

char letters[MAX_LETTERS];  // Array to store unique letters
int digits[MAX_LETTERS];     // Array to store assigned digits
int used[10];                // To track used digits (0-9)
int letterCount;             // Number of unique letters

// Function to convert a word into its numerical value based on current letter-to-digit mapping
int wordValue(const char* word) {
    int value = 0;
    for (int i = 0; word[i]; i++) {
        value = value * 10 + digits[word[i] - 'A'];  // Convert letter to digit
    }
    return value;
}

// Recursive backtracking function to find the solution
int solve(int index, const char* lhs1, const char* lhs2, const char* rhs) {
    // If all letters are assigned, check the equation
    if (index == letterCount) {
        int left1 = wordValue(lhs1);
        int left2 = wordValue(lhs2);
        int right = wordValue(rhs);
        // Check if left1 + left2 == right
        if (left1 + left2 == right) {
            printf("Solution found:\n");
            for (int i = 0; i < letterCount; i++) {
                printf("%c = %d\n", letters[i], digits[i]);
            }
            return 1;  // Solution found
        }
        return 0;  // Not a valid solution
    }

    // Iterate through all possible digits
    for (int d = 0; d < 10; d++) {
        if (!used[d]) {  // If digit d is not used
            used[d] = 1;         // Mark the digit as used
            digits[index] = d;   // Assign digit to the current letter

            // Check for leading zero constraints
            if ((letters[index] == lhs1[0] || letters[index] == lhs2[0] || letters[index] == rhs[0]) && d == 0) {
                used[d] = 0;  // Backtrack if leading digit is zero
                continue;
            }

            // Recur to assign the next letter
            if (solve(index + 1, lhs1, lhs2, rhs)) {
                return 1;  // Solution found in recursive call
            }

            // Backtrack
            used[d] = 0;  // Mark the digit as unused
        }
    }
    return 0;  // No solution found
}

// Function to find unique letters in the equation
void findUniqueLetters(const char* equation) {
    while (*equation) {
        if (*equation >= 'A' && *equation <= 'Z') {
            int found = 0;
            for (int i = 0; i < letterCount; i++) {
                if (letters[i] == *equation) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                letters[letterCount++] = *equation;  // Add unique letter
            }
        }
        equation++;
    }
}

// Function to split the input equation into two parts (left and right side of the equation)
void splitEquation(const char* equation, char* lhs1, char* lhs2, char* rhs) {
    char* plusPos = strchr(equation, '+');
    char* equalsPos = strchr(equation, '=');
    
    // Extracting left-hand side words
    strncpy(lhs1, equation, plusPos - equation);
    lhs1[plusPos - equation] = '\0';
    strncpy(lhs2, plusPos + 1, equalsPos - (plusPos + 1));
    lhs2[equalsPos - (plusPos + 1)] = '\0';
    strcpy(rhs, equalsPos + 1);  // Right-hand side
}

int main() {
    // Define variables for user input
    char equation[100]; // Buffer for the input equation
    char lhs1[50], lhs2[50], rhs[50];

    // Read the equation from user input
    printf("Enter the cryptarithmetic puzzle (e.g., SEND+MORE=MONEY): ");
    scanf("%s", equation);

    // Split the equation into left and right parts
    splitEquation(equation, lhs1, lhs2, rhs);

    // Initialize the letters array
    letterCount = 0;
    findUniqueLetters(equation);

    // Solve the puzzle
    if (!solve(0, lhs1, lhs2, rhs)) {
        printf("No solution found.\n");
    }

    return 0;
}

/*
Example Usage:
Enter the cryptarithmetic puzzle (e.g., SEND+MORE=MONEY): SEND+MORE=MONEY

Output:
Solution found:
S = 9
E = 5
N = 6
D = 7
M = 1
O = 0
R = 8
Y = 2
*/
