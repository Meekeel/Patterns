#include <iostream>
#include <string>

class USBPrinter {
public:
    virtual std::string printUSB(const std::string& data, int quality) = 0;
    virtual ~USBPrinter() = default;
};

class USBScanner {
public:
    virtual std::string scanUSB(int quality) = 0;
    virtual ~USBScanner() = default;
};

class OldLPTPrinter {
public:
    std::string printLPT(const std::string& data) {
        return "Старый принтер печатает по LPT: " + data;
    }
};

class OldLPTScanner {
public:
    std::string scanLPT() {
        return "Старый сканер сканирует по LPT";
    }
};

class LPTToUSBAdapter : public USBPrinter {
private:
    OldLPTPrinter* oldPrinter;
public:
    LPTToUSBAdapter(OldLPTPrinter* p) : oldPrinter(p) {}

    std::string printUSB(const std::string& data, int quality) override {
        std::string qualityStr;
        if (quality > 80) qualityStr = "высокое качество";
        else if (quality > 50) qualityStr = "среднее качество";
        else qualityStr = "черновик";

        return oldPrinter->printLPT(data + " [" + qualityStr + ", адаптировано для USB]");
    }
};

class LPTScannerToUSBAdapter : public USBScanner, private OldLPTScanner {
public:
    std::string scanUSB(int quality) override {
        std::string result = scanLPT();
        std::string qualityStr;

        if (quality > 80) qualityStr = "высокое разрешение";
        else if (quality > 50) qualityStr = "среднее разрешение";
        else qualityStr = "низкое разрешение";

        return result + " (качество: " + qualityStr + ", адаптировано для USB)";
    }
};

class ModernUSBPrinter : public USBPrinter {
public:
    std::string printUSB(const std::string& data, int quality) override {
        return "Современный USB-принтер печатает: " + data + " (качество: " + std::to_string(quality) + ")";
    }
};

int main() {
    std::cout << "Демонстрация паттерна Adapter\n";

    std::cout << "Старый принтер через объектный адаптер\n";
    OldLPTPrinter oldPrinter;
    USBPrinter* printerAdapter = new LPTToUSBAdapter(&oldPrinter);

    std::cout << printerAdapter->printUSB("Документ 1", 90) << std::endl;
    std::cout << printerAdapter->printUSB("Документ 2", 60) << std::endl;
    std::cout << printerAdapter->printUSB("Черновик", 30) << std::endl;

    std::cout << "\nСтарый сканер через класс-адаптер\n";
    USBScanner* scannerAdapter = new LPTScannerToUSBAdapter();

    std::cout << scannerAdapter->scanUSB(95) << std::endl;
    std::cout << scannerAdapter->scanUSB(70) << std::endl;
    std::cout << scannerAdapter->scanUSB(40) << std::endl;

    std::cout << "\nСовременное USB-устройство (для сравнения)\n";
    ModernUSBPrinter modernPrinter;
    std::cout << modernPrinter.printUSB("Документ", 85) << std::endl;

    delete printerAdapter;
    delete scannerAdapter;
}

/*
почему адаптер лучше:

Старый класс остаётся без изменений
Мы не рискуем сломать существующий код, который может использовать старый класс
Не нужно перекомпилировать legacy-код, зависящий от старого интерфейса

Работа с недоступным исходным кодом:
Часто старый код принадлежит сторонней библиотеке или утерян
Невозможно изменить класс, если у нас нет исходников
Пример: драйверы устройств, библиотеки вендоров
*/