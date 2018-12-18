#include <stdio.h>
#include <stdlib.h>


#define reff 10

float J,B,Ra,La,Kb,Kt,j,Kp,Ki,Kd;

float f=1000,t=0.01;

struct out_value{
	float Ia;
	float W;
	float tetha;
	float time;
	struct out_value *next;
};

struct out_value *head = NULL;


void display(int mode,struct out_value* a)
{
	if(mode==1){
    if (a==NULL){
    	return;
	}
    
    display(mode,a->next);
    printf("%f       %f\n",a->time,a->tetha);}
    
    else if(mode==2){
    if (a==NULL){
    	return;
	}
    
    display(mode,a->next);
    printf("%f       %f\n",a->time,a->W);}
    
    else if(mode==3){
    if (a==NULL){
    	return;
	}
    
    display(mode,a->next);
    printf("%f       %f\n",a->time,a->Ia);}
    
    else if(mode==9){
    if (a==NULL){
    	return;
	}
    
    display(mode,a->next);
    printf("%f\n",a->time);}
}

float search(int mode,int cond,float value,struct out_value* a){
	if(mode==1){
		if (a->next==NULL){
			return;
		}
		else if (a->time>=value&&a->next->time<=value){
			if(cond==1){
				return(a->time);
			}
			else if(cond==2){
				return(a->tetha);
			}
			else if(cond==3){
				return(a->W);
			}
		}
		else if (a->time<=value&&a->next->time>=value){
			if(cond==1){
				return(a->time);
			}
			else if(cond==2){
				return(a->tetha);
			}
			else if(cond==3){
				return(a->W);
			}
		}
		else{
			return(search(mode,cond,value,a->next));
		}
	}
	
	if(mode==2){
		if (a->next==NULL){
			return;
		}
		else if (a->tetha>=value&&a->next->tetha<=value){
			if(cond==1){
				return(a->time);
			}
			else if(cond==2){
				return(a->tetha);
			}
			else if(cond==3){
				return(a->W);
			}
		}
		else if (a->tetha<=value&&a->next->tetha>=value){
			if(cond==1){
				return(a->time);
			}
			else if(cond==2){
				return(a->tetha);
			}
			else if(cond==3){
				return(a->W);
			}
		}
		else{
			return(search(mode,cond,value,a->next));
		}
	}
	
	if(mode==3){
		if (a->next==NULL){
			return;
		}
		else if (a->W>=value&&a->next->W<=value){
			if(cond==1){
				return(a->time);
			}
			else if(cond==2){
				return(a->tetha);
			}
			else if(cond==3){
				return(a->W);
			}
		}
		else if (a->W<=value&&a->next->W>=value){
			if(cond==1){
				return(a->time);
			}
			else if(cond==2){
				return(a->tetha);
			}
			else if(cond==3){
				return(a->W);
			}
		}
		else{
			return(search(mode,cond,value,a->next));
		}
	}
}


void initialize(float input_val[9]){
	J			= 0.02215;
  	Kb			= 1.28;
  	Kt			= 1.28;
  	B			= 0.002953;
  	Ra			= 11.2;
  	La			= 0.1215;
  	
  	Kp			= 2;
  	Ki			= 2;
  	Kd			= 0.5;
  	
  	j=0;
  	
  	int i;
  	for(i=0;i<8;i++){
  		input_val[i]=0;
	  }
	  input_val[8]=reff;
  	
}

void update_motor(float input_val[9]){
	
	//[Vin,Tl,Ia,womega,tetha,Ia_dot_mem,womega_dot_mem,tetha_dot_mem,error_mem]
	float Te,Vb,Ia_dot,womega_dot,tetha_dot;
	Te			= Kt * input_val[2];
 	Vb 			= Kb * input_val[3];
  	Ia_dot		= (input_val[0]-Ra*input_val[2]-Vb)/(La);
  	womega_dot	= (Te-input_val[1]-B*input_val[0])/J;
  	tetha_dot	= input_val[3];
  	
  	
  	input_val[2]			+= (input_val[5] + Ia_dot)*t/2;
	input_val[3]			+= (input_val[6] + womega_dot)*t/2;
	input_val[4]			+= (input_val[7] + tetha_dot)*t/2;
	input_val[5]	= Ia_dot;
	input_val[6]	= womega_dot;
	input_val[7]	= tetha_dot;
	
	
	
}
update_out_v(float input_val[9]){
	
	//[Vin,Tl,Ia,womega,tetha,Ia_dot_mem,womega_dot_mem,tetha_dot_mem,error_mem]
	
	input_val[1]=0.001;
	
	float errorNow 	= reff-input_val[4]; //eror
	float turunan 	= (errorNow - input_val[8])/t;
	float integral 	= (errorNow + input_val[8])/2 *t;
	
	input_val[8]	= errorNow;
	
	input_val[0] = Ki*integral + Kd*turunan + Kp*errorNow;
}

int main(){
	
	float array_val[9];
	initialize(array_val);
	
	while(array_val[4]<0.95*reff){
	j+=1;	
	//update_time();
	update_out_v(array_val);
	update_motor(array_val);
	
	//printf("%f\n",array_val[4]);
	
	struct out_value *out_value = (struct out_value*) malloc(sizeof(struct out_value));
	
	out_value->Ia		= array_val[2];
	out_value->W		= array_val[3];
	out_value->tetha	= array_val[4];
	out_value->time		= j*t;
	out_value->next		= head;
	head				= out_value;
	
	}
	
	int input;
	while(1){
		fflush(stdin);
		while(1){
			input=0;
			printf("---------------------------------------------------------\n");
			printf("        TOOL Model dan analisa transien response\n");
			printf(" \n            dari motor DC pengontrol PID\n");
			printf("---------------------------------------------------------\n");
			
			printf("1. Cari Nilai Tertentu\n");
			printf("2. Tampilkan response\n");
			fflush(stdin);
			scanf("%d",&input);
			system("CLS");
			if(input==1||input==2){
				break;
			}
		}
		if (input==1){
			while(1){
				system("CLS");
				int input0,input1,value;
				printf("1. Cari berdasarkan waktu\n");
				printf("2. Cari berdasarkan tetha\n");
				printf("3. Cari berdasarkan omega\n");
				printf("4. back\n");
				fflush(stdin);
				scanf("%d",&input0);
				if(input0==4){
					system("CLS");
					break;
				}
				
				printf("\n\n1. Cari waktu\n");
				printf("2. Cari tetha\n");
				printf("3. Cari omega\n");
				printf("4. back\n");
				
				fflush(stdin);
				scanf("%d",&input1);
				
				if (input1==4){
					system("CLS");
					break;
				}
				printf("input angka yang ingin dicari: ");
				fflush(stdin);
				scanf("%d",&value);
				
				printf("Hasil yang dicari: %f\n\n",search(input0,input1,value,head));
				
				printf("input 1 untuk back\n");
				fflush(stdin);
				scanf("%d",&value);
				if(value==1){
					system("CLS");
					break;
				}
				
				
			}
			
		}
		else if (input==2){
			while(1){
				int input2;
				printf("1. Tampilkan Response Tetha\n");
				printf("2. Tampilkan Response Omega\n");
				printf("3. Tampilkan Response Arus\n");
				printf("4. Back\n");
				fflush(stdin);
				scanf("%d",&input2);
				system("CLS");
				if(input2==4){
					system("CLS");
					break;
				}
				else if(input==1||input==2||input==3){
					display(input2,head);
				}
				
			}
		}
	}
	
}
