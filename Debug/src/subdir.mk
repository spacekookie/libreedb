################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/core.c \
../src/hash_test.c \
../src/test.c \
../src/uuid.c \
../src/vault.c 

OBJS += \
./src/core.o \
./src/hash_test.o \
./src/test.o \
./src/uuid.o \
./src/vault.o 

C_DEPS += \
./src/core.d \
./src/hash_test.d \
./src/test.d \
./src/uuid.d \
./src/vault.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


