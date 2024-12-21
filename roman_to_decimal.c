#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to check if a string is a valid Roman numeral
int isValidRoman(char *roman) {
    // Loop through each character in the Roman numeral string
    for (int i = 0; roman[i] != '\0'; i++) {
        // Check if the character is one of the valid Roman numeral symbols
        if (roman[i] != 'I' && roman[i] != 'V' && roman[i] != 'X' &&
            roman[i] != 'L' && roman[i] != 'C' && roman[i] != 'D' &&
            roman[i] != 'M') {
            return 0;  // Return 0 if an invalid character is found
        }
    }
    return 1;  // Return 1 if all characters are valid
}

// Function to convert Roman numerals to decimal numbers
int romanToDecimal(char *roman) {
    int total = 0;  // Variable to store the total decimal value

    // Loop through each character in the Roman numeral string
    for (int i = 0; roman[i] != '\0'; i++) {
        int current = 0;  // Current Roman numeral's value

        // Assign value based on the Roman numeral symbol
        if (roman[i] == 'I') current = 1;
        else if (roman[i] == 'V') current = 5;
        else if (roman[i] == 'X') current = 10;
        else if (roman[i] == 'L') current = 50;
        else if (roman[i] == 'C') current = 100;
        else if (roman[i] == 'D') current = 500;
        else if (roman[i] == 'M') current = 1000;

        // Check for subtraction cases (e.g., IV, IX)
        if (roman[i + 1] != '\0') {
            if (roman[i] == 'I' && (roman[i + 1] == 'V' || roman[i + 1] == 'X')) current = -1;
            if (roman[i] == 'X' && (roman[i + 1] == 'L' || roman[i + 1] == 'C')) current = -10;
            if (roman[i] == 'C' && (roman[i + 1] == 'D' || roman[i + 1] == 'M')) current = -100;
        }
    
        total += current;  // Add the current value to the total
    }
    return total;  // Return the total decimal value
}

// Function to convert a number to its English word representation
void numberToWords(int num, char *output) {
    // Arrays for word representation of numbers
    char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million", "Billion"};

    if (num == 0) {
        strcpy(output, "Zero");  // Special case for zero
        return;
    }

    char result[1000] = "";  // Temporary string to store the result
    char temp[1000] = "";    // Temporary string for current group

    int isNegative = 0;  // Flag for negative numbers
    if (num < 0) {
        isNegative = 1;
        num = -num;  // Make the number positive for processing
    }

    int part, group = 0;  // Variables for number grouping
    while (num > 0) {
        part = num % 1000;  // Extract the last three digits
        num /= 1000;        // Remove the last three digits

        if (part > 0) {
            char tempGroup[100] = "";  // Temporary storage for the current group

            // Process the hundreds place
            if (part / 100 > 0) {
                strcat(tempGroup, ones[part / 100]);
                strcat(tempGroup, " Hundred ");
                part %= 100;
            }

            // Process the tens and ones place
            if (part >= 10 && part < 20) {
                strcat(tempGroup, teens[part - 10]);  // Special case for teens
            } else {
                if (part / 10 > 0) {
                    strcat(tempGroup, tens[part / 10]);
                    strcat(tempGroup, " ");
                }
                if (part % 10 > 0) {
                    strcat(tempGroup, ones[part % 10]);
                }
            }

            // Add the group name (Thousand, Million, etc.)
            if (group > 0) {
                strcat(tempGroup, " ");
                strcat(tempGroup, thousands[group]);
            }

            // Combine with the result so far
            if (strlen(result) > 0) {
                strcat(tempGroup, " ");
                strcat(tempGroup, result);
            }
            strcpy(result, tempGroup);
        }
        group++;  // Move to the next group (thousands, millions, etc.)
    }

    // Add "Negative" prefix if the number was negative
    if (isNegative) {
        char negativeResult[1000] = "Negative ";
        strcat(negativeResult, result);
        strcpy(result, negativeResult);
    }

    // Remove trailing space if present
    int len = strlen(result);
    if (len > 0 && result[len - 1] == ' ') {
        result[len - 1] = '\0';
    }

    strcpy(output, result);  // Copy the final result to the output
}

// Function to process each line from the input file
void processFile(FILE *inputFile, FILE *outputFile) {
    char line[256];  // Buffer to hold each line from the file

    // Read each line from the input file
    while (fgets(line, sizeof(line), inputFile)) {
        line[strcspn(line, "\n")] = '\0';  // Remove the newline character
        char roman1[100], roman2[100], operation;

        // Parse the input line into two Roman numerals and an operation
        if (sscanf(line, "%s %c %s", roman1, &operation, roman2) != 3) {
            printf("Invalid input format: %s\n", line);  // Handle invalid format
            fprintf(outputFile, "Invalid input format: %s\n", line);
            continue;
        }

        // Validate the Roman numerals
        if (!isValidRoman(roman1) || !isValidRoman(roman2)) {
            printf("Invalid Roman numeral in line: %s\n", line);
            fprintf(outputFile, "Invalid Roman numeral in line: %s\n", line);
            continue;
        }

        // Validate the operation
        if (operation != '+' && operation != '-' && operation != '*' && operation != '/') {
            printf("Invalid operation in line: %s\n", line);
            fprintf(outputFile, "Invalid operation in line: %s\n", line);
            continue;
        }

        // Convert Roman numerals to decimal numbers
        int num1 = romanToDecimal(roman1);
        int num2 = romanToDecimal(roman2);
        int result;

        // Perform the requested operation
        if (operation == '*') result = num1 * num2;
        else if (operation == '+') result = num1 + num2;
        else if (operation == '-') result = num1 - num2;
        else if (operation == '/') {
            if (num2 == 0) {  // Handle division by zero
                printf("Division by zero error in line: %s\n", line);
                fprintf(outputFile, "Division by zero error in line: %s\n", line);
                continue;
            }
            result = num1 / num2;
        }

        // Convert the result to words
        char wordResult[1000];
        numberToWords(result, wordResult);

        // Print and write the result to the output file
        printf("Process: %d %c %d = %d\n", num1, operation, num2, result);
        fprintf(outputFile, "%s\n", wordResult);
    }
}

// Main function
int main() {
    // Open the input and output files
    FILE *inputFile = fopen("input.txt", "r");
    FILE *outputFile = fopen("output.txt", "w");

    // Check if files were opened successfully
    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening files.\n");
        return 1;  // Exit the program with an error code
    }

    // Process the input file
    processFile(inputFile, outputFile);

    // Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("Processing complete. Check output.txt for word results.\n");
    return 0;  // Exit the program successfully
}
