// Copyright 2025 Anisimov
#include "main.h"

#include "textgen.h"

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  const int NPREF = 2;
  const int MAXGEN = 1200;

  TextGenerator generator(NPREF);

  std::string inputFile = "input.txt";
  if (argc > 1) {
    inputFile = argv[1];
  }

  std::cout << "Reading file: " << inputFile << std::endl;
  if (!generator.buildFromFile(inputFile)) {
    std::cerr << "Failed to build table. Check input file." << std::endl;
    return 1;
  }
  std::cout << "Table built. Number of prefixes: "
            << generator.getTable().size() << std::endl;

  std::string generatedText = generator.generate(MAXGEN);

  std::ofstream outFile("result/gen.txt");
  if (outFile.is_open()) {
    outFile << generatedText;
    outFile.close();
    std::cout << "Generated text saved to result/gen.txt" << std::endl;
  } else {
    std::cerr << "Error writing to result/gen.txt" << std::endl;
    return 1;
  }

  std::cout << "\n--- First 300 characters of generated text ---\n";
  std::cout << generatedText.substr(0, 300) << "...\n";

  return 0;
}
