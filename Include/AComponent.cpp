/*
** EPITECH PROJECT, 2023
** Acomponent
** File description:
** AComponent
*/

#include "IComponent.hpp"

void nts::AComponent::setLink(std::size_t pin, IComponent &other, std::size_t otherPin)
{
    _links[pin] = {&other, otherPin};
}

nts::Tristate nts::AComponent::getLink(std::size_t pin) const
{
    if (pin > 0 && _links.count(pin))
        return _links.at(pin).first->compute(_links.at(pin).second);
    return UNDEFINED;
}

void nts::AComponent::simulate()
{
}

std::ostream &operator << (std::ostream &stream, nts::Tristate state)
{
    switch (state) {
        case nts::Tristate::UNDEFINED :
            stream << "U";
            break;
        case nts::Tristate::TRUE :
            stream << "1";
            break;
        case nts::Tristate::FALSE :
            stream << "0";
            break;
    }
    return stream;
}

std::string returnState(nts::Tristate state)
{
    switch (state) {
        case nts::Tristate::UNDEFINED :
            return "U";
            break;
        case nts::Tristate::TRUE :
            return "1";
            break;
        case nts::Tristate::FALSE :
            return "0";
            break;
    }
    return "U";
}
