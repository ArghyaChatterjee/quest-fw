#include <Autocoders/Python/test/app1/Top.hpp>

#include <Fw/Obj/SimpleObjRegistry.hpp>
#include <iostream>
#include <string.h>

#include <Autocoders/Python/test/app1/DuckImpl.hpp>

#include <unistd.h>

using namespace std;

// Registry
static Fw::SimpleObjRegistry* simpleReg_ptr = 0;

// Component instance pointers
extern Duck::DuckImpl* Huey_ptr;
extern Duck::DuckImpl* Duey_ptr;

extern "C" {
	void dumparch(void);
	void dumpobj(const char* objName);
}

#ifdef TGT_OS_TYPE_LINUX
extern "C" {
	int main(int argc, char* argv[]);
};
#endif

int main(int argc, char* argv[])  {
	// Construct the topology here.
	constructArchitecture();
	// Ask for input to huey or duey here.
	char in[80];
	int cmd;
	Fw::EightyCharString *str;
	char str2[80];
	//
	while ( strcmp(in,"quit") != 0) {
        cout << "Enter any button to run or q to quit: ";
        cin >> in;
        if (strcmp(in,"q")==0){
            cout << "Quitting Program";
            break;
        }


		//
		cout << "Enter cmd number: ";
		cin >> cmd;
		//
		cout << "Enter short string: ";
		//cin.getline(str2,80);
		cin >> str2;
		cout << "The string 2 is: " << str2 << endl;
		str = new Fw::EightyCharString(str2);
		//cout << "The string is: " << str->toChar() << endl;
		//
		cout << "Enter component name for ExtCmdIn execution (huey or duey): ";
		cin >> in;
		//
		if (in[0]=='h') {
			cout << "hueyComp_ptr->get_ExtCmdIn_InputPort()->msg1_in(" << cmd << "," << str << ");" << endl;
			Huey_ptr->get_CmdIn_InputPort(0)->invoke(cmd,*str);
			cout << "huey ExtCmdIn call completed..." << endl;
		} else if (in[0] == 'd') {
			cout << "dueyComp_ptr->get_ExtCmdIn_InputPort()->msg1_in(" << cmd << "," << str << ");" << endl;
			Duey_ptr->get_ExtCmdIn_InputPort(0)->invoke(cmd,*str);
			cout << "duey ExtCmdIn call completed..." << endl;
		} else if (strcmp(in,"quit")==0) {
			cout << "quit demo!" << endl;
		} else {
			cout << "Unrecognized component name." << endl;
		}
		delete str;
		// Delay to allow components to print messages
		sleep(2);
	}

	cout << "Deleting components..." << endl;
	delete Huey_ptr;
	delete Duey_ptr;
	cout << "Delete registration objects..." << endl;
	delete simpleReg_ptr;
	cout << "Completed..." << endl;
}

