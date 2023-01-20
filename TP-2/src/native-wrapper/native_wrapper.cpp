#include "ensitlm.h"
#include "native_wrapper.h"

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
    NativeWrapper::get_instance()->hal_write32(addr, data);
}

extern "C" unsigned int hal_read32(uint32_t addr) {
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

// Constructor
NativeWrapper::NativeWrapper(sc_core::sc_module_name name) : sc_module(name),
							     irq("irq"), interrupt(false)
{
    // We want to continuosly execute compute
    SC_THREAD(compute);
    
    // We want to execute the interrupt handler once an interruption is received
    SC_METHOD(interrupt_handler_internal);
    sensitive << irq.pos();
}

void NativeWrapper::hal_write32(unsigned int addr, unsigned int data)
{
    // Write with iniciator socket
    socket.write(addr, data);
}

unsigned int NativeWrapper::hal_read32(unsigned int addr)
{
    uint32_t data;

    // Read with iniciator socket
    socket.read(addr, data);

    return data;
}

void NativeWrapper::hal_cpu_relax()
{
    wait(1, sc_core::SC_MS);
}

void NativeWrapper::hal_wait_for_irq()
{
    // The module waits for the interruption event to be triggered
    if (interrupt == false) {
        wait(interrupt_event);
    }

    interrupt = false;
}

void NativeWrapper::compute()
{
    main();
}

void NativeWrapper::interrupt_handler_internal()
{
    // Once the interruption signal is risen, we trigger the interruption event
    if (irq.posedge()){
        interrupt = true;
        interrupt_event.notify();

        interrupt_handler();
    }
}
