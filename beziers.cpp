#include "beziers.hpp"

double Bezier(double t,double p0,double p1,double p2,double p3);
double BezierDot(double t,double p0,double p1,double p2,double p3);
double BezierDdot(double t,double p0,double p1,double p2,double p3);

void Beziers(std::vector<xydata> data_org,
				std::vector<xydata> &data,
				std::vector<xydata> &data_dot,
				std::vector<xydata> &data_ddot){
	
	double t_,d0,d1,p0,p1,p2,p3;
	int i;
	for(double t=0;t<SIM_TIME;t+=STEP){
		if(data_org[i].x<=t){
			i++;
			if(data_org.size()<=i){
				i--;
				data.push_back(xydata(
					{t,
					data_org[i].y
					}));
				data_dot.push_back(xydata(
					{t,
					0.0
					}));
				data_ddot.push_back(xydata(
					{t,
					0.0
					}));
				continue;
			}
		}
		if(i<=1 && i==data_org.size()-1){
			t_=(t-data_org[i-1].x)/(data_org[i].x-
				data_org[i-1].x);
			//d0=0.0;
			d0=(data_org[i].y-data_org[i-1].y)/
				(data_org[i].x-data_org[i-1].x);
			d1=0.0;
			p0=data_org[i-1].y;
			p1=data_org[i-1].y+d0*(data_org[i].x-
				data_org[i-1].x)/2.0;
			p2=data_org[i].y-d1*(data_org[i].x-
				data_org[i-1].x)/2.0;
			p3=data_org[i].y;
		}else if(i<=1){
			t_=(t-data_org[i-1].x)/(data_org[i].x-
				data_org[i-1].x);
			//d0=0.0;
			d0=(data_org[i].y-data_org[i-1].y)/
				(data_org[i].x-data_org[i-1].x);
			d1=(data_org[i+1].y-data_org[i-1].y)/
				(data_org[i+1].x-data_org[i-1].x);
			p0=data_org[i-1].y;
			p1=data_org[i-1].y+d0*(data_org[i].x-
				data_org[i-1].x)/2.0;
			p2=data_org[i].y-d1*(data_org[i].x-
				data_org[i-1].x)/2.0;
			p3=data_org[i].y;
		}
		else if(i==data_org.size()-1){
			t_=(t-data_org[i-1].x)/(data_org[i].x-
				data_org[i-1].x);
			d0=(data_org[i].y-data_org[i-2].y)/
				(data_org[i].x-data_org[i-2].x);
			d1=0.0;
			p0=data_org[i-1].y;
			p1=data_org[i-1].y+d0*(data_org[i].x-
				data_org[i-1].x)/2.0;
			p2=data_org[i].y-d1*(data_org[i].x-
				data_org[i-1].x)/2.0;
			p3=data_org[i].y;
		}else{
			t_=(t-data_org[i-1].x)/(data_org[i].x-
				data_org[i-1].x);
			d0=(data_org[i].y-data_org[i-2].y)/
				(data_org[i].x-data_org[i-2].x);
			d1=(data_org[i+1].y-data_org[i-1].y)/
				(data_org[i+1].x-data_org[i-1].x);
			p0=data_org[i-1].y;
			p1=data_org[i-1].y+d0*(data_org[i].x-
				data_org[i-1].x)/2.0;
			p2=data_org[i].y-d1*(data_org[i].x-
				data_org[i-1].x)/2.0;
			p3=data_org[i].y;
		}
		data.push_back(xydata(
		{t,
		Bezier(t_,p0,p1,p2,p3)
		}));
		
		data_dot.push_back(xydata(
		{t,
		BezierDot(t_,p0,p1,p2,p3)
		}));
		
		data_ddot.push_back(xydata(
		{t,
		BezierDdot(t_,p0,p1,p2,p3)
		}));
	}
	
}

double Bezier(double t,double p0,double p1,double p2,double p3){
	return p0 * pow(1-t, 3) + p1 * 3.0 * pow(1-t, 2) * t +
		p2 * 3.0 * pow(t, 2) * (1-t) + p3 * pow(t, 3);
}

double BezierDot(double t,double p0,double p1,double p2,double p3){
	return 	p0 * (-3) * pow(1-t, 2) +
			p1 * 3.0 * (pow(1-t,2) -2 * t * (1-t)) +
			p2 * 3.0 * (2 * t * (1-t) - pow(t, 2) ) +
			p3 * 3 * pow(t, 2);
}

double BezierDdot(double t,double p0,double p1,double p2,double p3){
	return 	p0 * 6.0 * (1 - t) +
			p1 * 6.0 * (3*t - 2) +
			p2 * 6.0 * (1 - 3 * t) +
			p3 * 6.0 * t ;
}
