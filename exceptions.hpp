#include <exception>

class Exception: public exception{
	
	string name = "FATAL_EXCEPTION";

public:
	
	Exception() {}
	Exception(string msg){
	
		name = msg;
	}
	virtual const char* what() const throw(){
	
		return name.c_str();
	}
};
