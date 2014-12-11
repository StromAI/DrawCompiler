##About this
g++ -g -shared -fPIC -I/usr/include/python2.7 -g scanner.cpp -g scanner.h -g parse.cpp -g parse.h PythonInterface.cpp -lpython2.7 -lboost_python -o cppcompilers.so
 
