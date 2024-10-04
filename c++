#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring> // Aby używać strncmp

#pragma pack(push, 1) // Wyłączamy wyrównywanie struktur

// Struktura do przechowywania informacji nagłówka pliku GRIB
struct GribHeader {
    char identifier[4];   // "GRIB" (kody ASCII)
    uint32_t messageSize; // Całkowita długość wiadomości
    uint8_t version;      // Wersja GRIB (powinna wynosić 1 dla GRIB1)
};

// Struktura do przechowywania danych sekcji produktu (Section 1)
struct GribProductData {
    uint32_t sectionSize;          // Długość sekcji
    uint8_t parameterTableVersion; // Wersja tabeli parametrów
    uint8_t originatingCenter;     // Centrum generujące dane
    uint8_t generatingProcess;     // Proces generowania danych
    uint8_t gridID;                // Identyfikacja siatki
    uint8_t flag;                  // Flaga wskazująca obecność GDS i BMS
    uint16_t parameter;            // Identyfikator parametru
    uint8_t levelType;             // Typ poziomu
    uint16_t levelValue;           // Wartość poziomu
    uint16_t year;                 // Rok
    uint8_t month;                 // Miesiąc
    uint8_t day;                   // Dzień
    uint8_t hour;                  // Godzina
    uint8_t minute;                // Minuta
    uint8_t timeUnit;              // Jednostka czasu
    uint8_t period1;               // Okres 1
    uint8_t period2;               // Okres 2
    uint8_t timeRangeIndicator;    // Wskaźnik zakresu czasowego
    uint8_t avgCount;              // Liczba uśrednień
    uint8_t missingDataCount;      // Liczba brakujących danych
    uint8_t century;               // Wiek
    uint8_t subCenter;             // Podcentrum
    int16_t decimalScaleFactor;    // Skala dziesiętna
};

#pragma pack(pop) // Przywracamy domyślne wyrównanie

// Funkcja do odczytu nagłówka GRIB (sekcja 0)
void loadGribHeader(std::ifstream& inputFile, GribHeader& header) {
    inputFile.read(reinterpret_cast<char*>(&header), sizeof(GribHeader));
}

// Funkcja do odczytu sekcji produktu (sekcja 1)
void loadProductData(std::ifstream& inputFile, GribProductData& data) {
    inputFile.read(reinterpret_cast<char*>(&data), sizeof(GribProductData));
}

// Funkcja wyświetlająca informacje o nagłówku pliku
void displayGribHeader(const GribHeader& header) {
    std::cout << "==========================\n";
    std::cout << "== Informacje o wiadomości GRIB ==\n";
    std::cout << "==========================\n";
    std::cout << "Wersja GRIB: " << static_cast<int>(header.version) << "\n";
    std::cout << "Długość wiadomości: " << header.messageSize << "\n";
}

// Funkcja wyświetlająca dane sekcji 1 (produkt)
void displayProductData(const GribProductData& data) {
    std::cout << "\n======================\n";
    std::cout << "== Dane sekcji produktu (Sekcja 1) ==\n";
    std::cout << "======================\n";
    std::cout << "Centrum: " << static_cast<int>(data.originatingCenter) << "\n";
    std::cout << "Proces generowania: " << static_cast<int>(data.generatingProcess) << "\n";
    std::cout << "Rok: " << data.year << "\n";
    std::cout << "Miesiąc: " << static_cast<int>(data.month) << "\n";
    std::cout << "Dzień: " << static_cast<int>(data.day) << "\n";
    std::cout << "Godzina: " << static_cast<int>(data.hour) << "\n";
    std::cout << "Minuta: " << static_cast<int>(data.minute) << "\n";
    std::cout << "Poziom: " << static_cast<int>(data.levelType) << " na " << data.levelValue << "\n";
}

int main() {
    // Wczytujemy ścieżkę do pliku
    std::string filePath;
    std::cout << "Podaj ścieżkę do pliku GRIB1: ";
    std::cin >> filePath;

    // Otwieramy plik binarny
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Nie udało się otworzyć pliku: " << filePath << "\n";
        return 1;
    }

    // Odczytujemy nagłówek GRIB (sekcja 0)
    GribHeader header;
    loadGribHeader(inputFile, header);

    // Sprawdzamy, czy plik jest faktycznie plikiem GRIB
    if (strncmp(header.identifier, "GRIB", 4) != 0) {
        std::cerr << "To nie jest plik GRIB.\n";
        return 1;
    }

    // Wyświetlamy informacje z nagłówka
    displayGribHeader(header);

    // Odczytujemy dane sekcji produktu (sekcja 1)
    GribProductData productData;
    loadProductData(inputFile, productData);

    // Wyświetlamy dane produktu
    displayProductData(productData);

    inputFile.close();
    return 0;
}

