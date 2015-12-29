#this file 

import ctypes

#some constants
DEG_TO_RAD = 3.14159265/180.0
RAD_TO_DEG = 1.0/DEG_TO_RAD
PI = 3.14159265


t4 = ctypes.cdll.Titan4DLL

_readDouble = t4._read_double
_readDouble.argtypes = [ctypes.c_int]
_readDouble.restype = ctypes.c_double

#wrapper for above
def readDouble(address):
    #call import
    return _readDouble(address)

_readInt = t4._read_int
_readInt.argtypes = [ctypes.c_int]
_readInt.restype = ctypes.c_int

#wrapper for above
def readInt(address):
    #call import
    return _readInt(address)

_writeDouble = t4._write_double
_writeDouble.argtypes = [ctypes.c_int, ctypes.c_double]
_writeDouble.restype = ctypes.c_int

#wrapper for above
def writeDouble(address, value):
    #call import
    return _writeDouble(address, value)

_writeInt = t4._write_int
_writeInt.argtypes = [ctypes.c_int, ctypes.c_int]
_writeInt.restype = ctypes.c_int

#this is a hard-coded list of objects that will be ints
int_list = ['SIM_COMMAND', 'STAGE1_ENGINE_ON', 'STAGE2_ENGINE_ON', 'VIS_COUNT', 'VIS_COMMAND', 'ENGINE_EXTERNAL_CONTROL', 'SIM_PHASE', 'RECORDER_COMMAND']

#wrapper for above
def writeInt(address, value):
    #call import
    return _writeInt(address, value)


#this class defines a "data object" that can be read
class data_object(object):
    address = 0
    name = ''
    #constructor
    def __init__(self, _address, _name):
        self.address = _address
        self.name = _name
    #property function to get it's own value
    def _get_value(self):
        #DEBUG
        #print "get value called."
        #check if it's in the int override list
        if self.name in int_list:
            return readInt(self.address)
        else:
            return readDouble(self.address)
    #property function set it's own value
    def _set_value(self, value):
        #DEBUG
        #print "set value called."        
        #check if it's in the int override list
        if self.name in int_list:
            return writeInt(self.address, value)
        else:
            return writeDouble(self.address, value)
    #property that uses the above two values
    value = property(_get_value, _set_value)
    #representation
    def __repr__(self):
        #DEBUG
        #print "repr called."        
        #return value
        return str(self.value)
    #these two are invoked for a member variable of a class,
    #when one tries to get or set it.
    #it only works if the instance is itself a member of another class.
    def __get__(self, obj, cls):
        #DEBUG
        #print "get called."           
        return self
    #set it's own value 
    def __set__(self, obj, value):
        #DEBUG
        #print "set called."            
        #force a shallow copy (set the new value),
        #rather than the python default "deep copy" (actually becoming the new object)
        return self.set_value(value)
    #overload all the operators to force them to work against
    #the value and not the object itself
    def __lt__(self, other):
            return self.value < other
    def __gt__(self, other):
            return self.value > other
    def __eq__(self, other):
            return self.value == other
    def __ne__(self, other):
            return self.value <> other
    def __le__(self, other):
            return self.value <= other
    def __ge__(self, other):
            return self.value >= other
    def __add__(self, other):
            return self.value + other
    def __sub__(self, other):
            return self.value - other
    def __mul__(self, other):
            return self.value * other
    def __floordiv__(self, other):
            return self.value // other
    def __mod__(self, other):
            return self.value % other
    def __pow__(self, other):
            return pow(self.value, other)
    def __lshift__(self, other):
            return self.value << other
    def __rshift__(self, other):
            return self.value >> other
    def __and__(self, other):
            return self.value & other
    def __xor__(self, other):
            return self.value ^ other
    def __or__(self, other):
            return self.value | other
    def __div__(self, other):
            return self.value / other
    def __truediv__(self, other):
            return self.value / other
    def __radd__(self, other):
            return self.__add__(other)
    def __rsub__(self, other):
            return self.__sub__(other)
    def __rmul__(self, other):
            return self.__mul__(other)
    def __rdiv__(self, other):
            return self.__div__(other)
    def __rtruediv__(self, other):
            return self.__truediv__(other)
    def __rfloordiv__(self, other):
            return self.__floordiv__(other)
    def __rmod__(self, other):
            return self.__mod__(other)
    def __rdivmod__(self, other):
            return self.__divmod__(other)
    def __rlshift__(self, other):
            return self.__lshift__(other)
    def __rrshift__(self, other):
            return self.__rshift__(other)
    def __rand__(self, other):
            return self.__and__(other)
    def __rxor__(self, other):
            return self.__xor__(other)
    def __ror__(self, other):
            return self.__or__(other)
    def __iadd__(self, other):
            self.value = self.value + other
            return self.value
    def __isub__(self, other):
            self.value = self.value - other
            return self.value
    def __imul__(self, other):
            self.value = self.value * other
            return self.value
    def __idiv__(self, other):
            self.value = self.value / other
            return self.value
    def __itruediv__(self, other):
            self.value = self.value / other
            return self.value
    def __ifloordiv__(self, other):
            self.value = self.value // other
            return self.value
    def __imod__(self, other):
            self.value = self.value % other
            return self.value
    def __ipow__(self, other):
            self.value = self.value ** other
            return self.value
    def __ilshift__(self, other):
            self.value = self.value << other
            return self.value
    def __irshift__(self, other):
            self.value = self.value >> other
            return self.value
    def __iand__(self, other):
            self.value = self.value & other
            return self.value
    def __ixor__(self, other):
            self.value = self.value ^ other
            return self.value
    def __ior__(self, other):
            self.value = self.value | other
            return self.value
    def __neg__(self):
            return self.value * -1
    def __pos__(self):
            return self.value
    def __abs__(self):
            return abs(self.value)
    def __invert__(self):
            return ~self.value
    def __complex__(self):
            return complex(self.value)
    def __int__(self):
            return int(self.value)
    def __long__(self):
            return long(self.value)
    def __float__(self):
            return float(self.value)
    def __oct__(self):
            return oct(self.value)
    def __hex__(self):
            return hex(self.value)      

#this is a class that will hold all the parameters
class obj_collection(object):
    #constructor
    def __init__(self):
        #now read in the sm_defines.h file
        infile = open("sm_defines.h", "r")
        #read in the lines
        lines = infile.readlines()
        #close the file
        infile.close()
        #now go through the lines and make objects out of each one
        for line in lines:
            #replace tabs with spaces
            line = line.replace("\t", " ")
            #remove all multiple spaces
            for i in range(0, 10):
                line = line.replace("  ", " ")
            #if the line contains a define, keep processing
            if "#define" in line:
                substrs = line.split(" ")
                #obtain the name, and the address
                name = substrs[1]
                #addr
                addr = int(substrs[2], 16)
                #create a named object using the exec statement
                exec("self." + name + " = data_object(" + hex(addr) + ", '" + name + "')")        
    #modify the setattr function to force the assumption of data_object type
    def __setattr__(self, name, value):
        #DEBUG
        #print "set attr called for name: " + name
        #if the object already exists, check to make sure
        #we don't stomp on a data type we don't want to stomp on
        if name in dir(self):
            obj = eval("self." + name)
            if isinstance(obj, data_object):
                #force it to use the value assignment if it's a data object
                obj.value = value
            else:
                self.__dict__[name] = value
        else:
            self.__dict__[name] = value
            
            
    
#instantiate
global d            
d = obj_collection()


        
