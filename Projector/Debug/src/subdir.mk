################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GrayCodeUtil.cpp \
../src/PGCapture.cpp \
../src/Projector.cpp 

OBJS += \
./src/GrayCodeUtil.o \
./src/PGCapture.o \
./src/Projector.o 

CPP_DEPS += \
./src/GrayCodeUtil.d \
./src/PGCapture.d \
./src/Projector.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencv -I/usr/lib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


