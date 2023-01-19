#include "ensitlm.h"
#include "native_wrapper.h"

using namespace std;

/*
 * The C compiler does a bit of magic on the main() function. Trick it
 * by changing the name to something else.
 */
#define main __start

/* extern "C" is needed since the software is compiled in C and
 * is linked against native_wrapper.cpp, which is compiled in C++.
 */
extern "C" int main();
extern "C" void interrupt_handler();

extern "C" void hal_write32(uint32_t addr, uint32_t data) {
	cout << "Writing " << hex << data << " at " << addr << endl;
	NativeWrapper::get_instance()->hal_write32(addr,data);
}

extern "C" unsigned int hal_read32(uint32_t addr) {
	cout << "Reading from " << hex << addr << endl;
	return NativeWrapper::get_instance()->hal_read32(addr);
}

extern "C" void hal_cpu_relax() {
	NativeWrapper::get_instance()->hal_cpu_relax();
}

extern "C" void hal_wait_for_irq() {
	NativeWrapper::get_instance()->hal_wait_for_irq();
}

/* To keep it simple, the soft wrapper is a singleton, we can
 * call its methods in a simple manner, using
 * NativeWrapper::get_instance()->method_name()
 */
NativeWrapper * NativeWrapper::get_instance() {
	static NativeWrapper * instance = NULL;
	if (!instance)
		instance = new NativeWrapper("native_wrapper");
	return instance;
}

NativeWrapper::NativeWrapper(sc_core::sc_module_name name) : sc_module(name),
							     irq("irq")
{
	SC_THREAD(compute);

	SC_METHOD(interrupt_handler);
	sensitive << irq.pos();
	dont_initialize();

	interrupt = false;
}

void NativeWrapper::hal_write32(unsigned int addr, unsigned int data)
{
	ensitlm::addr_t a = addr;
	ensitlm::data_t d = data;
	
	socket.write(a, d);	
}

unsigned int NativeWrapper::hal_read32(unsigned int addr)
{
	ensitlm::addr_t a = addr;
	ensitlm::data_t rvalue;

	socket.read(a, rvalue);

	return (unsigned int)rvalue;
}

void NativeWrapper::hal_cpu_relax()
{
	abort(); // TODO
}

void NativeWrapper::hal_wait_for_irq()
{
		if (!interrupt)
			wait(interrupt_event);
		interrupt = false;
}

void NativeWrapper::compute()
{
	abort(); // TODO
}

void NativeWrapper::interrupt_handler_internal()
{
	interrupt = true;
	interrupt_event.notify();
}
