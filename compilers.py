from cppcompilers import *
import os

#get object
a = PythonInterface();
#Initializer Compilers
a.CompilersInit();

#get parameters
starFor = a.getStartFor()
endFor = a.getEndFor()
stepFor = a.getStepFor()
drawXFor = a.getDrawXFor()
drawYFor = a.getDrawYFor()
xOrigin = a.getXOrigin()
yOrigin = a.getYOrigin()
rot = a.getRot()
xScale = a.getXScale()
yScale = a.getYScale() 

#build matlab statement
#t=xx:xx:xx;
t = "t=" + starFor + ":" + stepFor + ":" + endFor + ";"
tStatement = t.lower();
#print tStatement

#x=xxxxx
#y=xxxxx
xValue = "x=" + drawXFor + ";"
yValue = "y=" + drawYFor + ";" 

if xScale != "":
	xScaleV = "x=x*" + xScale + ";"
	yScaleV = "y=y*" + yScale + ";"
	xValue = xValue + xScaleV;
	yValue = yValue + yScaleV;

if xOrigin != "":
	xOriginV = "x=x+" + xOrigin + ";"
	yOriginV = "y=y+" + yOrigin + ";"
	xValue = xValue + xOriginV;
	yValue = yValue + yOriginV;

posValue = xValue + yValue

if rot != "":
	xRot = "xr=x*cos(" + rot + ")" + "+" + "y*sin(" + rot + ");"
	yRot = "yr=y*cos(" + rot + ")" + "-" + "x*sin(" + rot + ");"
	oRot = xRot + yRot
	RotV = oRot + "x=xr;y=yr;"
	posValue = posValue + RotV

posStatement = posValue.lower()

#print posStatement

#draw command
drawStatment = "plot(x,y,'-');axis equal;"


#get matlab path
matlabPath = "/usr/local/MATLAB/MATLAB_Production_Server/R2013a/bin/matlab"
#get matlab program path
matlabProgramPath = "/home/sliencer/code/compilers/"
#set matlab parameters
openparameters = "-nosplash -nodesktop"

#drawcmd = "alpha=0:pi/sin(50):2*pi;x = cos(alpha);y = 2*sin(alpha);plot(x,y,'-');axis equal"
drawcmd = tStatement + posStatement + drawStatment

cmd = matlabPath + " " + openparameters + " -r " + '"' +drawcmd + '"'

#print cmd

os.system(cmd)
