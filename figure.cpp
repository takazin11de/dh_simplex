#include "figure.hpp"

bool figure::Init(){
	this->Init("fig.gnuplot");
	
}
bool figure::Init(std::string file_name){
	this->gp.Open("gnuplot");
	if(this->gp.Is_error() == true)
		return true;
	
	const timespec time={0,1000*1000*1};
	nanosleep(&time,0);
	
	this->f_log=true;
	
	this->log.open(file_name);
	gp_file_name = file_name;
	this->wxt_x_size=640;
	this->wxt_y_size=480;
	this->line_type=1;
	this->line_color="#FF0000";
	this->line_width=2;
	this->line_point=1;
	this->graph_style="lines";
	this->title="data";
	this->key_x_pos=100;
	this->key_y_pos=100;
	ss.str("");
	ss << "set terminal wxt size "<< wxt_x_size <<","<< wxt_y_size ;
	this->Out(ss.str());
	this->Out("set grid");
	this->Out("set mouse");
	this->Out("set tics font \"Times New Roman,16\"");
	this->Out("set xlabel \"time[s]\" font \"Times New Roman,20\"");
	this->Out("set ylabel \"value y[m]\" font \"Times New Roman,20\"");
	this->Out("set zlabel \"value z[m]\" font \"Times New Roman,20\"");
	this->Out("set key font \"Times New Roman,16\"");
	this->Out("set key opaque box spacing 3.2 width 0.2 reverse Left");
	//ss.str("");
	//ss << "set key " << this->key_x_pos <<","<< this->key_y_pos ;
	//this->gp.Out(ss.str());
	
	return false;
}


void figure::Out(std::string str){
	
	this->gp.Out(str);
	this->gp.Flush();
	if(f_log){
		this->log << str << std::endl;
	}
}


void figure::BasePlot(std::string filename, std::string command,
						std::vector<xydata> &data, std::string title){
	std::ofstream ofs(filename);
	for(auto it=data.begin();it!=data.end();it++){
		ofs << (*it).x << " " << (*it).y << endl;
		//if(0.941 < (*it).x && (*it).x < 0.946){
		//	std::cout << (*it).x << " " << (*it).y << endl;
		//}
	}
	ofs.close();
	const timespec time={0,1000*1000*10};
	nanosleep(&time,0);
	this->ss.str("");
	this->ss << command <<" '"<<filename<<"' with "<<this->graph_style<<" lt " 
		<< this->line_type << " lw " << this->line_width << 
		" lc rgb '" << this->line_color ;
		
	if(this->graph_style=="points" ||this->graph_style=="linepoints" ){
		this->ss << " pt " << this->line_point << " " ;
	}
	this->ss << "' title '"<< title <<"' ";

	this->Out(this->ss.str());
	this->ss.str("");

	
}


void figure::Plot(std::string filename, std::vector<xydata> &data, std::string title){
	this->BasePlot(filename, "plot",data,title);
}


void figure::Replot(std::string filename, std::vector<xydata> &data, std::string title){
	this->BasePlot(filename, "replot",data,title);
}


void figure::Plot(std::string filename, std::vector<xydata> &data){
	this->BasePlot(filename, "plot",data,"data");
}


void figure::Replot(std::string filename, std::vector<xydata> &data){
	this->BasePlot(filename, "replot",data,"data");
}


void figure::LineType(int x){
	this->line_type=x;
}


void figure::LineColor(std::string color){
	this->line_color=color;
}


void figure::LineWidth(int x){
	this->line_width=x;
}


void figure::LinePoint(int x){
	this->line_point=x;
}

void figure::Title(std::string title){
	this->title=title;
}

void figure::KeyPos(double x,double y){
	//ss.str("");
	//ss << "set key opaque box spacing 3.0 width 0.5 "
	//<< this->key_x_pos <<","<< this->key_y_pos ;
	//this->gp.Out(ss.str());
}

void figure::KeyPos(std::string pos){
	ss.str("");
	ss << "set key " << pos;
	this->Out(ss.str());
}

void figure::GraphStyle(std::string graph_style){
	this->graph_style=graph_style;
}



void figure::EpsMode(std::string filename){
	this->EpsMode(filename, 8.0,6.0);
}


void figure::EpsMode(std::string filename, double x_size, double y_size){
	this->ss.str("");
	this->ss << "set terminal postscript eps enhanced color size "
	<< x_size << "cm, " << y_size << "cm";
	this->Out(this->ss.str());
	
	this->ss.str("");
	this->ss << "set output \""<< filename <<"\"";
	this->Out(this->ss.str());
}


void figure::WxtMode(){
	this->WxtMode(this->wxt_x_size,this->wxt_y_size);
}


void figure::WxtMode(int x_size, int y_size){
	this->wxt_x_size=x_size;
	this->wxt_y_size=y_size;
	this->ss.str("");
	this->ss << "set terminal wxt size "
	<< this->wxt_x_size <<","<< this->wxt_y_size ;
	this->Out(ss.str());
}

void figure::StartLog(){
	f_log = true;
}
void figure::StopLog(){
	f_log = false;
}
void figure::ClearLog(){
	this->log.close();
	this->log.open(gp_file_name, ios::trunc);
}
// tips fixing BoundingBox
// fig1.Out("unset output");
// fig1.Out("command = sprintf(\"sed -e \\\"s/%%%%BoundingBox: 50 50/%%%%BoundingBox: 25 25/g\\\" fig_.eps > fig.eps\")");
// fig1.Out("system command");
	



