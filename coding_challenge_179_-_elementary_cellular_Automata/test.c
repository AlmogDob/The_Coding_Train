#include <stdio.h>

void number2binary(int number, char* binary, int min_length);

int main()
{
    char buffer[256];

    number2binary(5, buffer, 5);

    printf("%s\n", buffer);

    return 0;
}

void number2binary(int number, char* binary, int min_length)
{
    int counter = 0; 
    char temp;

    while (number) {
        if (number%2) {
            binary[counter++] = '1';
        } else {
            binary[counter++] = '0';
        }

        number = number/2;
    }

    while (counter < min_length) {
        binary[counter] = '0';
        counter++;
    }

    for (int i = 0; i < counter/2; i++) {
        temp = binary[i];
        binary[i] = binary[counter-i-1];
        binary[counter-i-1] = temp;
    }

    binary[counter] = '\0';
    
}
