#define BOOST_PYTHON_SOURCE
#include <boost/python.hpp>
#include "parse.h"

using namespace boost::python;


class PythonInterface
{
	//For Statement
	string StartFor;
	string EndFor;
	string StepFor;
	string DrawXFor;
	string DrawYFor;

	//Origin Statement
	string XOrigin;
	string YOrigin;

	//Rot Statement
	string Rot;

	//Scale Statement
	string XScale;
	string YScale;

	//----set-------------------------------------
	void setStartFor(string test){StartFor = test;}
	void setEndFor(string test){EndFor = test;}
	void setStepFor(string test){StepFor = test;}
	void setDrawXFor(string test){DrawXFor = test;}
	void setDrawYFor(string test){DrawYFor = test;}
	void setXOrigin(string test){XOrigin = test;}
	void setYOrigin(string test){YOrigin = test;}
	void setRot(string test){Rot = test;}
	void setXScale(string test){XScale = test;}
	void setYScale(string test){YScale = test;}

public:

	PythonInterface(){}
	void CompilersInit()
	{
		Parse &parse = Parse::Instance();
		parse.Parser("./a.txt");

		setStartFor(parse.getStartFor());
		setEndFor(parse.getEndFor());
		setStepFor(parse.getStepFor());
		setDrawXFor(parse.getDrawXFor());
		setDrawYFor(parse.getDrawYFor());
		setXOrigin(parse.getXOrigin());
		setYOrigin(parse.getYOrigin());
		setRot(parse.getRot());
		setXScale(parse.getXScale());
		setYScale(parse.getYScale());
	}
	//---get---------------------------------
	string getStartFor(){return StartFor;}
	string getEndFor(){return EndFor;}
	string getStepFor(){return StepFor;}
	string getDrawXFor(){return DrawXFor;}
	string getDrawYFor(){return DrawYFor;}
	string getXOrigin(){return XOrigin;}
	string getYOrigin(){return YOrigin;}
	string getRot(){return Rot;}
	string getXScale(){return XScale;}
	string getYScale(){return YScale;}
};

BOOST_PYTHON_MODULE(cppcompilers)
{
	class_<PythonInterface>("PythonInterface","PythonInterface of compiler")
		.def("CompilersInit", &PythonInterface::CompilersInit)
		.def("getStartFor", &PythonInterface::getStartFor)
		.def("getEndFor", &PythonInterface::getEndFor)
		.def("getStepFor", &PythonInterface::getStepFor)
		.def("getDrawXFor", &PythonInterface::getDrawXFor)
		.def("getDrawYFor", &PythonInterface::getDrawYFor)
		.def("getXOrigin", &PythonInterface::getXOrigin)
		.def("getYOrigin", &PythonInterface::getYOrigin)
		.def("getRot", &PythonInterface::getRot)
		.def("getXScale", &PythonInterface::getXScale)
		.def("getYScale", &PythonInterface::getYScale);
}