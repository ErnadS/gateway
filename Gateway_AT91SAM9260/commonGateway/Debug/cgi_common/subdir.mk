################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cgi_common/cgi_lib.c \
../cgi_common/cgi_llist.c \
../cgi_common/html_lib.c \
../cgi_common/htmllib.c \
../cgi_common/string_lib.c 

OBJS += \
./cgi_common/cgi_lib.o \
./cgi_common/cgi_llist.o \
./cgi_common/html_lib.o \
./cgi_common/htmllib.o \
./cgi_common/string_lib.o 

C_DEPS += \
./cgi_common/cgi_lib.d \
./cgi_common/cgi_llist.d \
./cgi_common/html_lib.d \
./cgi_common/htmllib.d \
./cgi_common/string_lib.d 


# Each subdirectory must supply rules for building sources it contributes
cgi_common/%.o: ../cgi_common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-angstrom-linux-gnueabi-gcc -I/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


