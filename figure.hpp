#include "user_std.hpp"
#include "common_data.hpp"
#include "pipe.hpp"

#ifndef PLOTDATA_HPP
#define PLOTDATA_HPP


class figure{
	
	public:
	~figure(){
	}
	bool Init();
	bool Init(std::string file_name);
	void Out(std::string str);
	void Plot(std::string filename, std::vector<xydata> &data, std::string title);
	void Replot(std::string filename, std::vector<xydata> &data, std::string title);
	void Plot(std::string filename, std::vector<xydata> &data);
	void Replot(std::string filename, std::vector<xydata> &data);
	void LineType(int x);
	void LineColor(std::string color);
	void LineWidth(int x);
	void LinePoint(int x);
	void Title(std::string title);
	void KeyPos(double x,double y);
	void KeyPos(std::string pos);
	void GraphStyle(std::string graph_style);
	void EpsMode(std::string filename);
	void EpsMode(std::string filename, double x_size, double y_size);
	void WxtMode();
	void WxtMode(int x_size, int y_size);
	void StartLog();
	void StopLog();
	void ClearLog();
	
	private:
	
	void BasePlot(std::string filename, std::string command,
			std::vector<xydata> &data, std::string title);
	
	stringstream ss;
	
	Pipe gp;
	int line_type;
	std::string line_color;
	double line_width;
	int line_point;
	std::string graph_style;
	std::string title;
	
	double key_x_pos;
	double key_y_pos;
	
	int wxt_x_size;
	int wxt_y_size;
	
	bool f_log;
	std::ofstream log;
	string gp_file_name;
	
};

#endif
