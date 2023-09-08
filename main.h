#ifndef SIMULATORE_PER_DEBUG_MAIN_H
#define SIMULATORE_PER_DEBUG_MAIN_H

#include <stdint.h>

//Struct
typedef struct {
    double x;	//ascissa
    double y;	//ordinata
    double d;	//distanza
    double a;	//angolo
} point;

typedef struct {
    double velocity;
    double init_time;	//istante moto rettilineo uniforme
    double init_value;
} speed_info;

typedef struct {
    double x;
    double y;
    double facing_angle;
} car_position;

//enum
typedef enum {
    right = 0, left = 1, forward = 2, backward = 3
} direction;

typedef enum {
    slow = 15, normal = 50
} speed;

//costanti motore
extern const int time_to_full_power;

//costanti macchina
extern const int car_half_length, car_half_width, car_half_diagonal, space_needed_to_turn;

//costanti tof
extern const int range_TOF;
extern const uint16_t timingBudget;

//costanti seriale
extern const int debug_print;

//constanti angoli supportati per la rotazione
extern const int m[5];

//variabili orientamento
extern car_position actual;

//funzioni seriale
void trace(const char *format, ...);

//funzioni tof
int read_approximated_tof(int num_samples);
int read_tof(void);

//funzioni calibrazione
void calibrate_velocity(int init_time, int numSamples, int tipo);
void complete_calibrate(void);

//funzioni orientamento
int rileva_ostacoli(int num_samples, int num_p);
point punto_rispetto_al_centro(double distanza_1, double angolo_1);
double corridoio_tra_due_punti(point ostacolo_A, point ostacolo_B);
int memorizza_ostacoli(double dis_ostacolo_n, double ang_ostacolo_n, int *classe_ostacolo_o, double *ang_ostacolo_o, double *ang_ostacolo_last, int *dis_ostacolo_last);
void move(int quantity, direction dir);
void evaluate_car_position(direction dir, uint32_t total_time, int print);
void reset_car_position(void);
_Noreturn void movement_control(int num_samples);
direction opposite(direction dir);
int controlla_passaggio(int num_corridoi, int passaggio_da_controllare);

//funzioni motore
void motor_control(direction d, speed s);
void stop_motor(void);

#endif
