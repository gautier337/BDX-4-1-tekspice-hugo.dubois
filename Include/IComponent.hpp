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
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <csignal>
#include <atomic>
#include <unistd.h>
#include "FileParser.hpp"
#include "Exception.hpp"

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
        NOT,
        CLOCK,
        NOR
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
            virtual void setState() = 0;
            virtual void setTmpState(Tristate state) = 0;
    };

    class AComponent : public IComponent
    {
        public:
            void setLink(std::size_t pin, IComponent &other, std::size_t otherPin);
            Tristate getLink(std::size_t pin) const;
            void simulate();
            virtual Tristate compute(std::size_t pin = 1) = 0;
            virtual Types getType() const = 0;
            void display() { std::cout << getType() << std::endl; }
            virtual void setState() = 0;
            virtual void setTmpState(Tristate state) = 0;
        protected:
            std::map<std::size_t, std::pair<IComponent *, std::size_t>> _links;
    };

    class InputComponent : public AComponent {
        public:
            InputComponent() : _state(UNDEFINED), _tmpState(UNDEFINED) {}
            Tristate compute(std::size_t pin = 0) {
                (void) pin;
                return _state;
            }
            Types getType() const { return INPUT; }
            void setState() { _state = _tmpState; }
            void setTmpState(Tristate state) { _tmpState = state; }
        protected:
            Tristate _state;
            Tristate _tmpState = UNDEFINED;
    };

    class ClockComponent : public AComponent {
        public:
            ClockComponent() : _state(FALSE) {}
            Tristate compute(std::size_t pin = 0) {
                (void) pin;
                return _state;
            }
            Types getType() const { return CLOCK; }
            void setState() {
                if (_state == TRUE)
                    _state = FALSE;
                else
                    _state = TRUE;
            }
            virtual void setTmpState(Tristate state) { (void) state; }
        protected:
            Tristate _state = FALSE;
    };

    class OutputComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                (void) pin;
                return getLink(1);
            }
            Types getType() const { return OUTPUT; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class TrueComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                (void) pin;
                return TRUE;
            }
            Types getType() const { return TRUEComponent; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class FalseComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                (void) pin;
                return FALSE;
            }
            Types getType() const { return FALSEComponent; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class AndComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    Tristate pin2 = getLink(2);
                    if (pin1 == TRUE && pin2 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE && pin2 == UNDEFINED)
                        return FALSE;
                    if (pin1 == UNDEFINED && pin2 == FALSE)
                        return FALSE;
                    if (pin1 == UNDEFINED || pin2 == UNDEFINED)
                        return UNDEFINED;
                    return FALSE;
                }
                return UNDEFINED;
            }
            Types getType() const { return AND; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class OrComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    Tristate pin2 = getLink(2);

                    if (pin1 == UNDEFINED && pin2 == UNDEFINED)
                        return UNDEFINED;
                    if (pin1 == TRUE && pin2 == UNDEFINED)
                        return TRUE;
                    if (pin1 == UNDEFINED && pin2 == TRUE)
                        return TRUE;
                    if (pin1 == UNDEFINED && pin2 == FALSE)
                        return UNDEFINED;
                    if (pin1 == FALSE && pin2 == UNDEFINED)
                        return UNDEFINED;
                    if (pin1 == TRUE || pin2 == TRUE)
                        return TRUE;
                    return FALSE;
                }
                return UNDEFINED;
            }
            Types getType() const { return OR; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class XorComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    Tristate pin2 = getLink(2);
                    if (pin1 == UNDEFINED || pin2 == UNDEFINED)
                        return UNDEFINED;
                    if (pin1 == UNDEFINED && pin2 == UNDEFINED)
                        return UNDEFINED;
                    if ((pin1 == TRUE || pin2 == TRUE) && pin1 != pin2)
                        return TRUE;
                    return FALSE;
                }
                return UNDEFINED;
            }
            Types getType() const { return XOR; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class NotComponent : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 2) {
                    Tristate pin1 = getLink(1);
                    if (pin1 == TRUE)
                        return FALSE;
                    if (pin1 == UNDEFINED)
                        return UNDEFINED;
                    return TRUE;
                }
                return UNDEFINED;
            }   
            Types getType() const { return NOT; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class Component4011NAND : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    Tristate pin2 = getLink(2);
                    if (pin1 == TRUE && pin2 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE && pin2 == UNDEFINED)
                        return TRUE;
                    if (pin1 == UNDEFINED && pin2 == FALSE)
                        return TRUE;
                    if (pin1 == UNDEFINED || pin2 == UNDEFINED)
                        return UNDEFINED;
                    return TRUE;
                }
                if (pin == 4) {
                    Tristate pin1 = getLink(5);
                    Tristate pin2 = getLink(6);
                    if (pin1 == TRUE && pin2 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE && pin2 == UNDEFINED)
                        return TRUE;
                    if (pin1 == UNDEFINED && pin2 == FALSE)
                        return TRUE;
                    if (pin1 == UNDEFINED || pin2 == UNDEFINED)
                        return UNDEFINED;
                    return TRUE;
                }
                if (pin == 10) {
                    Tristate pin1 = getLink(8);
                    Tristate pin2 = getLink(9);
                    if (pin1 == TRUE && pin2 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE && pin2 == UNDEFINED)
                        return TRUE;
                    if (pin1 == UNDEFINED && pin2 == FALSE)
                        return TRUE;
                    if (pin1 == UNDEFINED || pin2 == UNDEFINED)
                        return UNDEFINED;
                    return TRUE;
                }
                if (pin == 11) {
                    Tristate pin1 = getLink(12);
                    Tristate pin2 = getLink(13);
                    if (pin1 == TRUE && pin2 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE && pin2 == UNDEFINED)
                        return TRUE;
                    if (pin1 == UNDEFINED && pin2 == FALSE)
                        return TRUE;
                    if (pin1 == UNDEFINED || pin2 == UNDEFINED)
                        return UNDEFINED;
                    return TRUE;
                }
                return UNDEFINED;
            }
            Types getType() const { return AND; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class Component4030XOR : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    Tristate pin2 = getLink(2);
                    if (pin1 == UNDEFINED || pin2 == UNDEFINED)
                        return UNDEFINED;
                    if (pin1 == UNDEFINED && pin2 == UNDEFINED)
                        return UNDEFINED;
                    if ((pin1 == TRUE || pin2 == TRUE) && pin1 != pin2)
                        return TRUE;
                    return FALSE;
                }
                if (pin == 4) {
                    Tristate pin1 = getLink(5);
                    Tristate pin2 = getLink(6);
                    if (pin1 == UNDEFINED || pin2 == UNDEFINED)
                        return UNDEFINED;
                    if (pin1 == UNDEFINED && pin2 == UNDEFINED)
                        return UNDEFINED;
                    if ((pin1 == TRUE || pin2 == TRUE) && pin1 != pin2)
                        return TRUE;
                    return FALSE;
                }
                if (pin == 10) {
                    Tristate pin1 = getLink(8);
                    Tristate pin2 = getLink(9);
                    if (pin1 == UNDEFINED || pin2 == UNDEFINED)
                        return UNDEFINED;
                    if (pin1 == UNDEFINED && pin2 == UNDEFINED)
                        return UNDEFINED;
                    if ((pin1 == TRUE || pin2 == TRUE) && pin1 != pin2)
                        return TRUE;
                    return FALSE;
                }
                if (pin == 11) {
                    Tristate pin1 = getLink(12);
                    Tristate pin2 = getLink(13);
                    if (pin1 == UNDEFINED || pin2 == UNDEFINED)
                        return UNDEFINED;
                    if (pin1 == UNDEFINED && pin2 == UNDEFINED)
                        return UNDEFINED;
                    if ((pin1 == TRUE || pin2 == TRUE) && pin1 != pin2)
                        return TRUE;
                    return FALSE;
                }
                return UNDEFINED;
            }
            Types getType() const { return AND; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class Component4069NOT : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 2) {
                    Tristate pin1 = getLink(1);
                    if (pin1 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE)
                        return TRUE;
                    return UNDEFINED;
                }
                if (pin == 4) {
                    Tristate pin1 = getLink(3);
                    if (pin1 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE)
                        return TRUE;
                    return UNDEFINED;
                }
                if (pin == 6) {
                    Tristate pin1 = getLink(5);
                    if (pin1 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE)
                        return TRUE;
                    return UNDEFINED;
                }
                if (pin == 8) {
                    Tristate pin1 = getLink(9);
                    if (pin1 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE)
                        return TRUE;
                    return UNDEFINED;
                }
                if (pin == 10) {
                    Tristate pin1 = getLink(11);
                    if (pin1 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE)
                        return TRUE;
                    return UNDEFINED;
                }
                if (pin == 12) {
                    Tristate pin1 = getLink(13);
                    if (pin1 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE)
                        return TRUE;
                    return UNDEFINED;
                }
                return UNDEFINED;
            }
            Types getType() const { return AND; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class Component4071OR : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    Tristate pin2 = getLink(2);
                    if (pin1 == TRUE || pin2 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE && pin2 == FALSE)
                        return FALSE;
                    return UNDEFINED;
                }
                if (pin == 4) {
                    Tristate pin1 = getLink(5);
                    Tristate pin2 = getLink(6);
                    if (pin1 == TRUE || pin2 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE && pin2 == FALSE)
                        return FALSE;
                    return UNDEFINED;
                }
                if (pin == 10) {
                    Tristate pin1 = getLink(8);
                    Tristate pin2 = getLink(9);
                    if (pin1 == TRUE || pin2 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE && pin2 == FALSE)
                        return FALSE;
                    return UNDEFINED;
                }
                if (pin == 11) {
                    Tristate pin1 = getLink(12);
                    Tristate pin2 = getLink(13);
                    if (pin1 == TRUE || pin2 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE && pin2 == FALSE)
                        return FALSE;
                    return UNDEFINED;
                }
                return UNDEFINED;
            }
            Types getType() const { return AND; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class Component4081AND : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    Tristate pin2 = getLink(2);
                    if (pin1 == TRUE && pin2 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE || pin2 == FALSE)
                        return FALSE;
                    return UNDEFINED;
                }
                if (pin == 4) {
                    Tristate pin1 = getLink(5);
                    Tristate pin2 = getLink(6);
                    if (pin1 == TRUE && pin2 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE || pin2 == FALSE)
                        return FALSE;
                    return UNDEFINED;
                }
                if (pin == 10) {
                    Tristate pin1 = getLink(8);
                    Tristate pin2 = getLink(9);
                    if (pin1 == TRUE && pin2 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE || pin2 == FALSE)
                        return FALSE;
                    return UNDEFINED;
                }
                if (pin == 11) {
                    Tristate pin1 = getLink(12);
                    Tristate pin2 = getLink(13);
                    if (pin1 == TRUE && pin2 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE || pin2 == FALSE)
                        return FALSE;
                    return UNDEFINED;
                }
                return UNDEFINED;
            }
            Types getType() const { return AND; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };

    class Component4008ADDER : public AComponent {
        public:
            Tristate computePin(Tristate pin1, Tristate pin2) {
                if (pin1 == FALSE && pin2 == FALSE && inc == FALSE) {
                    inc = FALSE;
                    return FALSE;
                }
                if (pin1 == TRUE && pin2 == FALSE && inc == FALSE) {
                    inc = TRUE;
                    return TRUE;
                }
                if (pin1 == FALSE && pin2 == TRUE && inc == FALSE) {
                    inc = TRUE;
                    return TRUE;
                }
                if (pin1 == TRUE && pin2 == TRUE && inc == FALSE) {
                    inc = TRUE;
                    return FALSE;
                }
                if (pin1 == FALSE && pin2 == FALSE && inc == TRUE) {
                    inc = FALSE;
                    return TRUE;
                }
                if (pin1 == TRUE && pin2 == FALSE && inc == TRUE) {
                    inc = TRUE;
                    return FALSE;
                }
                if (pin1 == FALSE && pin2 == TRUE && inc == TRUE) {
                    inc = TRUE;
                    return FALSE;
                }
                if (pin1 == TRUE && pin2 == TRUE && inc == TRUE) {
                    inc = TRUE;
                    return TRUE;
                }
                return UNDEFINED;
            }
            Tristate compute(std::size_t pin = 0) {
                if (pin == 10) {
                    Tristate pin1 = getLink(6);
                    Tristate pin2 = getLink(7);
                    return computePin(pin1, pin2);
                }
                if (pin == 11) {
                    Tristate pin1 = getLink(4);
                    Tristate pin2 = getLink(5);
                    return computePin(pin1, pin2);
                }
                if (pin == 12) {
                    Tristate pin1 = getLink(2);
                    Tristate pin2 = getLink(3);
                    return computePin(pin1, pin2);
                }
                if (pin == 13) {
                    Tristate pin1 = getLink(15);
                    Tristate pin2 = getLink(1);
                    return computePin(pin1, pin2);
                }
                return UNDEFINED;
            }
            Types getType() const { return AND; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
        protected:
            Tristate inc = FALSE;
    };

    class Circuit4013FlipFlop : public AComponent {
        public:
            Tristate compute(std::size_t pin = 0) {
                if (pin == 3) {
                    Tristate pin1 = getLink(1);
                    if (pin1 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE)
                        return FALSE;
                    return UNDEFINED;
                }
                if (pin == 4) {
                    Tristate pin1 = getLink(1);
                    if (pin1 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE)
                        return TRUE;
                    return UNDEFINED;
                }
                if (pin == 10) {
                    Tristate pin1 = getLink(11);
                    if (pin1 == TRUE)
                        return TRUE;
                    if (pin1 == FALSE)
                        return FALSE;
                    return UNDEFINED;
                }
                if (pin == 11) {
                    Tristate pin1 = getLink(12);
                    if (pin1 == TRUE)
                        return FALSE;
                    if (pin1 == FALSE)
                        return TRUE;
                    return UNDEFINED;
                }
                return UNDEFINED;
            }
            Types getType() const { return AND; }
            void setState() {}
            virtual void setTmpState(Tristate state) { (void) state; }
    };
}
