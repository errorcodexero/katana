#ifndef NAVX_CONTROL_H
#define NAVX_CONTROL_H

#include "../util/interface.h"
#include "SerialPort.h"

class AHRS;

class Navx_control{
	private:
	AHRS* ahrs;
	Navx_input in;

	public:
	Navx_control();
	Navx_control(frc::SerialPort::Port);
	~Navx_control();

	Navx_input get();
		
	void init(frc::SerialPort::Port);
	friend std::ostream& operator<<(std::ostream&,Navx_control);
};

#endif
