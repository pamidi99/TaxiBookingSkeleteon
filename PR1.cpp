#include<stdio.h>
#include<stdlib.h>
struct travel{
	int custid;
	int from;
	int to;
	int pickup_hrs;
	int pickup_min;
	int drop_hrs;
	int drop_min;
	int amount_charged;
	struct travel *next;
};
int bookingid;
int numoftaxis;
struct car{
	int earning;
	struct travel *head;
};
struct car* initialise(int n){
	int i;
	struct car *car_array=(struct car*)malloc(n*sizeof(struct car));
	struct car *current=car_array;
	for(i=0;i<n;i++){  
	    current->earning=0;
	    current->head=NULL;
	    current++;
	}
	return car_array;
}
void fill_drop(struct travel *newtravel){
	int dist=(newtravel->from)-(newtravel->to);
	if(dist<0) dist=dist*-1;
	int traveltime=dist*15;
	newtravel->drop_hrs=newtravel->pickup_hrs;
	newtravel->drop_min=newtravel->pickup_min+traveltime;
	if(newtravel->drop_min>=60){
		newtravel->drop_hrs+=newtravel->drop_min/60;
		newtravel->drop_min=newtravel->drop_min%60;
	}
	
}
struct travel* initialise_travel(int custid,int src,int dest,int pickup_hrs,int pickup_min){
/*	printf("\nin intialise travel"); */
	struct travel* newtravel=(struct travel*)malloc(sizeof(struct travel));
	if(newtravel){
		newtravel->custid=custid;
		newtravel->from=src;
		newtravel->to=dest;
		newtravel->pickup_hrs=pickup_hrs;
		newtravel->pickup_min=pickup_min;
		fill_drop(newtravel);
		int dist=(newtravel->from)-(newtravel->to);
		dist=dist*15;
		if(dist<0) dist*=-1;
		newtravel->amount_charged=(dist)*10;
		newtravel->next=NULL;
	}
//	printf("\nout of intialise travel");
	return newtravel;
}
void insert(int taxi_id,struct car* taxis,struct travel *newtravel){
	//	printf("\nin insertt");
	(taxis+taxi_id)->earning+=newtravel->amount_charged;
	newtravel->next=(taxis+taxi_id)->head;
	(taxis+taxi_id)->head=newtravel;
	//	printf("\nout of insert");
}
int isItBusy(struct car *current_taxi,int pickup_hrs,int pickup_min){
	//	printf("\nin is it busy");
	struct travel *travel_list=current_taxi->head;
	while(travel_list!=NULL){
		int required_time=pickup_hrs*60+pickup_min;
		int start_time=(travel_list->pickup_hrs)*60+(travel_list->pickup_min);
		int end_time=(travel_list->drop_hrs)*60+(travel_list->drop_min);
		if(required_time>=start_time&&required_time<=end_time)
		    return -1;
		travel_list=travel_list->next;
	}
	return 1;
//		printf("\nout of is it busy");
}
int findcurrent_point(struct car *taxi,int pickup_hrs,int pickup_min){
//	printf("\nin findpoint");
	int current_point=0;
	int min_time=-1;
	int temp;
	int required_time,end_time;
	struct travel *current=taxi->head;
	while(current!=NULL){
		required_time=pickup_hrs*60+pickup_min;
		end_time=(current->drop_hrs)*60+(current->drop_min);
		if(min_time==-1){
			if(required_time>=end_time){
			min_time=(required_time-end_time); 
			current_point=current->to;
		   }
	   }
	   else{
	   	temp=(required_time-end_time); 
	   	if(required_time>=end_time&&temp<min_time){
	   		min_time=temp; 
			current_point=current->to;
	   	}
	   }
	   current=current->next;
	}
		//printf("\nout of findpoint");
	return current_point;
}
int findTaxiId(int numoftaxis,struct car* taxis,struct travel *newtravel){
	//	printf("\nin findtaxid");
	int i;
	int selected_taxi=-1;
	int min=6;
	int temp;
	int pickup_hrs=newtravel->pickup_hrs;
	int pickup_min=newtravel->pickup_min;
	struct car *currenttaxi=taxis;
	for(i=0;i<numoftaxis;i++){
		if(isItBusy(currenttaxi,pickup_hrs,pickup_min)!=-1){
			if(selected_taxi==-1){
				selected_taxi=i;
				printf("\n curent point of i and dist is %d %d ",i+1,findcurrent_point(currenttaxi,pickup_hrs,pickup_min));
				min=(newtravel->from)-findcurrent_point(currenttaxi,pickup_hrs,pickup_min);
				if(min<0) min*=-1;
				
			}
			else{
				
				temp=(newtravel->from)-findcurrent_point(currenttaxi,pickup_hrs,pickup_min);
			    printf("\n curent point of i and dist is %d %d ",i+1,findcurrent_point(currenttaxi,pickup_hrs,pickup_min));
				if(temp<0) temp*=-1;
			
				if(min>temp){
					min=temp;
					selected_taxi=i;
				}
				else if(min==temp){
					int temp_earning=(taxis+i)->earning;
					int min_earning=(taxis+selected_taxi)->earning;
					if(temp_earning<min_earning){
						min=temp;
					    selected_taxi=i;
					}
				}
		   }
		}
		currenttaxi++;
	}
//	printf("\nout of findtaxid");
	return selected_taxi;
}
void Bookfunction(int numoftaxis,struct car* taxis,struct travel *newtravel){
	int id=findTaxiId(numoftaxis,taxis,newtravel);
	if(id!=-1){
		insert(id,taxis,newtravel);
		printf("\n***Taxi Booked Succesfully****\n");
	    printf("\nBooking id : %d",++bookingid);
	    printf("\nAllotted Taxi id: Taxi %d",id+1);
	}
	else
		printf("No Taxi is Available");
	
}
void printHistory(struct car *car_array,int id){
	struct travel *head=(car_array+id)->head;
	printf("*******Travel history of taxi %d *****",id+1);
	while(head!=NULL){
		printf("\n%c  %c  %d.%d   %d.%d  %d ",(head->from)+65,(head->to)+65,head->pickup_hrs,head->pickup_min,head->drop_hrs,head->drop_min,head->amount_charged);
		head=head->next;
	}
	printf("***********************");
}
int main(){
    struct car *car_array=initialise(5);
    int choice;
    printf("1-Book a Taxi\n 2-display");
    scanf("%d",&choice);
    int custid,pickup_hrs,pickup_min;
    int start,dest;
    struct travel *temp;
    while(choice==1||choice==2){
    	if(choice==1){
    		printf("\nenter cust id,pickup_hrs,pickup_min\n");
    		scanf("%d%d%d%d%d",&custid,&start,&dest,&pickup_hrs,&pickup_min);
    	
    		temp=initialise_travel(custid,start,dest,pickup_hrs,pickup_min);
    		Bookfunction(5,car_array,temp);
    	}
    	else
    	{
    		printf("Enter taxi id");
    		scanf("%d",&custid);
    		printHistory(car_array,custid-1);
    	}
    	printf("\nEnter choice");
    	scanf("%d",&choice);
    }
    
    
}
