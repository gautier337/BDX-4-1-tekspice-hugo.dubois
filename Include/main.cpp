#include "IComponent.hpp"
#include "Circuit.hpp"

int main (void)
{
    std::unique_ptr<nts::IComponent> gate = std::make_unique<nts::XorComponent>();
    std::unique_ptr<nts::IComponent> input1 = std::make_unique<nts::FalseComponent>();
    std::unique_ptr<nts::IComponent> input2 = std::make_unique<nts::FalseComponent>();
    std::unique_ptr<nts::IComponent> inverter = std::make_unique<nts::NotComponent>();

    // gate->setLink (1, *input1);
    // gate->setLink (2, *input2);
    // inverter->setLink (1, *gate, 3) ;
    // std::cout << "!(" << input1->compute (1) << " && " << input2->compute(1) << ") -> " << inverter->compute (2) << std::endl;

    Circuit circuit;
    circuit.addComponent("gate", std::move(gate));
    circuit.addComponent("input1", std::move(input1));
    circuit.addComponent("input2", std::move(input2));
    circuit.addComponent("inverter", std::move(inverter));
    circuit.findComponent("gate")->setLink(1, *circuit.findComponent("input1"));
    circuit.findComponent("gate")->setLink(2, *circuit.findComponent("input2"));
    circuit.findComponent("inverter")->setLink(1, *circuit.findComponent("gate"), 3);

    std::cout << "!(" << circuit.findComponent("input1")->compute(1) << " !|| " << circuit.findComponent("input2")->compute(1) << ") -> " << circuit.findComponent("inverter")->compute(2) << std::endl;
    return 0;
}
