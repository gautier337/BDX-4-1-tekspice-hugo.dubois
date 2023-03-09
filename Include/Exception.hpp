/*
** EPITECH PROJECT, 2023
** NanoTekSpice
** File description:
** Exception
*/

#pragma once

#include "IComponent.hpp"

namespace nts {
    class Exception : public std::exception {
        public:
            explicit Exception(const std::string& message) : _message(message) {}

            virtual const char* what() const noexcept override { return _message.c_str(); }

            static Exception failedToOpenFile(const std::string& filename) 
            {
                return Exception("Failed to open file " + filename);
            }

            static Exception twoComponentsWithSameName(const std::string& name, const std::string& filename)
            {
                return Exception("Two components with the same name '" + name + "' found in file " + filename);
            }

            static Exception invalidType(const std::string& type, const std::string& filename)
            {
                return Exception("Invalid type '" + type + "' found in file " + filename);
            }

            static Exception noChipsetFound(const std::string& filename)
            {
                return Exception("No chipset found in file " + filename);
            }

            static Exception linkConnectedMoreThanOnce(const std::string& link, const std::string& value, const std::string& filename)
            {
                return Exception("Link '" + link + "' is connected to '" + value + "' more than once in file " + filename);
            }

            static Exception linkNotConnected(const std::string& value, const std::string& filename)
            {
                return Exception("Link '" + value + "' is not connected to any component in file " + filename);
            }

            static Exception linkNameNotConnected(const std::string& link, const std::string& filename)
            {
                return Exception("Link '" + link + "' is not connected to any component in file " + filename);
            }

            static Exception invalidComponentName(const std::string& message)
            {
                return Exception("Invalid component name: " + message);
            }

            static Exception invalidLink(const std::string& message)
            {
                return Exception("Invalid link: " + message);
            }

            static Exception invalidArgumentnumber(void)
            {
                return Exception("Usage: ./nanotekspice file.nts");
            }

        private:
            std::string _message;
    };
}
