// Copyright 2025 Anisimov
#include "textgen.h"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

TextGenerator::TextGenerator(int npref)
    : NPREF(npref), rng(std::time(nullptr)) {}

void TextGenerator::clear() {
  statetab.clear();
}

std::vector<std::string> TextGenerator::splitToWords(const std::string& text) {
  std::vector<std::string> words;
  std::istringstream iss(text);
  std::string word;
  while (iss >> word) {
    if (!word.empty()) {
      words.push_back(word);
    }
  }
  return words;
}

bool TextGenerator::buildFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: cannot open file " << filename << std::endl;
    return false;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content = buffer.str();
  file.close();

  std::vector<std::string> words = splitToWords(content);
  if (words.size() <= static_cast<size_t>(NPREF)) {
    std::cerr << "File too small (words <= NPREF)" << std::endl;
    return false;
  }

  for (size_t i = 0; i + NPREF < words.size(); ++i) {
    Prefix prefix;
    for (int j = 0; j < NPREF; ++j) {
      prefix.push_back(words[i + j]);
    }
    std::string suffix = words[i + NPREF];
    statetab[prefix].push_back(suffix);
  }

  return true;
}

std::string TextGenerator::randomSuffix(const std::vector<std::string>& suffixes) {
  if (suffixes.empty()) return "";
  std::uniform_int_distribution<size_t> dist(0,
      suffixes.size() - 1);
  return suffixes[dist(rng)];
}

std::string TextGenerator::generate(int maxWords) {
  if (statetab.empty()) {
    return "[Error] Table is empty. Call buildFromFile() first.";
  }

  auto it = statetab.begin();
  std::uniform_int_distribution<size_t> dist(0, statetab.size() - 1);
  std::advance(it, dist(rng));
  return generateFrom(it->first, maxWords);
}

std::string TextGenerator::generateFrom(const Prefix& start, int maxWords) {
  if (statetab.empty()) {
    return "[Error] Table is empty. Call buildFromFile() first.";
  }

  Prefix current = start;
  std::vector<std::string> result;
  for (const auto& w : current) {
    result.push_back(w);
  }

  for (int i = 0; i < maxWords - NPREF; ++i) {
    auto findIt = statetab.find(current);
    if (findIt == statetab.end() || findIt->second.empty()) {
      break;
    }
    std::string nextWord = randomSuffix(findIt->second);
    result.push_back(nextWord);
    current.pop_front();
    current.push_back(nextWord);
  }

  std::string output;
  for (const auto& w : result) {
    output += w + " ";
  }
  return output;
}

void TextGenerator::addRule(const Prefix& prefix, const std::string& suffix) {
  statetab[prefix].push_back(suffix);
}
