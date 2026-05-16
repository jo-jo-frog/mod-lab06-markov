#include "textgen.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    const int NPREF = 2;
    const int MAXGEN = 1200;
    
    TextGenerator generator(NPREF);
    
    std::string inputFile = "input.txt";
    if (argc > 1) {
        inputFile = argv[1];
    }
    
    std::cout << "Чтение файла: " << inputFile << std::endl;
    if (!generator.buildFromFile(inputFile)) {
        std::cerr << "Не удалось построить таблицу. Проверьте файл." << std::endl;
        return 1;
    }
    std::cout << "Таблица успешно построена. Количество префиксов: " << generator.getTable().size() << std::endl;
    
    std::string generatedText = generator.generate(MAXGEN);
    
    std::ofstream outFile("result/gen.txt");
    if (outFile.is_open()) {
        outFile << generatedText;
        outFile.close();
        std::cout << "Текст успешно сгенерирован и сохранён в result/gen.txt" << std::endl;
    } else {
        std::cerr << "Ошибка записи в result/gen.txt" << std::endl;
        return 1;
    }
    
    std::cout << "\n--- Начало сгенерированного текста (первые 300 символов) ---\n";
    std::cout << generatedText.substr(0, 300) << "...\n";
    std::cout << "--- Конец фрагмента ---\n";
    
    return 0;
}
