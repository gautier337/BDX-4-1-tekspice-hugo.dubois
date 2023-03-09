/*
** EPITECH PROJECT, 2023
** NanoTekSpice
** File description:
** Circuit
*/

#pragma once

#include "IComponent.hpp"

class Circuit {
    public:
        void addComponent(const std::string &name, std::unique_ptr<nts::IComponent> component);
        nts::IComponent *findComponent(const std::string &name) const;
        void simulate();
        void display();
    private:
        std::map<std::string, std::unique_ptr<nts::IComponent>> components;
        int tick = 0;
};
