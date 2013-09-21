CFLAGS = `pkg-config opencv --libs --cflags` -Wall -g
LIBS=-pthread -lboost_regex -lboost_system -lboost_program_options
all:
	clang core.cpp image_kb.cpp kb.cpp ../control_sys/robosub_controller.cpp -o ai ../control_sys/robosub_control_data.cpp ../control_sys/arduino_data.cpp ../AIFunctionCall/checkImage.cpp -I $(CFLAGS) $(LIBS)

clean:
	rm -rf *.o
