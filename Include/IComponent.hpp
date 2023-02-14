/*
** EPITECH PROJECT, 2023
** IComponent
** File description:
** IComponent
*/

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>

namespace nts {
    enum Tristate {
        UNDEFINED = (-true),
        TRUE = true,
        FALSE = false
    };
    
    enum Types {
        INPUT,
        OUTPUT,
        TRUEComponent,
        FALSEComponent,
        AND,
        OR,
        XOR,
        NOT
    };

    class IComponent {
        public:
            virtual ~IComponent() = default;
            virtual Tristate compute(std::size_t pin_num_this = 1) = 0;
            virtual void setLink(std::size_t pin, IComponent &other, std::size_t otherPin = 1) = 0;
            virtual Tristate getLink(std::size_t pin) const = 0;
            virtual void simulate() = 0;
            virtual void display() = 0;
            virtual Types getType() const = 0;
    };

    class AComponent : public IComponent
    {
        public:
            void setLink(std::size_t pin, IComponent &other, std::size_t otherPin);
            Tristate getLink(std::size_t pin) const;
            void simulate();
            virtual Tristate compute(std::size_t pin = 1) = 0;
            virtual Types getType() const = 0;
            void display() { std::cout << getType() << std::endl;}
        protected:
            std::map<std::size_t, std::pair<IComponent *, std::size_t>> _links;
    };

    class InputComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                return getLink(pin);
            }
            Types getType() const { return INPUT; }
    };
    class OutputComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                return getLink(pin);
            }
            Types getType() const { return OUTPUT; }
    };
    class TrueComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                (void) pin;
                return TRUE;
            }
            Types getType() const { return TRUEComponent; }
    };
    class FalseComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                (void) pin;
                return FALSE;
            }
            Types getType() const { return FALSEComponent; }
    };
    class AndComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    Tristate pin2 = getLink(2);
                    if (pin1 == TRUE && pin2 == TRUE)
                        return TRUE;
                    return FALSE;
                }
                return UNDEFINED;
            }
            Types getType() const { return AND; }
    };
    class OrComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    Tristate pin2 = getLink(2);
                    if (pin1 == TRUE || pin2 == TRUE)
                        return TRUE;
                    return FALSE;
                }
                return UNDEFINED;
            }
            Types getType() const { return OR; }
    };
    class XorComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    Tristate pin2 = getLink(2);
                    if ((pin1 == TRUE || pin2 == TRUE) && pin1 != pin2)
                        return TRUE;
                    return FALSE;
                }
                return UNDEFINED;
            }
            Types getType() const { return XOR; }
    };
    class NotComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 2) {
                    Tristate pin1 = getLink(1);
                    if (pin1 == TRUE)
                        return FALSE;
                    return TRUE;
                }
                return UNDEFINED;
            }
            Types getType() const { return NOT; }
    };
}
