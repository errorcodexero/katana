#include "autonomous.h"

#include "teleop.h" 
#include "chain.h"
#include "step.h"
#include "align.h"
using namespace std; 

double deg_to_rad(double deg){
	return deg/180*PI;
}

const Inch BUMPER_LENGTH = 3.0;//inches thickness of one bumper

const Inch ROBOT_LENGTH = 28.0 + 1 * BUMPER_LENGTH; //inches from front to back //TODO: change to match number of sides with bumpers

Executive make_test_step(auto a){
	return Executive{Chain{
		Step{a},
		Executive{Teleop{}}
	}};
}

Executive get_auto_mode(Next_mode_info info){
	if(!info.autonomous) return Executive{Teleop()};

	////////////////////////////
	//
	// Parts of other autonomous modes
	// 

	//////////////////////////
	//
	// Full autonomous modes
	//
	
	//Auto mode which does nothing
	const Executive auto_null{Teleop{}};

	Executive motion_profile_test{Chain{
		Step{MP_drive{100.0}}, //drive 100 inches
		Executive{Teleop{}}	
	}};

	Executive drive_test{Chain{
		Step{Drive(1)},
		Executive{Teleop()}
	}};

	Executive drive_straight_test{Chain{
		Step{Navx_drive_straight{100.0}},
		Executive{Teleop()}
	}};
	
	Executive rotate_test{Chain{
		Step{Navx_rotate{90.0}},
		Executive{Teleop()}
	}};

	Executive test_bucket_drive{Chain{
		vector<Step>{
			Step{Drive_to_bucket()},
			Step{Grab_bucket()}
		},
		Executive{Teleop()}
	}};

	Executive cross_midline{Chain{
		Step{Navx_drive_straight{396.0}},
		Executive{Teleop()}
	}};

	Executive cross_field{Chain{
		Step{Navx_drive_straight{540.0}},
		Executive{Teleop()}
	}};

	Executive bucket{Chain{
		vector<Step>{
			Step{Navx_drive_straight{288.0}},
			Step{Drive_to_bucket()},
			Step{Grab_bucket()}
		},
		Executive{Teleop()}
	}};

	Executive cross_midline_r{Chain{
		Step{Navx_drive_straight{-396.0}},
		Executive{Teleop()}
	}};

	Executive cross_field_r{Chain{
		Step{Navx_drive_straight{-540.0}},
		Executive{Teleop()}
	}};

	if(!info.panel.in_use){
		return auto_null;//Do nothing during autonomous mode if no panel exists
	}

	switch(info.panel.auto_select){
		case 0: 
			return auto_null;
		case 1: 
			return cross_midline;
		case 2: 
			return cross_field;
		case 3:
			return bucket;
		case 4:
			return cross_midline_r;
		case 5: 
			return cross_field_r;
		case 6:
		case 7: 
		case 8:
			return auto_null; 
		case 9: 
			return test_bucket_drive;
		default:
			return auto_null;
	}
}

Executive Autonomous::next_mode(Next_mode_info info){
	static const Time DELAY = 0.0;//seconds, TODO: base it off of the dial on the OI? 
	if(!info.autonomous) return Executive{Teleop()};
	if(info.since_switch > DELAY) return get_auto_mode(info);
	return Executive{Autonomous()};
}

Toplevel::Goal Autonomous::run(Run_info){	
	return {};//nothing, just waits
}

bool Autonomous::operator<(Autonomous const&)const{ return 0; }

bool Autonomous::operator==(Autonomous const&)const{ return 1; }

void Autonomous::display(ostream& o)const{
	o<<"Autonomous()";
}


#ifdef AUTONOMOUS_TEST
#include "test.h"
int main(){
	Autonomous a;
	test_executive(a);
}
#endif 
