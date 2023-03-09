/*
** EPITECH PROJECT, 2023
** NanoTekSpice
** File description:
** main function
*/

#include "../Include/IComponent.hpp"
#include "../Include/Circuit.hpp"

std::atomic<bool> g_interrupted(false);

void addComponentCircuit(std::vector<std::pair<std::string, std::string>> chipsets, Circuit &circuit)
{
    for (const auto &chipset : chipsets) {
        std::unique_ptr<nts::IComponent> component;

        if (chipset.first == "input") {
            component = std::make_unique<nts::InputComponent>();
        } else if (chipset.first == "clock") {
            component = std::make_unique<nts::ClockComponent>();
        } else if (chipset.first == "output") {
            component = std::make_unique<nts::OutputComponent>();
        } else if (chipset.first == "and") {
            component = std::make_unique<nts::AndComponent>();
        } else if (chipset.first == "or") {
            component = std::make_unique<nts::OrComponent>();
        } else if (chipset.first == "xor") {
            component = std::make_unique<nts::XorComponent>();
        } else if (chipset.first == "not") {
            component = std::make_unique<nts::NotComponent>();
        } else if (chipset.first == "false") {
            component = std::make_unique<nts::FalseComponent>();
        } else if (chipset.first == "true") {
            component = std::make_unique<nts::TrueComponent>();
        } else if (chipset.first == "4011") {
            component = std::make_unique<nts::Component4011NAND>();
        } else if (chipset.first == "4030") {
            component = std::make_unique<nts::Component4030XOR>();
        } else if (chipset.first == "4069") {
            component = std::make_unique<nts::Component4069NOT>();
        } else if (chipset.first == "4071") {
            component = std::make_unique<nts::Component4071OR>();
        } else if (chipset.first == "4081") {
            component = std::make_unique<nts::Component4081AND>();
        } else if (chipset.first == "4008") {
            component = std::make_unique<nts::Component4008ADDER>();
        } else {
            throw nts::Exception::invalidComponentName(chipset.first);
        }

        circuit.addComponent(chipset.second, std::move(component));
    }
}

void sigint_handler(int signal)
{
    g_interrupted = true;
    std::cout << std::endl;
    std::signal(SIGINT, SIG_DFL);
}

int core(std::vector<std::pair<std::string, std::string>> chipsets, std::vector<std::pair<std::string, std::string>> links)
{
    std::string command = "";
    Circuit circuit;

    addComponentCircuit(chipsets, circuit);

    for (const auto &link : links) {
        std::string in_pin = "";
        std::string input = "";

        std::size_t pos = link.first.find(':');
        if (pos != std::string::npos) {
            in_pin = link.first.substr(pos + 1);
            input = link.first.substr(0, pos);
        } else {
            throw nts::Exception::invalidLink(link.first);
        }
        std::string out_pin = "";
        std::string gate = "";
        pos = link.second.find(':');
        if (pos != std::string::npos) {
            out_pin = link.second.substr(pos + 1);
            gate = link.second.substr(0, pos);
        } else {
            throw nts::Exception::invalidLink(link.first);
        }
        circuit.findComponent(gate)->setLink(std::stoi(out_pin), *circuit.findComponent(input), std::stoi(in_pin));
        circuit.findComponent(input)->setLink(std::stoi(in_pin), *circuit.findComponent(gate), std::stoi(out_pin));
    }

    std::cout << "> ";
    while (std::cin >> command) {
        if (command == "simulate") {
            circuit.simulate();
        } else if (command == "display") {
            circuit.display();
        } else if (command == "exit") {
            break;
        } else if (command == "loop") {
            std::signal(SIGINT, sigint_handler);
            while (!g_interrupted) {
                circuit.simulate();
                circuit.display();
                sleep(1);
            }
            g_interrupted = false;
        } else {
            int found = 0;
            for (const auto &chipset : chipsets) {
                if (circuit.findComponent(chipset.second)->getType() != nts::INPUT) {
                    continue;
                }
                if (command == chipset.second + "=1") {
                    circuit.findComponent(chipset.second)->setTmpState(nts::Tristate::TRUE);
                    found++;
                } else if (command == chipset.second + "=0") {
                    circuit.findComponent(chipset.second)->setTmpState(nts::Tristate::FALSE);
                    found++;
                } else if (command == chipset.second + "=U") {
                    circuit.findComponent(chipset.second)->setTmpState(nts::Tristate::UNDEFINED);
                    found++;
                }
            }
            if (found == 0)
                std::cout << "Unknow command." << std::endl;
        }
        std::cout << "> ";
    }
    return 0;
}

int main(int argc, char **argv)
{
    try {
        if (argc != 2)
            throw nts::Exception::invalidArgumentnumber();
        std::vector<std::pair<std::string, std::string>> chipsets = parseChipsets(argv[1]);
        std::vector<std::pair<std::string, std::string>> links = parseLinks(argv[1]);
        verifyChipsetsAndLinks(argv[1]);
        core(chipsets, links);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
