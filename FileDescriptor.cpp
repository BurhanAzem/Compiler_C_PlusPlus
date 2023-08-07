#include <iostream>
#include <fstream>
#include <cstring> 

#define BUFFER_SIZE 256

class FileDescriptor {
private:
    FILE* fp;
    char* file;
    char ungetBuffer;
    int line_number;
    int char_number;
    int buf_size;
    char* buffer;
    char last_char;
public:
    FileDescriptor(const char* filename) {
        file = _strdup(filename);
        std::cout << "Attempting to open file: " << file << std::endl; // Debug statement
        errno_t err = fopen_s(&fp, file, "r");
        if (err != 0) {
            perror("Error opening fil");
            free(file);
            exit(EXIT_FAILURE);
        }
        buf_size = BUFFER_SIZE;
        buffer = (char*)malloc(buf_size * sizeof(char));
        ungetBuffer = '\0';
        line_number = 1;
        char_number = 0; // Corrected the initialization here
    }

    ~FileDescriptor() {
        fclose(fp);
        free(file);
        free(buffer); // Free the buffer memory
    }

    char readChar() {
        if (ungetBuffer != '\0') {
            char ch = ungetBuffer;
            ungetBuffer = '\0';
            return ch;
        }

        char ch = fgetc(fp);
        if (ch == EOF) {
            // Handle end of file (EOF)
            return ch;
        }

        if (ch == '\n') {
            buffer[char_number] = '\0'; // Null-terminate the buffer to represent the end of the line
            char_number = 0; // Reset the character number for the next line
            line_number++;
            //std::cout << "\n--------" << buffer; // Process or print the line

        }
        else {
            buffer[char_number++] = ch; // Store the character in the buffer
            if (char_number >= buf_size - 1) {
                // Resize the buffer if it's about to overflow
                buf_size *= 2;
                buffer = (char*)realloc(buffer, buf_size * sizeof(char));
                if (buffer == nullptr) {
                    perror("Error reallocating memory for buffer");
                    exit(EXIT_FAILURE);
                }
            }
        }

        return ch;
    }


    void unget() {
        if (ungetBuffer != '\0') {
            std::cerr << "Error: Two ungets in a row are not allowed." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (char_number > 0) {
            ungetBuffer = buffer[char_number - 1]; // Store the last read character in the unget buffer
            char_number--; // Decrement char_number after using it

        }
    }

    char peek() {
        char ch = readChar();
        unget();
        return ch;
    }

    int getLineNumber() const {
        return line_number;
    }

    void reportError(const char* message) const {
        fprintf(stderr, "%s\n", buffer);
        for (int i = 0; i < char_number; i++)
            fprintf(stderr, " ");
        fprintf(stderr, "^\n");
        fprintf(stderr, "Error: \"%s\" on line %d of %s\n", message, line_number, file);
    }
};
