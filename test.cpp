/*
 * test.cpp
 * 
 * Copyright (c) 2014 takako takahito
 * Released under the MIT license
 * http://opensource.org/licenses/mit-license.php
 */

#include "user_std.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "mat.hpp"
#include "pipe.hpp"
#include "beziers.hpp"
#include "common_data.hpp"
#include "figure.hpp"
#include "dh_simplex.hpp"
#include "time_for_filename.hpp"
#include "fifth_polynomial.hpp"

void CalcPressure();

std::stringstream ss;
std::vector<xydata> data;
std::vector<xydata> data_dot;
std::vector<xydata> data_ddot;
std::vector<xydata> data_p;
std::vector<xydata> data_ekii;
std::vector<xydata> data_org;
std::vector<fifth_polynomial> data_ffp;

double SIM_TIME = 4;
double z0, z1, z2, z3;
double p = 0.0;
double z;
double z_dot;
double z_ddot;
double ekii = 0;
double ekii1, ekii2, ekii3;
double z_dot0, z_dot1, z_dot2, z_dot3;
double vol_tmp;
double v1, v2, v3, v4;
double A_ratio = 0;
double A2_ratio = 0;


/*
座標系について
	下鋳型を基準として，上下方向をzとし，下方向が正とする．
	上鋳型底面部の位置速度加速度をz,z_dot,z_ddotとする．
*/
double eval_func(dh_simplex::Simplex &simplex);
void Init();
void PreCalc();
void PlotFigure();
void PlotFigureToWindow();


figure fig1;

int main(){
	Init();
	PreCalc();
	
	fig1.Init("fig.gp");
	
	std::random_device seed_gen;
	std::mt19937 mt(0);
	uniform_real_distribution<> dist(-1,1);
	uniform_real_distribution<> dist_minus(-1,0);
	uniform_real_distribution<> dist_plus(0,1);
	
	dh_simplex::DhSimplex dhsimplex;
	std::vector<dh_simplex::Simplex> simplexes;
	dh_simplex::Simplex simplex,simplex_;
	simplex.resize(SIMPLEX_SIZE);
	simplex_.resize(SIMPLEX_SIZE);
	
	for(int i = 0; i < (SIMPLEX_SIZE + 6); i++){
		simplex(0) = 0 + dist_plus(mt) / 100.0; // x0_dot
		simplex(1) = 0 + dist(mt) / 100.0; // x0_ddot
		simplex(2) = 0 + dist(mt) / 100.0; // x1_dot
		simplex(3) = 0 + dist(mt) / 100.0; // x1_ddot
		simplex(4) = 1 + dist(mt) / 10.0; // x1_t
		simplex(5) = 0 + dist(mt) / 100.0; // x2_dot
		simplex(6) = 0 + dist(mt) / 100.0; // x2_ddot
		simplex(7) = 1 + dist(mt) / 10.0; // x2_t
		simplex(8) = 0 + dist_minus(mt) / 100.0; // x3_ddot
		simplex(9) = 1 + dist(mt) / 100.0; // x3_t
		simplexes.push_back(simplex);
	}
	dhsimplex.Init(simplexes, &eval_func, SIMPLEX_SIZE);
	
	double k_rand=20;
	for(int i = 0; 1; i++){
		if(i>100){
			i=0;
			fig1.ClearLog();
			PlotFigure();
			dhsimplex.get_min_simplex(simplex_);
			cout << "min_simplex : " << simplex_ << endl;
			cout << "max : " << dhsimplex.get_max() << endl;
			cout << "min : " << dhsimplex.get_min() << endl;
			cout << "gap : " << dhsimplex.get_gap() << endl;
			simplexes.resize(0);
			for(int i = 0; i < (SIMPLEX_SIZE + 6); i++){
				if(i==0){
					simplex=simplex_;
					simplexes.push_back(simplex);
					continue;
				}
				simplex(0) = simplex_(0)*(1.0+k_rand*dist(mt)/100);
				simplex(1) = simplex_(1)*(1.0+k_rand*dist(mt)/100);
				simplex(2) = simplex_(2)*(1.0+k_rand*dist(mt)/100);
				simplex(3) = simplex_(3)*(1.0+k_rand*dist(mt)/100);
				simplex(4) = simplex_(4)*(1.0+k_rand*dist(mt)/100);
				simplex(5) = simplex_(5)*(1.0+k_rand*dist(mt)/100);
				simplex(6) = simplex_(6)*(1.0+k_rand*dist(mt)/100);
				simplex(7) = simplex_(7)*(1.0+k_rand*dist(mt)/100);
				simplex(8) = simplex_(8)*(1.0+k_rand*dist(mt)/100);
				simplex(9) = simplex_(9)*(1.0+k_rand*dist(mt)/100);
				simplexes.push_back(simplex);
			}
			dhsimplex.Init(simplexes, &eval_func, SIMPLEX_SIZE);
		}
		dhsimplex.Do();
		//cout << i << endl;
		
	}
	
	
	cin.get();
	return 0;
}

void CalcPressure(){
	data_p.resize(0);
	data_ekii.resize(0);
	int i=0;
	for(double t=0;t<SIM_TIME;t+=STEP){
		while(data[i].x < t){
			i++;
			if(data.size()-1 < i){
				break;
			}
		}
		z=data[i].y;
		z_dot=data_dot[i].y;
		z_ddot=data_ddot[i].y;
		
		v1=(-z)*Ab1;
		v2=(Ab1-Au1)*(Bb-(-z));
		v3=(Ab2-Au1)*(Bu+(-z)-Bb);
		if( v1 <= VOL && VOL < v1+v2){
			vol_tmp=VOL-(v1);
			ekii=vol_tmp/(Ab1-Au1);
			A_ratio=Au1/(Ab1-Au1);
		}
		if( v1+v2 <= VOL && VOL < v1+v2+v3){
			vol_tmp=VOL-(v1+v2);
			ekii=(Bb-(-z))+vol_tmp/(Ab2-Au1);
			
			A_ratio=Au1/(Ab2-Au1);
		}
		if( v1+v2+v3 <= VOL){
			vol_tmp=VOL-(v1+v2+v3);
			ekii=Bu+vol_tmp/(Ab2-Au2);
			A_ratio=Au2/(Ab2-Au2);
		}
		A2_ratio=pow(A_ratio,2.0);
		double pressure=ekii*Rho*g+0.5*Rho*A2_ratio*pow(z_dot,2.0)+
				Rho*A2_ratio*z_ddot*(z-z0);
		//double pressure=ekii*Rho*g;
		data_p.push_back(xydata({t,pressure}));
	}
}

double eval_func(dh_simplex::Simplex &simplex){
	double value = 0.0;
	fifth_polynomial tmp_ffp;
	data_ffp.resize(0);
	tmp_ffp.xi = z0;
	tmp_ffp.xi_dot = simplex(0);
	tmp_ffp.xi_ddot = simplex(1);
	tmp_ffp.xf = z1;
	tmp_ffp.xf_dot = simplex(2);
	tmp_ffp.xf_ddot = simplex(3);
	tmp_ffp.t_start = 0.0;
	tmp_ffp.t_end = simplex(4);
	data_ffp.push_back(tmp_ffp);
	
	tmp_ffp.xi = tmp_ffp.xf;
	tmp_ffp.xi_dot = tmp_ffp.xf_dot;
	tmp_ffp.xi_ddot = tmp_ffp.xf_ddot;
	tmp_ffp.xf = z2;
	tmp_ffp.xf_dot = simplex(5);
	tmp_ffp.xf_ddot = simplex(6);
	tmp_ffp.t_start = tmp_ffp.t_end ;
	tmp_ffp.t_end = simplex(4) + simplex(7);
	data_ffp.push_back(tmp_ffp);
	
	tmp_ffp.xi = tmp_ffp.xf;
	tmp_ffp.xi_dot = tmp_ffp.xf_dot;
	tmp_ffp.xi_ddot = tmp_ffp.xf_ddot;
	tmp_ffp.xf = z3;
	tmp_ffp.xf_dot = 0.0;
	tmp_ffp.xf_ddot = simplex(8);
	tmp_ffp.t_start = tmp_ffp.t_end ;
	tmp_ffp.t_end = simplex(4) + simplex(7) + simplex(9);
	data_ffp.push_back(tmp_ffp);
	
	SIM_TIME = (simplex(4) + simplex(7) + simplex(9))*1.1;
	
	FifthPolynomials(data_ffp, data, data_dot, data_ddot);
	
	CalcPressure();
	
	auto it1 = find_if(data_dot.begin(), data_dot.end(), 
			[](xydata data) -> bool {return (data.y > 1.0)||(data.y < 0.0);});
	if(it1 != data_dot.end())
		value += 999999;
	
	auto it2 = find_if(data_ddot.begin(), data_ddot.end(), 
			[](xydata data) -> bool {return (data.y > 1.8)||(data.y < -1.8);});
	if(it2 != data_ddot.end())
		value += 999999;
	
	auto it3 = find_if(data_p.begin(), data_p.end(), 
			[](xydata data) -> bool {return (data.y > 1400.0)||(data.y < -1400.0);});
	if(it3 != data_p.end())
		value += 999999;
	
	if( 0 >= simplex(4) || 0 >= simplex(7) || 0 >= simplex(9) )
		value += 999999;
	
	double value_ = 0;
	for(auto it = data_p.begin(); it != data_p.end(); ++it){
		if((*it).x> (simplex(4) + simplex(7) + simplex(9)))
			break;
		value_ += ((*it).y*STEP);
	}
	value_=1.0/(value_/(simplex(4) + simplex(7) + simplex(9))/1500.0) - 1.0;
	value += simplex(4) + simplex(7) + simplex(9);
	
	
	return value;
	// data
	// data_p.x
	// data_p.y
}



void Init(){
	data.clear();
	data_dot.clear();
	data_ddot.clear();
	data_p.clear();
	data_ekii.clear();
	data_org.clear();
	
	string date ;
	TimeForFileName(date);
	
	mkdir(date.c_str(),0775);
	chdir(date.c_str());

}
void PreCalc(){
	z0=-VOL/Ab1;//着水時の位置
	
	//一段目に水が達するときのz
	//VOL=Ab1*(-z1)+(Ab1-Au1)*(Bb-(-z1));
	//二段目に水が達するときのz
	//VOL=Ab1 *(-z2)+(Ab1 - Au1)*(Bb - (-z2))+(Ab2 - Au1)*(Bu +(-z2)- Bb);
	z1 = (VOL-(Ab1-Au1)* Bb)/(-Au1);
	z2 = (VOL-(Ab1 - Au1)*Bb-(Ab2 - Au1)*(Bu- Bb))/(-Ab2);
	z3 = -0.003;//終了時の位置
	
	ekii1=Bb - (-z1);
	ekii2=Bu;
	//VOL=Ab1 *(-z3)+(Ab1 - Au1)*(Bb - (-z3))+(Ab2 - Au1)*(Bu +(-z3)- Bb) + (Ab2 - Au2)*z3_;
	double z3_ = (VOL-(Ab1 *(-z3)+(Ab1 - Au1)*(Bb - (-z3))+(Ab2 - Au1)*(Bu +(-z3)- Bb)))/(Ab2 - Au2);
	ekii3=Bu + z3_;//不要
	
	//形状切り替え点で出せる最大の速度を計算
	z_dot0 = sqrt((2.0 * (Pmax) ) / (Rho)) * (Ab1 - Au1) / (Au1);
	z_dot1 = sqrt((2.0 * (Pmax - ekii1 * Rho * g) ) / (Rho)) * (Ab1 - Au1) / (Au1);
	z_dot2 = sqrt((2.0 * (Pmax - ekii2 * Rho * g) ) / (Rho)) * (Ab2 - Au2) / (Au2);
	z_dot3 = 0;
	
	cout << "ekii1 " << ekii1 << endl;
	cout << "ekii2 " << ekii2 << endl;
	cout << "z_dot0 " << z_dot0 << endl;
	cout << "z_dot1 " << z_dot1 << endl;
	cout << "z_dot2 " << z_dot2 << endl;
	
	/*
	cout << z0 << endl;
	cout << z1 << endl;
	cout << z2 << endl;
	cout << z3 << endl;
	
	cout << "ekii1*rho*g " << ekii1*Rho*g << endl;
	cout << "ekii2*rho*g " << ekii2*Rho*g << endl;
	cout << "ekii3*rho*g " << ekii3*Rho*g << endl;
	*/
}

void PlotFigure(){
	fig1.WxtMode(800,1200);
	
	auto Plot = [&]{
		
		fig1.Out("#\n##########\n#");
		fig1.KeyPos("bottom left");
		//fig1.Out("set ytics 300");
		//fig1.Out("set xrange[0:2]");
		fig1.Out("set multiplot layout 4,1");
		//fig1.Out("set multiplot layout 2,1");
		fig1.Out("set lmargin 12");
		fig1.Out("set rmargin 2");
		fig1.Out("set tmargin 1");
		fig1.Out("set bmargin 1");
		fig1.Out("set xlabel ''");
		fig1.Out("set format x ''");
		
		fig1.Out("set format y '%5.0f'");
		//fig1.Out("set ytics 500");
		fig1.LineColor("red");
		fig1.Out("set autoscale y");
		fig1.Out("set ylabel 'Pressure [Pa]'");
		fig1.Plot("data_p.dat",data_p,"Pressure [Pa]");
		
		//fig1.Out("set ytics 0.01");
		fig1.Out("set format y '%5.2f'");
		fig1.Out("set ylabel 'Position [m]'");
		fig1.Plot("data.dat",data,"Pos [m]");
		//fig1.Out("set yrange[-0.005:0.03]");
		fig1.Out("set ylabel 'Velocity [m/s]'");
		/*	fig1.Out("set bmargin 4");
			fig1.Out("set xlabel 'time [s]'");
			fig1.Out("set format x '%g'");
			*/
		fig1.Plot("data_dot.dat",data_dot,"Vel [m/s]");
		fig1.Out("set bmargin 4");
		fig1.Out("set xlabel 'time [s]'");
		fig1.Out("set format x '%g'");
		//fig1.Out("set yrange[-0.06:0.01]");
		//fig1.Out("set ytics 0.02");
		fig1.Out("set ylabel 'Acceleration [m/s^2]'");
		fig1.Plot("data_ddot.dat",data_ddot,"Acc [m/s^2]");
		fig1.Out("unset multiplot");
		fig1.Out("#\n##########\n#");
		//fig.LineColor("blue");
		//fig.Replot("data.dat",data);
	};
	Plot();
	Plot();
	fig1.EpsMode("fig.eps",8,12);
	Plot();
	
	fig1.Out("set term png size 800*1.5,600*1.5");
	fig1.Out("set output 'fig.png'");
	Plot();
	
	fig1.WxtMode(800,1200);
}

void PlotFigureToWindow(){
	fig1.WxtMode(800,1200);
	
	auto Plot = [&]{
		
		fig1.Out("#\n##########\n#");
		fig1.KeyPos("bottom left");
		//fig1.Out("set ytics 300");
		//fig1.Out("set xrange[0:2]");
		fig1.Out("set multiplot layout 4,1");
		//fig1.Out("set multiplot layout 2,1");
		fig1.Out("set lmargin 12");
		fig1.Out("set rmargin 2");
		fig1.Out("set tmargin 1");
		fig1.Out("set bmargin 1");
		fig1.Out("set xlabel ''");
		fig1.Out("set format x ''");
		
		fig1.Out("set format y '%5.0f'");
		//fig1.Out("set ytics 500");
		fig1.LineColor("red");
		fig1.Out("set autoscale y");
		fig1.Out("set ylabel 'Pressure [Pa]'");
		fig1.Plot("data_p.dat",data_p,"Pressure [Pa]");
		
		//fig1.Out("set ytics 0.01");
		fig1.Out("set format y '%5.2f'");
		fig1.Out("set ylabel 'Position [m]'");
		fig1.Plot("data.dat",data,"Pos [m]");
		//fig1.Out("set yrange[-0.005:0.03]");
		fig1.Out("set ylabel 'Velocity [m/s]'");
		/*	fig1.Out("set bmargin 4");
			fig1.Out("set xlabel 'time [s]'");
			fig1.Out("set format x '%g'");
			*/
		fig1.Plot("data_dot.dat",data_dot,"Vel [m/s]");
		fig1.Out("set bmargin 4");
		fig1.Out("set xlabel 'time [s]'");
		fig1.Out("set format x '%g'");
		//fig1.Out("set yrange[-0.06:0.01]");
		//fig1.Out("set ytics 0.02");
		fig1.Out("set ylabel 'Acceleration [m/s^2]'");
		fig1.Plot("data_ddot.dat",data_ddot,"Acc [m/s^2]");
		fig1.Out("unset multiplot");
		fig1.Out("#\n##########\n#");
		//fig.LineColor("blue");
		//fig.Replot("data.dat",data);
	};
	Plot();
	Plot();
}




