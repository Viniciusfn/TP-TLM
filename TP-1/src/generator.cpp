#include "generator.h"

using namespace std;

void Generator::thread(void) {
	ensitlm::data_t data = 1;
	ensitlm::addr_t addr = 1;
	while (true) {
		cout << "Enter a number : ";
		cin >> data;
		cout << this->name() << " sends : " << std::dec << data << endl;
		initiator.write(addr, data);
	}
}

Generator::Generator(sc_core::sc_module_name name) : sc_core::sc_module(name) {
	SC_THREAD(thread);
}
