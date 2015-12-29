//these numbers are approximated off a chart,  Figure 4.3 of the textbook
//"Rocket Propulsion Elements" by Sutton....

//these are for a V2 rocket. 
//this is because this is the only rocket I have found hypersonic drag values for,
//for anything even close to a Falcon or any other kind of rocket.

//if I wanted accurate values, I would probably have to buy HyperCFD and input the geometry of
//the Falcon rocket and see what I get.

#define DRAG_TABLE_LENGTH	28

//mach numbers
const double M_table[28] = {0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0,
3.2, 3.4, 3.6, 3.8, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.2, 5.4};

//base drag numbers (at zero angle of attack)
const double cd0_table[28] = {0.145, 0.145, 0.145, 0.145, 0.180,     //0.0 - 0.8
						  0.30, 0.41, 0.36, 0.30, 0.26,          //1.0 - 1.8
						  0.242, 0.24, 0.23, 0.22, 0.21,         //2.0 - 2.8
						  0.205, 0.20, 0.195, 0.19, 0.18,		 //3.0 - 3.8
						  0.175, 0.17, 0.165, 0.16, 0.155,       //4.0 - 4.8
						  0.152, 0.150, 0.148};					//5.0-5.4

//linear coefficient of angle of attack on drag, per degree of angle of attack
const double cdaoa_deg_table[28] = {0.02, 0.02, 0.02, 0.02, 0.02,   //0.0 - 0.8
								0.025, 0.025, 0.025, 0.025, 0.025,    //1.0 - 1.8
								0.025, 0.025, 0.025, 0.025, 0.025,    //2.0 - 2.8
								0.025, 0.0235, 0.021, 0.0195, 0.018,    //3.0 - 3.8
								0.017, 0.016, 0.015, 0.014, 0.013,     //4.0 - 4.8
								0.0125, 0.0125, 0.0125};               //5.0 - 5.4

								
