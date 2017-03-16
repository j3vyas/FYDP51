#include "ChairMove.h"

#include <Python.h>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

using namespace std;

ChairMove::ChairMove(): 
	pInstance(NULL)
{
    //Requrire Bluetooth Connection
	PyObject *module_name, *module, *pDict, *pClass, *pObject;

	string PY_NAME = "blue_tooth_command";
	string PY_CLASS_NAME = "BlueToothConnect";

	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")");
	PyRun_SimpleString("import bluetooth");
	module_name = PyUnicode_FromString(PY_NAME.c_str());
	module = PyImport_Import(module_name);
	if (module == NULL) {
		cout << "\n=========pModule fail\n";
		PyErr_Print();
		return;
	}
	pDict = PyModule_GetDict(module);
	if (pDict == NULL) {
		cout << "\n=========pDict fail\n";
		return;
	}
	pClass = PyDict_GetItemString(pDict, PY_CLASS_NAME.c_str());
	Py_INCREF(pClass);
	if (PyCallable_Check(pClass)) {
		pInstance = PyObject_CallObject(pClass, NULL);
	}
}
    
void ChairMove::turnRight() {
	send_motor_command("SF-150-B-150X");
	cout << "TURN RIGHT" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	ChairMove::moveStop();
}
    
void ChairMove::turnLeft() {
	send_motor_command("SB-150-F-150X");
	cout << "TURN LEFT" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	ChairMove::moveStop();
}

void ChairMove::moveForward() {
	send_motor_command("SF-100-F-100X");
	cout << "FORWARD" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	ChairMove::moveStop();
}
    
void ChairMove::moveStop() {
	send_motor_command("SF-0-F-0X");
	cout << "STOP" << endl;
}

void ChairMove::send_motor_command(std::string command) {
	string COMMAND_FUNCTION = "sendCommand";
	PyObject *callResult = PyObject_CallMethod(pInstance, COMMAND_FUNCTION.c_str(), "(s)", command.c_str());
	if (callResult == NULL) {
		cout << "pCall Error ";
		PyErr_Print(); 
	}
	else {
		cout << "\n=============command sent: " << command << "\n";
		PyErr_Print();
	}
}

void ChairMove::closeBTSocket() {
	string CLOSE_FUNCTION = "closeConnection";
	PyObject_CallMethod(pInstance, CLOSE_FUNCTION.c_str(), NULL);
}