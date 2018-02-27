
void __irq PulseIn_0_Process(void);
void __irq PulseIn_1_Process(void);
void __irq PulseIn_2_3_Process(void);

void InPulse1_Start(void);
void InPulse1_Stop(void);
void InPulse2_Start(void);
void InPulse2_Stop(void);
void InPulse3_Start(void);
void InPulse3_Stop(void);
void Pulse_In_Init(void);


void __irq PulseOut_1_Process(void);

void PulseOut_1_Start(unsigned int frequence, unsigned int pulse_PreSet);
void PulseOut_1_Stop(void);
void Pulse_Out_Init(void);
void PulseOut_2_Stop(void);
void PulseOut_3_Stop(void);



void __irq DA0_10_Process(void);
void DA0_10_Start(float voltage);
void DA0_10_Stop(void);
void DA0_10_Init(void);



void X_In_Init(void);
void Y_Out_Init(void);		



unsigned char Get_X_Value(unsigned char X_num);
void Set_Y_Value(unsigned char Y_num, unsigned char Value);






