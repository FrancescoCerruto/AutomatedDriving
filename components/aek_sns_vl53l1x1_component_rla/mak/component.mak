##############################################################################
# References to the component library (./component/lib) locations.



					
LIB_C_SRC       += 	./components/aek_sns_vl53l1x1_component_rla/lib/src/AEK_SNS_VL53L1X1.c 						\
					./components/aek_sns_vl53l1x1_component_rla/lib/src/uld_src/uld_api.c	  						\
					./components/aek_sns_vl53l1x1_component_rla/lib/src/uld_src/VL53L1X_api.c                      \
					./components/aek_sns_vl53l1x1_component_rla/lib/src/uld_src/VL53L1X_calibration.c				\
					./components/aek_sns_vl53l1x1_component_rla/lib/src/uld_src/VL53L1X_support.c					
					
					
			
					
					
LIB_CPP_SRC     +=
LIB_ASM_SRC     += 
LIB_INCLUDES    += ./components/aek_sns_vl53l1x1_component_rla/lib/include

##############################################################################
# References to the application locations.

APP_C_SRC       += ./components/aek_sns_vl53l1x1_component_rla/cfg/AEK_SNS_VL53L1X1_cfg.c

APP_CPP_SRC     +=

APP_ASM_SRC     +=

APP_INCLUDES    += ./components/aek_sns_vl53l1x1_component_rla/cfg

##############################################################################
# Default directories, definitions and libraries.

# C/C++ definitions (-D....).
DDEFS   =

# Assembled definitions (-D....).
DADEFS  =

# Include files search directories separated by spaces.
DINCDIR =

# Library files search directories separated by spaces.
DLIBDIR =

# Libraries (-l...).
DLIBS   =
