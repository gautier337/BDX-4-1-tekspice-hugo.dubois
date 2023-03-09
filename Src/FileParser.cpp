/*
** EPITECH PROJECT, 2023
** NanoTekSpice
** File description:
** Parsing files
*/

#include "../Include/IComponent.hpp"

std::vector<std::pair<std::string, std::string>> parseChipsets(const std::string &filename)
{
    std::vector<std::pair<std::string, std::string>> chipset = {};
    bool start_parsing = false;
    std::string line = "";
    std::vector<std::string> valid_types = {"input", "output", "and", "or", "xor", "not", "false", "true", "4011", "4030", "4069", "4071", "4081", "4008"};
    std::ifstream file(filename);
    if (!file.is_open())
        throw nts::Exception::failedToOpenFile(filename);
    while (std::getline(file, line)) {
        if (line.find(".links:") != std::string::npos)
            break;
        if (line.find(".chipsets:") != std::string::npos) {
            start_parsing = true;
            continue;
        }
        if (start_parsing && !line.empty()) {
            std::istringstream iss(line);
            std::string type, name = "";
            iss >> type >> name;
            if (std::find(valid_types.begin(), valid_types.end(), type) != valid_types.end()) {
                for (const auto& component : chipset)
                    if (component.second == name)
                        throw nts::Exception::twoComponentsWithSameName(name, filename);
                chipset.emplace_back(type, name);
            }
            else
                throw nts::Exception::invalidType(type, filename);
        }
    }
    if (chipset.empty())
        throw nts::Exception::noChipsetFound(filename);
    file.close();
    return chipset;
}

std::vector<std::pair<std::string, std::string>>parseLinks(const std::string& filename)
{
    bool start_parsing = false;
    std::string line = "";
    std::ifstream file(filename);
    std::vector<std::pair<std::string, std::string>> links = {};
    if (!file.is_open())
        throw nts::Exception::failedToOpenFile(filename);
    while (std::getline(file, line)) {
        if (line.find(".links:") != std::string::npos)
            start_parsing = true;
        if (start_parsing && !line.empty()) {
            std::istringstream iss(line);
            std::string link, value = "";
            iss >> link >> value;
            if (!link.empty() && !value.empty())
                links.emplace_back(link, value);
        }
    }
    for (const auto& link : links) {
        int count = 0;
        for (const auto& link2 : links) {
            if (link.first == link2.first && link.second == link2.second)
                count++;
        }
        if (count > 1)
            throw nts::Exception::linkConnectedMoreThanOnce(link.first, link.second, filename);
    }
    file.close();
    return links;
}

void verifyChipsetsAndLinks(const std::string& filename) {
    std::vector<std::pair<std::string, std::string>> chipsets = parseChipsets(filename);
    std::vector<std::pair<std::string, std::string>> links = parseLinks(filename);

    int links_size = links.size();

    for (int i = 0; i < links_size; i++) {
        std::string link = links[i].first;
        std::string value = links[i].second;
        std::string link_name = "";
        std::string value_name = "";

        for (int j = 0; j < link.size(); j++) {
            if (link[j] == ':')
                break;
            link_name += link[j];
        }

        for (int j = 0; j < value.size(); j++) {
            if (value[j] == ':')
                break;
            value_name += value[j];
        }

        bool value_name_exists = false;
        for (int j = 0; j < chipsets.size(); j++) {
            if (chipsets[j].second == value_name) {
                value_name_exists = true;
                break;
            }
        }

        if (!value_name_exists)
            throw nts::Exception::linkNotConnected(value, filename);
        bool link_name_exists = false;
        for (int j = 0; j < chipsets.size(); j++) {
            if (chipsets[j].second == link_name) {
                link_name_exists = true;
                break;
            }
        }

        if (!link_name_exists)
            throw nts::Exception::linkNotConnected(link, filename);
    }
}
