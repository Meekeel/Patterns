#include <iostream>
#include <string>
#include <memory>

class Button {
public:
    virtual std::string render() = 0;
    virtual void interact() = 0;
    virtual ~Button() = default;
};

class ScrollBar {
public:
    virtual std::string render() = 0;
    virtual void interact() = 0;
    virtual ~ScrollBar() = default;
};

class CheckBox {
public:
    virtual std::string render() = 0;
    virtual void interact() = 0;
    virtual ~CheckBox() = default;
};

class WinButton : public Button {
public:
    std::string render() override { return "Windows Кнопка"; }
    void interact() override { std::cout << "Клик по Windows кнопке!\n"; }
};

class WinScrollBar : public ScrollBar {
public:
    std::string render() override { return "Windows Скроллбар"; }
    void interact() override { std::cout << "Прокрутка Windows скроллбара\n"; }
};

class WinCheckBox : public CheckBox {
public:
    std::string render() override { return "Windows Чекбокс"; }
    void interact() override { std::cout << "Переключение Windows чекбокса\n"; }
};

class MacButton : public Button {
public:
    std::string render() override { return "macOS Кнопка"; }
    void interact() override { std::cout << "Клик по macOS кнопке!\n"; }
};

class MacScrollBar : public ScrollBar {
public:
    std::string render() override { return "macOS Скроллбар"; }
    void interact() override { std::cout << "Прокрутка macOS скроллбара\n"; }
};

class MacCheckBox : public CheckBox {
public:
    std::string render() override { return "macOS Чекбокс"; }
    void interact() override { std::cout << "Переключение macOS чекбокса\n"; }
};

class LinuxButton : public Button {
public:
    std::string render() override { return "Linux Кнопка"; }
    void interact() override { std::cout << "Клик по Linux кнопке!\n"; }
};

class LinuxScrollBar : public ScrollBar {
public:
    std::string render() override { return "Linux Скроллбар"; }
    void interact() override { std::cout << "Прокрутка Linux скроллбара\n"; }
};

class LinuxCheckBox : public CheckBox {
public:
    std::string render() override { return "Linux Чекбокс"; }
    void interact() override { std::cout << "Переключение Linux чекбокса\n"; }
};

class UIFactory {
public:
    virtual Button* createButton() = 0;
    virtual ScrollBar* createScrollBar() = 0;
    virtual CheckBox* createCheckBox() = 0;
    virtual ~UIFactory() = default;
};

class WinFactory : public UIFactory {
public:
    Button* createButton() override { return new WinButton(); }
    ScrollBar* createScrollBar() override { return new WinScrollBar(); }
    CheckBox* createCheckBox() override { return new WinCheckBox(); }
};

class MacFactory : public UIFactory {
public:
    Button* createButton() override { return new MacButton(); }
    ScrollBar* createScrollBar() override { return new MacScrollBar(); }
    CheckBox* createCheckBox() override { return new MacCheckBox(); }
};

class LinuxFactory : public UIFactory {
public:
    Button* createButton() override { return new LinuxButton(); }
    ScrollBar* createScrollBar() override { return new LinuxScrollBar(); }
    CheckBox* createCheckBox() override { return new LinuxCheckBox(); }
};

UIFactory* createFactory(const std::string& osType) {
    if (osType == "win") {
        return new WinFactory();
    }
    else if (osType == "mac") {
        return new MacFactory();
    }
    else if (osType == "linux") {
        return new LinuxFactory();
    }
    else {
        std::cout << "Неизвестная ОС, используется Windows по умолчанию\n";
        return new WinFactory();
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    std::string osChoice;
    std::cout << "Выберите ОС (win/mac/linux): ";
    std::getline(std::cin, osChoice);

    UIFactory* factory = createFactory(osChoice);

    Button* btn = factory->createButton();
    ScrollBar* sb = factory->createScrollBar();
    CheckBox* cb = factory->createCheckBox();

    std::cout << "\n Рендеринг элементов \n";
    std::cout << btn->render() << "\n";
    std::cout << sb->render() << "\n";
    std::cout << cb->render() << "\n";

    std::cout << "\n Взаимодействие с элементами \n";
    btn->interact();
    sb->interact();
    cb->interact();

    delete btn;
    delete sb;
    delete cb;
    delete factory;
}

/*
преимущества abstract factory:
Abstract Factory гарантирует, что создаваемые продукты будут совместимы друг с другом
Например, нельзя случайно создать Windows кнопку с macOS скроллбаром
При использовании отдельных фабрик такая ситуация возможна
Лёгкость добавления новых платформ
*/