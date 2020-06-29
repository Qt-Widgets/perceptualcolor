# define which checks we want to run
export CLAZY_CHECKS="level2,no-qproperty-without-notify"
# run clazy only for the targets perceptualcolor (the library) and perceptualcolorpicker (the application), but not for the unit tests
rm -r build/* && cd build && cmake ../ -DCMAKE_CXX_COMPILER=clazy && make clean && make perceptualcolor && make perceptualcolorpicker && cd .. && rm -r build/*
