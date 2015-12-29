#this file implements simulation control functions on top of
#the variables imported by titan7.

#import the math library
import math
#
import time

#bring in the data
execfile("DataInterface.py")

#this encapsulates sending the simulation a command
#(mainly adds the waiting for the sim to acknowledge that it received it
def send_sim_command(cmd_number):
    #dispatch to sim
    d.SIM_COMMAND = cmd_number
    #wait until it sets it back to 0 to confirm completion
    while d.SIM_COMMAND != 0:
        time.sleep(0.001)

#this function calculates ECEF x,y and z for a given lat-long-alt
def calc_ecef_xyz(lat_deg, long_deg, alt_m):
    #convert to radians
    lat_rad = lat_deg * DEG_TO_RAD
    long_rad = long_deg * DEG_TO_RAD
    #convert altitude to distance from earth center
    r_m = alt_m + 6378100.0
    #compute x,y and z
    px = r_m * math.cos(long_rad) * math.cos(lat_rad)
    py = r_m * math.sin(long_rad) * math.cos(lat_rad)
    pz = r_m * math.sin(lat_rad)
    #return the result
    return [px, py, pz]

#this function does an IC with the supplied parameters.
#note that it defaults to a launch at cape canaveral
def ic(ic_long_deg = -80.5585, ic_lat_deg = 28.4667, ic_alt_m = 0, \
       ic_pitch_deg = 90.0, ic_roll_deg = 0.0, ic_yaw_deg = 0.0):
    #get px, py and pz
    [px, py, pz] = calc_ecef_xyz(ic_lat_deg, ic_long_deg, ic_alt_m)
    #convert pitch/roll/yaw to radians
    ic_pitch_rad = ic_pitch_deg * DEG_TO_RAD
    ic_roll_rad = ic_roll_deg * DEG_TO_RAD
    ic_yaw_rad = ic_yaw_deg * DEG_TO_RAD
    #write values to the IC variables:
    #position
    d.IC_PX_ECEF = px
    d.IC_PY_ECEF = py
    d.IC_PZ_ECEF = pz
    #euler angles
    d.IC_PITCH_RAD = ic_pitch_rad
    d.IC_ROLL_RAD = ic_roll_rad
    d.IC_YAW_RAD = ic_yaw_rad
    #set all other needed variables to defaults i don't know what else to do:
    #velocity
    d.IC_VX_ECEF = 0
    d.IC_VY_ECEF = 0
    d.IC_VZ_ECEF = 0
    #angular velocity
    d.IC_WX = 0
    d.IC_WY = 0
    d.IC_WZ = 0
    #mass (indicate to sim to use defaults
    d.IC_MASS_FUEL_FIRST_STAGE = 0
    #set step size and engine control
    d.SIM_STEP_SIZE = 0.05  #50 msec
    #set engine control
    d.ENGINE_EXTERNAL_CONTROL = 0  #internal for the moment
    #tell Display to clear history lines
    clear_history_lines()
    #then send command to the sim to IC
    send_sim_command(1)

#tell the sim to step
def step(step_size = 0.05):
    #set the timestep
    d.SIM_STEP_SIZE = step_size
    #send command to the sim
    send_sim_command(2)

#tell the sim to perform a first stage separation
def first_stage_separation():
    #print out
    print "T = " + str(d.SIM_TIME)[0:6] + ". Commanded first stage separation."
    #send command to the sim
    send_sim_command(100)
    #wait 100 msec to allow it to go through
    time.sleep(0.1)

#tell the GUI to clear the history lines
def clear_history_lines():
    #DEBUG
    print "commanded reset of history lines."
    #send command to visualizer
    d.VIS_COMMAND = 2

#define an engine controller class
class EngineController(object):
    #altitude threshold trigger
    altitude_threshold = False
    #save the last variables
    last_r = 0
    last_theta = 0
    #target r
    target_r = 6378.1 + 290.0
    target_theta = 0
    #
    hit_apogee = False
    hit_seco = False
    #
    terminal_guidance = False

    #utility function to prescribe a tilt angle for the first stage
    #flight, which moves the rocket towards a target pitch angle.
    #the control is based on a PD control that attempts to
    #model the rocket as a critically damped harmonic oscillator
    #in the rotation axis.
    def prescribe_tilt_angle_first_stage(self, _k, target_theta, theta, theta_rate):
        #abort if too early.
        if d.SIM_TIME < 1.0:
            return 0.0
        #define the moment_arm we are using
        lgt = 11.65 #assume this for now, 11.65m
        #define the proportional constant  (note: we could use IYY or IZZ here....the rocket is symmetrical about either, so...)
        k = d.PHASE0_IZZ*_k
        #define the differential constant as the value for the
        #supplied k that would make a critically damped harmonic oscillator
        #out of the resulting system.
        c = 2*math.sqrt(d.PHASE0_IZZ*k)
        #now compute the required torque this would require
        #note: I determined the signs by trial and error.
        tau = k*(theta - target_theta) + c*theta_rate
        #now compute the tilt angle that would create that torque.
        #note we're using the approximation of sin(tilt) = tilt since
        #we won't be using large angles anyway.
        #first, compute the magnitude of the thrust
        thrust_magnitude = math.sqrt(d.THRUST_FX*d.THRUST_FX + d.THRUST_FY*d.THRUST_FY + d.THRUST_FZ *d.THRUST_FZ)
        #if current thrust is 0, return 0 since we tilting is pointless
        #and we can avoid a divide-by-zero later
        if thrust_magnitude == 0:
            return 0
        #otherwise compute.
        #assume the moment arm for the first stage is 11.65m
        tilt_angle = tau / (lgt*thrust_magnitude)
        #return this value
        return tilt_angle
    #same for second stage
    def prescribe_tilt_angle_second_stage(self, _k, target_theta, theta, theta_rate):
        #define the moment_arm we are using
        lgt = 4.13 #assume this for now, 4.13m
        #define the proportional constant
        k = d.PHASE1_STAGE2_IZZ*_k
        #define the differential constant as the value for the
        c = 2*math.sqrt(d.PHASE1_STAGE2_IZZ*k)
        #now compute the required torque this would require
        tau = k*(theta - target_theta) + c*theta_rate
        #now compute the tilt angle that would create that torque.
        #first, compute the magnitude of the thrust
        thrust_magnitude = math.sqrt(d.THRUST_FX*d.THRUST_FX + d.THRUST_FY*d.THRUST_FY + d.THRUST_FZ *d.THRUST_FZ)
        #if current thrust is 0, return 0 since we tilting is pointless
        #and we can avoid a divide-by-zero later
        if thrust_magnitude == 0:
            return 0
        #otherwise compute.
        tilt_angle = tau / (lgt*thrust_magnitude)
        #return this value
        return tilt_angle   
    #this function does the engine control during stage1
    def engine_control_stage1(self, time_step):
        #stage 1 engine control
        #for now, always command the engine to be on
        d.STAGE1_ENGINE_ON = 1
        #compute pitch rate
        theta_rate = (d.PHASE0_PITCH.value - self.last_theta) / time_step
        #save new last value (remember to use value here.)
        self.last_theta = d.PHASE0_PITCH.value
        self.last_r = d.PHASE0_R.value
        #at exactly 30 seconds, print that we're beginning pitch-over maneuver
        if math.fabs(d.SIM_TIME - 30.0) < 0.001:
            print "T = 30.0: beginning pitch-over maneuver."
        #at 35 seconds, finish pitch-over, start gravity turn.
        if math.fabs(d.SIM_TIME - 35.0) < 0.001:
            print "T = 35.0: finish pitch-over at FPA = " + str(d.PHASE0_FLIGHT_PATH_ANGLE * RAD_TO_DEG)[0:5] + " deg. Beginning gravity turn." 
        #before 30 seconds ...beginning...straight up.
        if d.SIM_TIME < 30.0:
            d.STAGE1_ENGINE_GIMBAL_TILT = 0
            d.STAGE1_ENGINE_GIMBAL_ROLL = 0
        #30-35 seconds: start pitch over maneuver
        elif d.SIM_TIME < 35.0:
            d.STAGE1_ENGINE_GIMBAL_TILT = self.prescribe_tilt_angle_first_stage(2.0, 79.0*DEG_TO_RAD, d.PHASE0_PITCH.value, theta_rate) 
            d.STAGE1_ENGINE_GIMBAL_ROLL = d.PHASE0_YAW
        #otherwise do gravity turn maneuver
        else:
            d.STAGE1_ENGINE_GIMBAL_TILT = self.prescribe_tilt_angle_first_stage(5.0, d.PHASE0_FLIGHT_PATH_ANGLE.value, d.PHASE0_PITCH.value, theta_rate) 
            d.STAGE1_ENGINE_GIMBAL_ROLL = d.PHASE0_YAW     
    #this function does the engine control during stage2
    def engine_control_stage2(self, time_step):
        #stage 2 engine control
        #for now, always command the engine to be on
        if self.hit_seco == False:
            d.STAGE2_ENGINE_ON = 1           
        #compute pitch rate
        theta_rate = (d.PHASE1_STAGE2_PITCH.value - self.last_theta) / time_step
        #if current r is previous one, we hit apogee, and we need to halt simulation
        if d.PHASE1_STAGE2_R.value < self.last_r:
            if self.hit_apogee == False:
                #calculate the velocity we reached.
                v = math.sqrt(d.PHASE1_STAGE2_VX * d.PHASE1_STAGE2_VX + d.PHASE1_STAGE2_VY * d.PHASE1_STAGE2_VY +  d.PHASE1_STAGE2_VZ * d.PHASE1_STAGE2_VZ)
                print "T = " + str(d.SIM_TIME)[0:6] + ". Reached apogee at altitude: " + str(self.last_r - 6378.1)[0:7] + " km. velocity: " + str(v)[0:6] + " m/s."
                self.hit_apogee = True
        #if we've run out of fuel
        if d.PHASE1_STAGE2_MASS <= 590.0:  #leave 50kg of fuel
            if self.hit_seco == False:
                #turn off engine
                d.STAGE2_ENGINE_ON = 0
                #calculate the velocity we reached.
                v = math.sqrt(d.PHASE1_STAGE2_VX * d.PHASE1_STAGE2_VX + d.PHASE1_STAGE2_VY * d.PHASE1_STAGE2_VY +  d.PHASE1_STAGE2_VZ * d.PHASE1_STAGE2_VZ)                
                print "T = " + str(d.SIM_TIME)[0:6] + ". Hit SECO. Altitude: " + str(self.last_r - 6378.1)[0:7] + " km. velocity: " + str(v)[0:6] + " m/s."
                self.hit_seco = True
        #abort once both are true
        if self.hit_seco and self.hit_apogee:
            pass
            #return 0
        #save new last value (remember to use value here.)
        self.last_theta = d.PHASE1_STAGE2_PITCH.value
        self.last_r = d.PHASE1_STAGE2_R.value
        #if math.fabs(d.SIM_TIME - 30.0) < 0.001:
        #    print "T = 30.0: beginning pitch-over maneuver."
        #do gravity turn maneuver
        if d.PHASE1_STAGE2_FLIGHT_PATH_ANGLE.value >= (5.0*DEG_TO_RAD):
            d.STAGE2_ENGINE_GIMBAL_TILT = self.prescribe_tilt_angle_second_stage(8.0, d.PHASE1_STAGE2_FLIGHT_PATH_ANGLE.value, d.PHASE1_STAGE2_PITCH.value, theta_rate) 
            d.STAGE2_ENGINE_GIMBAL_ROLL = d.PHASE1_STAGE2_YAW   
        #otherwise go into terminal guidance
        else:
            if self.terminal_guidance == False:
                self.terminal_guidance = True
                #DEBUG
                print "T = " + str(d.SIM_TIME)[0:6] + ". Engage Terminal Guidance mode."
            d.STAGE2_ENGINE_GIMBAL_TILT = self.prescribe_tilt_angle_second_stage(8.0, 10.0*DEG_TO_RAD, d.PHASE1_STAGE2_PITCH.value, theta_rate) 
            d.STAGE2_ENGINE_GIMBAL_ROLL = d.PHASE1_STAGE2_YAW     
    #main engine control function
    def engine_control(self, time_step):
        if d.SIM_PHASE == 0:
            return self.engine_control_stage1(time_step)
        elif d.SIM_PHASE == 1:
            return self.engine_control_stage2(time_step)         
          
#instantiate
global engine_controller
engine_controller = EngineController()

#this performs a simulation run
def simulation_run(step_size = 0.05):
    #initialize the sim (kwajalein atoll)
    ic(ic_long_deg = 167.7431, ic_lat_deg = 9.0482)
    #initialize the engine controller
    engine_controller.hit_seco = False
    #turn on external engien control
    d.ENGINE_EXTERNAL_CONTROL = 1
    #keep track if we've commmand sep or not
    commanded_sep = False
    #create an infinite loop here:
    while True:
        #step the sim
        step(step_size)
        #run the engine control function
        ret = engine_controller.engine_control(step_size)
        if ret == 0:
            print "Simulation run completed."
            return 0
        #if we're in phase 0, and the stage 1 rocket mass is less than or equal to 7970kg (out of fuel)
        if (d.SIM_PHASE == 0) and (d.PHASE0_MASS <= 7970.0) and (commanded_sep == False):
            first_stage_separation()
            commanded_sep = True
        #if we're in phase 1, and the stage 2 rocket mass is less than or equal to 540kg (empty mass)
        #quit
        if (d.SIM_PHASE == 1) and (d.PHASE1_STAGE2_MASS <= 540.0):
            #break
            pass
        
