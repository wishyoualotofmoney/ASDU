#include <iostream>
#include "modbus.h"

using namespace std;

int checker_int(int a, int b)
{
	int in;
	while (1) {
		if ((cin >> in) && (in >= a) && (in <= b))
		{
			cin.clear();
			cin.ignore(1024, '\n');
			return(in);
		}
		else
		{
			cout << " Incorrect input. Try again. Enter number from [" << a << "] till [" << b << "]: ";
			cin.clear();
			cin.ignore(1024, '\n');
		}
	}
}

int show_menu()
{
	cout << "\n";
	cout << " 1. Force Multiple Coils" << endl;
	cout << " 2. Read Coil Status" << endl;
	cout << " 3. Preset Multiple Registers" << endl;
	cout << " 4. Read Holding Register" << endl;
	cout << " 5. Read Input Status" << endl;
	cout << " 6. Read Input Registers" << endl;
	cout << " 0. Exit" << endl;
	cout << " >> ";

	return checker_int(0, 6);
}

int main()
{
	setlocale(LC_ALL, ".1251");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	//////////////////////////////// SET REGISTERS
	const int num_type_bits = 128, num_type_registers = 128;
	uint8_t tab_type_bits[num_type_bits];
	uint16_t tab_type_registers[num_type_registers];

	///////////////////////////////// CONNECTION
	const char* connect_ip = "127.0.0.1";
	int connect_port = 502;
	modbus_t* context_mb = modbus_new_tcp(connect_ip, connect_port);
	if (context_mb == NULL) {
		fprintf(stderr, "Unable to allocate libmodbus context\n");
		exit(1);
	}
	int connect_mb = modbus_connect(context_mb);
	if (connect_mb == -1) {
		fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		modbus_free(context_mb);
		exit(2);
	}
	cout << " {Connection successful: " << connect_ip << ":" << connect_port << " }" << endl;

	///////////////////////////////// SET/GET SLAVE
	int slave_ID = 1;
	if (modbus_set_slave(context_mb, slave_ID) == -1) {
		fprintf(stderr, "Invalid slave ID\n");
		modbus_free(context_mb);
		return -1;
	}
	slave_ID = modbus_get_slave(context_mb);
	cout << " {Slave ID: = " << slave_ID << "}" << endl;

	///////////////////////////////// ON/OFF DEBUGGER
	modbus_set_debug(context_mb, TRUE);
	int num_registers = 1;

	while (1) {
		switch (show_menu()) {
		case 1:
		{
			/// ONLY TYPE 1 - BITS
			std::cout << "\nNumber of registers: ";
			num_registers = checker_int(0, num_type_bits);
			if (!num_registers)
				break;

			for (int i = 0; i < num_registers; i++)
			{
				std::cout << "WRITE_BIT >> [" << i + 1 << "] = ";
				tab_type_bits[i] = (bool)checker_int(0, 1);
			}

			std::cout << "WRITTEN_BITS = [ ";
			for (int i = 0; i < num_registers; i++)
				std::cout << (int)tab_type_bits[i] << " ";
			std::cout << "]" << std::endl;

			int num_successful = modbus_write_bits(context_mb, 0, num_registers, tab_type_bits);
			if (num_successful == -1)
				fprintf(stderr, "%s\n", modbus_strerror(errno));
			else
				cout << "Written " << num_successful << " bytes\n";

			break;
		}
		case 2:
		{
			/// ONLY TYPE 1 - BITS
			std::cout << "\nNumber of registers: ";
			num_registers = checker_int(0, num_type_bits);
			if (!num_registers)
				break;

			int num_successful = modbus_read_bits(context_mb, 0, num_registers, tab_type_bits);
			if (num_successful == -1)
				fprintf(stderr, "%s\n", modbus_strerror(errno));
			else
				cout << "Written " << num_successful << " bytes\n";

			std::cout << "READ_BITS = [ ";
			for (int i = 0; i < num_registers; i++)
				std::cout << /*bit_cast<int>((int)*/(int)tab_type_bits[i] << " ";
			std::cout << "]" << std::endl;
			break;
		}
		case 3:
		{
			/// ONLY TYPE 2 - REGISTERS
			std::cout << "\nNumber of registers: ";
			num_registers = checker_int(0, num_type_registers);
			if (!num_registers)
				break;

			for (int i = 0; i < num_registers; i++)
			{
				std::cout << "WRITE_REGISTERS >> [" << i + 1 << "] = ";
				tab_type_registers[i] = checker_int(0, 65535);
			}

			std::cout << "WRITTEN_REGISTERS = [ ";
			for (int i = 0; i < num_registers; i++)
				std::cout << tab_type_registers[i] << " ";
			std::cout << "]" << std::endl;
			int num_successful = modbus_write_registers(context_mb, 0, num_registers, tab_type_registers);
			if (num_successful == -1)
				fprintf(stderr, "%s\n", modbus_strerror(errno));
			else
				cout << "Written " << num_successful << " registers\n";

			break;
		}
		case 4:
		{
			/// ONLY TYPE 2 - REGISTERS
			std::cout << "\nNumber of registers: ";
			num_registers = checker_int(0, num_type_registers);
			if (!num_registers)
				break;

			int num_successful = modbus_read_registers(context_mb, 0, num_registers, tab_type_registers);
			if (num_successful == -1)
				fprintf(stderr, "%s\n", modbus_strerror(errno));
			else
				cout << "Read " << num_successful << " registerss\n";

			std::cout << "READ_REGISTERS = [ ";
			for (int i = 0; i < num_registers; i++)
				std::cout << tab_type_registers[i] << " ";
			std::cout << "]" << std::endl;
			break;
		}
		case 5:
		{
			/// ONLY TYPE 1 - BITS
			std::cout << "\nNumber of registers: ";
			num_registers = checker_int(0, num_type_bits);
			if (!num_registers)
				break;

			int num_successful = modbus_read_input_bits(context_mb, 0, num_registers, tab_type_bits);
			if (num_successful == -1)
				fprintf(stderr, "%s\n", modbus_strerror(errno));
			else
				cout << "Written " << num_successful << " bytes\n";

			std::cout << "READ_INPUT_BITS = [ ";
			for (int i = 0; i < num_registers; i++)
				std::cout << /*bit_cast<int>((int)*/(int)tab_type_bits[i] << " ";
			std::cout << "]" << std::endl;
			break;
		}
		case 6:
		{
			/// ONLY TYPE 2 - REGISTERS
			std::cout << "\nNumber of registers: ";
			num_registers = checker_int(0, num_type_registers);
			if (!num_registers)
				break;

			int num_successful = modbus_read_input_registers(context_mb, 0, num_registers, tab_type_registers);
			if (num_successful == -1)
				fprintf(stderr, "%s\n", modbus_strerror(errno));
			else
				cout << "Read " << num_successful << " registerss\n";

			std::cout << "READ_INPUTS_REGISTERS = [ ";
			for (int i = 0; i < num_registers; i++)
				std::cout << tab_type_registers[i] << " ";
			std::cout << "]" << std::endl;
			break;
		}
		case 0:
		{
			return 0;
			break;
		}
		}
	}

	modbus_close(context_mb);
	modbus_free(context_mb);
}
