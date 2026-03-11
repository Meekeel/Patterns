#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class VendingMachine;

class State {
public:
    virtual void insertCoin(VendingMachine* vm) = 0;
    virtual void ejectCoin(VendingMachine* vm) = 0;
    virtual void dispense(VendingMachine* vm) = 0;
    virtual std::string getName() const = 0;
    virtual ~State() = default;
};

class NoCoinState : public State {
public:
    void insertCoin(VendingMachine* vm) override;
    void ejectCoin(VendingMachine* vm) override {
        std::cout << "Нет монеты для возврата!" << std::endl;
    }
    void dispense(VendingMachine* vm) override {
        std::cout << "Вставьте монету сначала!" << std::endl;
    }
    std::string getName() const override { return "NoCoin"; }
};

class HasCoinState : public State {
public:
    void insertCoin(VendingMachine* vm) override {
        std::cout << "Монета уже вставлена!" << std::endl;
    }
    void ejectCoin(VendingMachine* vm) override;
    void dispense(VendingMachine* vm) override;
    std::string getName() const override { return "HasCoin"; }
};

class SoldOutState : public State {
public:
    void insertCoin(VendingMachine* vm) override {
        std::cout << "Напитки кончились! Монета возвращена." << std::endl;
    }
    void ejectCoin(VendingMachine* vm) override {
        std::cout << "Монеты нет, но напитки кончились." << std::endl;
    }
    void dispense(VendingMachine* vm) override {
        std::cout << "Напитки кончились!" << std::endl;
    }
    std::string getName() const override { return "SoldOut"; }
};

class WinnerState : public State {
public:
    void insertCoin(VendingMachine* vm) override {
        std::cout << "Подождите, выдаётся выигрыш!" << std::endl;
    }
    void ejectCoin(VendingMachine* vm) override {
        std::cout << "Нельзя вернуть монету во время выдачи выигрыша!" << std::endl;
    }
    void dispense(VendingMachine* vm) override;
    std::string getName() const override { return "Winner"; }
};

class VendingMachine {
private:
    State* noCoinState;
    State* hasCoinState;
    State* soldOutState;
    State* winnerState;
    State* currentState;
    int count; 

public:
    VendingMachine(int numberOfDrinks = 5) : count(numberOfDrinks) {
        noCoinState = new NoCoinState();
        hasCoinState = new HasCoinState();
        soldOutState = new SoldOutState();
        winnerState = new WinnerState();
        if (count > 0) {
            currentState = noCoinState;
        }
        else {
            currentState = soldOutState;
        }

        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    ~VendingMachine() {
        delete noCoinState;
        delete hasCoinState;
        delete soldOutState;
        delete winnerState;
    }

    void setState(State* s) {
        currentState = s;
    }

    void insertCoin() {
        currentState->insertCoin(this);
    }

    void ejectCoin() {
        currentState->ejectCoin(this);
    }

    void dispense() {
        currentState->dispense(this);
    }

    void releaseDrink() {
        if (count > 0) {
            std::cout << "Напиток выдан!" << std::endl;
            count--;
            std::cout << "Осталось напитков: " << count << std::endl;
        }
    }

    int getCount() const { return count; }

    State* getNoCoinState() { return noCoinState; }
    State* getHasCoinState() { return hasCoinState; }
    State* getSoldOutState() { return soldOutState; }
    State* getWinnerState() { return winnerState; }
};

void NoCoinState::insertCoin(VendingMachine* vm) {
    std::cout << "Монета вставлена." << std::endl;
    vm->setState(vm->getHasCoinState());
}

void HasCoinState::ejectCoin(VendingMachine* vm) {
    std::cout << "Монета возвращена." << std::endl;
    vm->setState(vm->getNoCoinState());
}

void HasCoinState::dispense(VendingMachine* vm) {
    std::cout << "Проверяем возможность выигрыша..." << std::endl;

    if (vm->getCount() >= 2 && (std::rand() % 100) < 20) {
        vm->setState(vm->getWinnerState());
        vm->dispense();
    }
    else {
        vm->releaseDrink();

        if (vm->getCount() > 0) {
            vm->setState(vm->getNoCoinState());
        }
        else {
            std::cout << "Напитки кончились!" << std::endl;
            vm->setState(vm->getSoldOutState());
        }
    }
}

void WinnerState::dispense(VendingMachine* vm) {
    std::cout << "ВЫ ВЫИГРАЛИ! Второй напиток бесплатно!" << std::endl;
    vm->releaseDrink();

    if (vm->getCount() > 0) {
        vm->releaseDrink();

        if (vm->getCount() > 0) {
            vm->setState(vm->getNoCoinState());
        }
        else {
            std::cout << "Напитки кончились!" << std::endl;
            vm->setState(vm->getSoldOutState());
        }
    }
    else {
        std::cout << "Ой! Напитков нет для выигрыша!" << std::endl;
        vm->setState(vm->getSoldOutState());
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    std::cout << "АВТОМАТ С НАПИТКАМИ (ПАТТЕРН STATE)\n\n";

    VendingMachine vm(3);

    std::cout << "Тест 1: Обычная работа\n";
    vm.dispense();
    vm.insertCoin();
    vm.ejectCoin();
    vm.insertCoin();
    vm.dispense();

    std::cout << "\nТест 2: Попытка вернуть монету после выдачи\n";
    vm.insertCoin();
    vm.dispense();
    vm.ejectCoin();

    std::cout << "\nТест 3: Работа с пустым автоматом\n";
    VendingMachine emptyVm(0);
    emptyVm.insertCoin();
    emptyVm.dispense();

    std::cout << "\nТест 4: Демонстрация выигрыша (может потребоваться несколько попыток)\n";
    VendingMachine winnerVm(5);

    for (int i = 0; i < 6; i++) {
        std::cout << "\nПопытка " << i + 1 << ":\n";
        winnerVm.insertCoin();
        winnerVm.dispense();
    }

}

/*
преимущества state

Принцип единственной ответственности:
Каждое состояние инкапсулировано в отдельном классе
Логика для конкретного состояния не смешивается с другими
С if-else все состояния и переходы были бы в одном огромном классе
Новые состояния добавляются без изменения существующего кода
Нужно только создать новый класс состояния
С if-else пришлось бы менять каждый метод при добавлении состояния

*/