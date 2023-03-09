/*
** EPITECH PROJECT, 2023
** NanoTekSpice
** File description:
** Circuit
*/

#include "../Include/Circuit.hpp"

void Circuit::addComponent(const std::string &name, std::unique_ptr<nts::IComponent> component)
{
    components[name] = std::move(component);
    // std::cout << "Component " << name << " added" << std::endl;
}

nts::IComponent *Circuit::findComponent(const std::string &name) const
{
    if (components.count(name))
        return components.at(name).get();
    return nullptr;
}

void Circuit::simulate()
{
    tick++;
    for (const auto &component : components) {
        if (component.second->getType() == nts::INPUT) {
            component.second->setState();
        }
    }
}

std::ostream &operator << (std::ostream &stream, nts::Tristate state)
{
    switch (state) {
        case nts::Tristate::UNDEFINED :
            stream << "U";
            break;
        case nts::Tristate::TRUE:
            stream << "1";
            break;
        case nts::Tristate::FALSE:
            stream << "0";
            break;
    }
    return stream;
}

void Circuit::display() {
    std::cout << "tick: " << tick << std::endl;

    std::cout << "input(s):" << std::endl;
    for (const auto &component : components) {
        if (component.second->getType() == nts::INPUT)
            std::cout << "  " << component.first << ": " << component.second->compute(1) << std::endl;
    }
    std::cout << "output(s):" << std::endl;
    for (const auto &component : components) {
        if (component.second->getType() == nts::OUTPUT)
            std::cout << "  " << component.first << ": " << component.second->compute(1) << std::endl;
    }
}
