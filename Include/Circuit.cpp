/*
** EPITECH PROJECT, 2023
** NanoTekSpice
** File description:
** Circuit
*/

#include "Circuit.hpp"

void Circuit::addComponent(const std::string &name, std::unique_ptr<nts::IComponent> component)
{
    components[name] = std::move(component);
}

nts::IComponent *Circuit::findComponent(const std::string &name) const
{
    if (components.count(name))
        return components.at(name).get();
    return nullptr;
}

void Circuit::simulate()
{
    for (const auto &component : components) {
        component.second->simulate();
    }
}

void Circuit::display() {
    for (const auto &component : components) {
        component.second->display();
    }
}
