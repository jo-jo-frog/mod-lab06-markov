#ifndef TEXTGEN_H
#define TEXTGEN_H

#include <iostream>
#include <string>
#include <deque>
#include <map>
#include <vector>
#include <random>

typedef std::deque<std::string> Prefix;

typedef std::map<Prefix, std::vector<std::string>> Statetab;

class TextGenerator {
public:
    explicit TextGenerator(int npref = 2);
    
    bool buildFromFile(const std::string& filename);
    
    std::string generate(int maxWords);
    
    void clear();
    
    void addRule(const Prefix& prefix, const std::string& suffix);
    
    const Statetab& getTable() const { return statetab; }
    
    int getPrefixSize() const { return NPREF; }
    
private:
    int NPREF;
    Statetab statetab;
    std::mt19937 rng;
    
    std::vector<std::string> splitToWords(const std::string& text);
    
    std::string randomSuffix(const std::vector<std::string>& suffixes);
};

#endif
