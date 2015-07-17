#ifndef PIPE_HPP 
#define PIPE_HPP 

#include <string>
#include <cstdio>

class Pipe{
	public:
	bool Is_error();
	bool Open(std::string str);
	void Out(std::string str);
	void Flush();
	
	Pipe():err(false){}
	~Pipe(){
		pclose(fp);
	}
	private:
	FILE *fp;
	bool err;
};

#endif

