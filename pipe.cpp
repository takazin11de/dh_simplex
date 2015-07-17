#include "pipe.hpp"
#include "user_std.hpp"

bool Pipe::Open(std::string str){
		fp = popen(str.c_str(), "w"); 
		if (fp == NULL){
			err=true;
			return !err;
		}
		return !err;
	}

bool Pipe::Is_error(){
	return this->err;
}

void Pipe::Out(std::string str){
	if(this->err == false){
		fputs((str+"\n").c_str(), this->fp);
		std::cout << str << std::endl;
		this->Flush();
	}
}

void Pipe::Flush(){
	if(this->err == false)
		fflush(fp);
}
