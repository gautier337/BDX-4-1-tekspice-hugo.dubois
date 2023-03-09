/*
** EPITECH PROJECT, 2023
** Acomponent
** File description:
** AComponent
*/

#include "../Include/IComponent.hpp"

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
