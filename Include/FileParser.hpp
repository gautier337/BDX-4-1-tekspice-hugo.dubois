/*
** EPITECH PROJECT, 2023
** NanoTekSpice
** File description:
** File parser headers
*/

#pragma once

#include "IComponent.hpp"

std::vector<std::pair<std::string, std::string>>parseLinks(const std::string& filename);
std::vector<std::pair<std::string, std::string>>parseChipsets(const std::string &filename);
void verifyChipsetsAndLinks(const std::string& filename);