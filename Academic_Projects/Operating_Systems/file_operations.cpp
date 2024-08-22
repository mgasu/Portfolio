//
//  file_operations.cpp
//  CSE330_Assignment5
//
//  Created by Maria Garcia on 11/17/23.
//

//#include "file_operations.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>


namespace fs = std::filesystem;

// Define a function to list all the files in the current directory
void listFiles() {
    fs::path currentPath = fs::current_path(); // Get the current working directory

    try {
        std::cout << "Current path is " << currentPath << "\n";
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            if (fs::is_regular_file(entry)) {
                std::cout << "File: " << entry.path().filename() << "\n";
            } else if (fs::is_directory(entry)) {
                std::cout << "Directory: " << entry.path().filename() << "\n";
            }
        }
    } catch (const fs::filesystem_error& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return;
}


int main() {
    std::string filename = "assignment5_output.txt";
    std::ofstream file;

    std::string data = "\tHello, File System!\n\t2023Fall CSE330\n\tAssignment 5\n\tLearning File System\n";

    // TODO 1: Create and open a file for writing (use file and filename)
    // Note: For open() operation, if the file already exists, its content will be cleared
    // Hint: Only one line of code is needed
    file.open(filename);


    // TODO 2: use '<<' to write the data variable to the file
    // Hint: Only one line of code is needed
    file << data;


    // TODO 3: Close the file
    // Hint: Only one line of code is needed
    file.close();


    std::cout << "File created and written successfully." << std::endl;
    listFiles();
    std::cout << std::endl;

    // TODO 4: Use std::ifstream to open the previous file for reading and modifying
    // Hint: Only one line of code is needed
    std::ifstream inputFile(filename);


    std::string fileContent;
    std::string line;
    // TODO 5: Use while loop and std::getline to read the file content line by line
    while (std::getline(inputFile, line)) {
            fileContent += line + "\n";
        }


    // TODO 6: Close the file
    // Hint: Only one line of code is needed
    inputFile.close();


    // Print the file content
    std::cout << "File content:" << std::endl;
    std::cout << fileContent << std::endl;
    std::cout << std::endl;

    // Modify the content by finding and replacing a substring
    std::string searchString = "File System";


    // TODO 7: Replace "Your Name" with your name
    std::string replaceString = "Maria Garcia";

    // TODO 8: use find() to find and use replace() to replace the string
    // Hint: use a while loop to replace all occurrences of the searchString: e.g. while (find_result != std::string::npos)
    // Hint: use source_string.replace(position, length, replaceString) to replace the substring
    size_t findResult = fileContent.find(searchString);
        while (findResult != std::string::npos) {
            fileContent.replace(findResult, searchString.length(), replaceString);
            findResult = fileContent.find(searchString, findResult + replaceString.length());
        }



    // Print the modified content
    std::cout << "Modified file content:" << std::endl;
    std::cout << fileContent << std::endl;
    std::cout << std::endl;

    // TODO 9: update the fileContent to the file, then close it
    // Hint: use open() again
    std::ofstream outputFile(filename);
        outputFile << fileContent;
        outputFile.close();



    std::string newFilename = "assignment5_renamed.txt";
    // TODO 10: use std::rename(oldname, newname) to rename the file
    // Hint: you may need to convert the filename to c_str() before passing it to std::rename()
    std::rename(filename.c_str(), newFilename.c_str());




    // test if the file is renamed successfully
    std::ifstream renamedFile(newFilename);
    if (!renamedFile) {
        std::cerr << "Failed to open the renamed file." << std::endl;
        return 1;
    }
    // read the rename file content and then print it
    std::string renamedFileContent;
    std::string renamedLine;
    while (std::getline(renamedFile, renamedLine)) {
        renamedFileContent += renamedLine + "\n";
    }
    renamedFile.close();
    std::cout << "New file name: " << newFilename << std::endl;
    std::cout << "Renamed file content:" << std::endl;
    std::cout << renamedFileContent << std::endl;
    std::cout << std::endl;

    listFiles();
    return 0;
}
