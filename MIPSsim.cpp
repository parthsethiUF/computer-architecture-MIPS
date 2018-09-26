/* On my honor, I have neither given nor received unauthorized aid on this assignment */

// Made By Parth Sethi

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <math.h>
#include <iomanip>
using namespace std;
//registers--
int registers[32] = {0};
int datastack[1000000] = {0};
string instrarray[100000] = "";
int memory_location = 256;
int isbreak = 0, awi = 0, dataamount =  0;
fstream de;
int count = 0, c=0;
string end="";
int stringtoint(string s){
	return atoi(s.c_str());
}
int en;
//-----------

//identification for cat 1
string opcode_cat1(int identity){
	string database[12] = {"J","JR","BEQ","BLTZ","BGTZ","BREAK","SW","LW","SLL","SRL","SRA","NOP"};
	return database[identity];
}

//identification for cat 2
string identification_cat2(int identity){
	string database[12] = {"ADD","SUB","MUL","AND","OR","XOR","NOR","SLT","ADDI","ANDI","ORI","XORI"};
	return database[identity];
}
string database_registers[32] = {"R0","R1","R2","R3","R4","R5","R6","R7","R8","R9","R10","R11","R12","R13","R14","R15","R16","R17","R18","R19","R20","R21","R22","R23","R24","R25","R26","R27","R28","R29","R30","R31"};
//tell about registers
string register_no(int identity){
	return database_registers[identity];
}

//find index for registers or instruction address
int find_index(string s){
	//send back register no. index
	for(int i =0; i<32 ; i++){
		if(!database_registers[i].compare(s)){
			return i;
		}
	}
	//send back memory location of instruction
	for(int i =0; i < count ; i++){
		if(!instrarray[i].compare(s)){
			return ((i*4)+256);
		}
	}
	
}

//print for simulator------------------------------
void print_for_simulator(fstream& sim)
{
	if(c != 0){
		sim << "\n" ;
	}

	//cout << "--------------------\nCycle:"<< ++c <<"\t"<<find_index(instr)<<"\t"<<instr.substr(37,instr.length()-1)<<"\n";
	sim << "Registers";
	for(int i = 0; i < 32 ; i++){
		if(i%8 == 0)
		sim << "\nR" << setfill('0') << std::setw(2) << i <<":\t";
		
		sim << registers[i] << "\t";	
	}
	sim << "\n\nData";
	int dummyawi = awi -4;
	for(int i = 0 ; i < dataamount ; i++){
		dummyawi = dummyawi + 4;
		if(i%8 == 0){
		sim << "\n" << dummyawi << ":\t";
	
		}
		sim << datastack[i] << "\t";

	}
	
	sim << "\n";
	
}
//-------------------------------------------------



//unsigned binary to decimal
int binary_to_int(string binaryString){
	int value = 0;
	int indexCounter = 0;
	for(int i=binaryString.length()-1;i>=0;i--){
	
	    if(binaryString[i]=='1'){
	        value += pow(2, indexCounter);
	    }
    	indexCounter++;
	}
	return value;
}

// signed decimal to binary
string int_to_binary(int num)
{
    int r;
    string s = "";
    string rem="";
    if(num > -1){
    for(r = 0 ; num >0; num = num/2)
  	{
    	r = num % 2;
    	if(r == 0)
    		rem = "0";
    	else
			rem = "1";
		s = rem + s; 		
	  }  
	stringstream sim;
	sim <<setfill('0') << std::setw(32) << s;
	s = sim.str();
	}	
	else {
		
	num = (num * (-1)) - 1 ;	
    for(r = 0 ; num > 0; num = num/2)
  	{
    	r = num % 2;
    	if(r == 0)
    		rem = "1";
    	else
			rem = "0";
		s =  rem + s; 		
	  }  
	stringstream sim;
	sim <<setfill('1') << std::setw(32) << s;
	s = sim.str();
	}
	return s;
}

//signed binary to decimal
int signed_binary_to_decimal(string str)
 {
    int value = 0, multiplier = 1;
	int indexCounter = 0;
	if(str[0]=='1'){
	        multiplier = -1;
	}
	for(int i=str.length()-1;i>=0;i--){
	
	    if(str[i]=='0' && multiplier == -1){
	        value += pow(2, indexCounter);
	    }
	    if(str[i]=='1' && multiplier == 1){
	        value += pow(2, indexCounter);
	    }
    	indexCounter++;
	}
	if(multiplier == -1)
		value += 1;
	memory_location += 4;
	return value * multiplier;
 }
//find index for registers or instruction address
int find_instrarray(string s){
	//send back memory location of instruction
	for(int i =0; i < count ; i++){
		if(!instrarray[i].compare(s)){
			return (i);
		}
	}
	
}
template <typename T>
string ntos ( T number )
  {
     ostringstream stream;
     stream << number;
     return stream.str();
  }
  
//----------------------------------------all 24 functions definations------------------------------------
//----------------------------------------CATEGORY 1 -----------------------------------------------------
void BEQ(int &i, int r1, int r2, int value){
	//cout << "==" << i << "==" << registers[find_index(r1)] << "=="<< registers[find_index(r2)];
	if(registers[r1] == registers[r2]){
		i = i +(value / 4 );
	}
	//cout << "==" << i;
}
void BGTZ(int &i, int r1, int value){
 	if(registers[r1] > 0){
		i = i + (value / 4 );
	}

}

void BLTZ(int &i, int r1, int value){
	if(registers[r1] < 0){
		i = i +(value / 4 );
	}
}

void BREAK(){
	isbreak = 1;
}

void NOP(){
	// do nothing, NO OPERATION.
}

void J(int &i, int value){
		
		i = ((value -256) / 4) - 1;
}

void JR(int &i, int r1){
		i =  ((registers[r1] -256) / 4) - 1;
}

void LW(int rt, int offset ,int rs){
	int indx = ( (registers[rs] + offset) - awi ) / 4 ;
	registers[rt] = datastack[indx];
}

void SW(int rt, int offset ,int rs){
	int indx = ( (registers[rs] + offset) - awi ) / 4 ;
	datastack[indx] = registers[rt];
}


void SLL(int rd, int rt ,int value){
	int newvalue = registers[rt];
	string newbits = int_to_binary(newvalue);
	//cout << "new=" << newbits<<"\n";
	for(int i = 0; i < value; i++)
		{
			newbits = newbits + "0";
			newbits = newbits.substr(1,newbits.length()-1);
		}
	//cout <<"new="<< newbits << "\nlength = "<<newbits.length();
	registers[rd] = signed_binary_to_decimal(newbits);
	//registers[rd] = registers[rt] * pow(2,value);
}
void SRL(int rd, int rt ,int value){
	int newvalue = registers[rt];
	string newbits = int_to_binary(newvalue);
	//cout << "new=" << newbits<<"\n";
	for(int i = 0; i < value; i++)
		{
			newbits = "0" + newbits ;
			newbits = newbits.substr(0,newbits.length()-1);
		}
	//cout <<"new="<< newbits<<"\n";
	registers[rd] = signed_binary_to_decimal(newbits);
	//registers[rd] = registers[rt] * pow(2,value);
}
void SRA(int rd, int rt ,int value){
	int newvalue = registers[rt];
	string newbits = int_to_binary(newvalue);
	//cout << "new=" << newbits<<"\n";
	for(int i = 0; i < value; i++)
		{
			if(newvalue > 0)
			newbits = "0" + newbits ;
			else 
			newbits = "1" + newbits ;
			newbits = newbits.substr(0,newbits.length()-1);
		}
	//cout <<"new="<< newbits<<"\n";
	registers[rd] = signed_binary_to_decimal(newbits);
	//registers[rd] = registers[rt] * pow(2,value);
}
//--------------------------------------------------------------------------------------------------------
//----------------------------------------CATEGORY 2 -----------------------------------------------------

void ADD(int r1, int r2, int r3){
	registers[r1] = (registers[r2]) + (registers[r3]);
}

void SUB(int r1, int r2, int r3){
	registers[r1] = (registers[r2] )- (registers[r3]);
}

void MUL(int r1, int r2, int r3){
	registers[r1] = (registers[r2]) * (registers[r3]);
}

void ADDI(int r1, int r2, int value){
	registers[r1] = (registers[r2]) + (value);
}

void AND(int r1, int r2, int r3){
	registers[r1] = (registers[r2]) & (registers[r3]);
}

void OR(int r1, int r2, int r3){
	registers[r1] = (registers[r2]) | (registers[r3]);
}

void NOR(int r1, int r2, int r3){
	registers[r1] = ~( (registers[r2]) | (registers[r3])) ;
}

void XOR(int r1, int r2, int r3){
	registers[r1] = (registers[r2])^ (registers[r3]);
}
void ANDI(int r1, int r2, int value){
	registers[r1] = (registers[r2]) & (value);
}

void ORI(int r1, int r2, int value){
	registers[r1] = (registers[r2]) | (value);
}

void XORI(int r1, int r2, int value){
	registers[r1] = (registers[r2])^ (value);
}

void SLT(int r1, int r2, int r3){
	registers[r1] = (registers[r2]) < (registers[r3]) ? 1:0;
}


//--------------------------------------------------------------------------------------------------------  
// disassembler category 1-------------------------------------------------------
void disassembler_ca1(string check, fstream &de){
	string opcode = opcode_cat1(binary_to_int(check.substr(2,4)));
	string instr = "";
	if(!opcode.compare("BEQ")){
		//BEQ RS RT offset, 5 5 16
		instrarray[count]  =  check + "\t"+ ntos(memory_location)+"\t"+ opcode + " "+ ntos(register_no(binary_to_int(check.substr(6,5)))) +", "+ntos(register_no(binary_to_int(check.substr(11,5)))) +", #" + ntos(binary_to_int(check.substr(16,16)) * 4) +"\n";
		de << instrarray[count++] ;
	}
	if(!opcode.compare("BGTZ")){
		//BGTZ RS offset, 5 - (00000) - 16
		instrarray[count] = check + "\t"+ ntos(memory_location) +"\t" + opcode + " "+ ntos(register_no(binary_to_int(check.substr(6,5)))) +", #"+ntos(binary_to_int(check.substr(16,16)) * 4) +"\n";
		de << instrarray[count++] ;
	}
	if(!opcode.compare("BLTZ")){
		instrarray[count]  = check + "\t"+ntos(memory_location)+"\t"+ opcode +" " + ntos(register_no(binary_to_int(check.substr(6,5)))) +", #"+ ntos(binary_to_int(check.substr(16,16)) * 4) +"\n";
		de << instrarray[count++] ;
	}	//BlTZ RS offset, 5 - (00000) - 16
	
	if(!opcode.compare("BREAK")){
		//BREAK
		isbreak = 1;
		instrarray[count] = check + "\t" + ntos(memory_location) + "\t"  + opcode + "\n" ;
		de << instrarray[count++] ;
	}
	if(!opcode.compare("J")){
		//J target;
		instrarray[count]  = check + "\t" + ntos(memory_location) +"\t" + opcode + " #" + ntos(binary_to_int(check.substr(6,26)) * 4) + "\n" ;
		de << instrarray[count++] ;
	}
	if(!opcode.compare("JR")){
		//JR rs;
		instrarray[count]  = check + "\t" + ntos(memory_location) + "\t" + opcode + " " + ntos(register_no(binary_to_int(check.substr(6,5)))) + "\n" ;
		de << instrarray[count++] ;
	}
	if(!opcode.compare("LW")){
		//LW RT offset(RS), 5 16 5
		instrarray[count] = check + "\t" + ntos(memory_location) + "\t" + opcode + " " + ntos(register_no(binary_to_int(check.substr(11,5)))) + ", " + ntos(binary_to_int(check.substr(16,16))) +"(" + ntos(register_no(binary_to_int(check.substr(6,5)))) + ")\n";
		de << instrarray[count++] ;
	}
	if(!opcode.compare("NOP")){
		//NOP
		instrarray[count]  = check + "\t" + ntos(memory_location) + "\t" + opcode +"\n";
		de << instrarray[count++] ;
	}
	if(!opcode.compare("SLL")){
		//SLL RD RT SA
		instrarray[count]  = check + "\t" + ntos(memory_location) + "\t" + opcode +" " + ntos(register_no(binary_to_int(check.substr(16,5)))) + ", " + ntos(register_no(binary_to_int(check.substr(11,5)))) + ", #" + ntos(binary_to_int(check.substr(21,5))) + "\n";
		de << instrarray[count++] ;
	}
	if(!opcode.compare("SRA")){
		//SRA RD RT SA
		instrarray[count]  = check + "\t" + ntos(memory_location) + "\t" + opcode + " " + ntos(register_no(binary_to_int(check.substr(16,5)))) + ", " + ntos(register_no(binary_to_int(check.substr(11,5)))) + ", #" + ntos(binary_to_int(check.substr(21,5))) + "\n";
		de << instrarray[count++] ;
	}
	if(!opcode.compare("SRL")){
		//SRL RD RT SA
		instrarray[count]  = check + "\t" + ntos(memory_location) + "\t" + opcode + " " + ntos(register_no(binary_to_int(check.substr(16,5)))) + ", " + ntos(register_no(binary_to_int(check.substr(11,5)))) + ", #" + ntos(binary_to_int(check.substr(21,5))) + "\n";
		de << instrarray[count++] ;
	}	
	if(!opcode.compare("SW")){
		//SW RT offset(value), 5 5 16
		instrarray[count] = check + "\t" + ntos(memory_location) + "\t" + opcode + " " + ntos(register_no(binary_to_int(check.substr(11,5)))) + ", " + ntos(binary_to_int(check.substr(16,16))) + "(" + ntos(register_no(binary_to_int(check.substr(6,5)))) + ")\n";
		de << instrarray[count++] ;
	}
	//print_for_simulator(instr);
	if(isbreak == 0)
	memory_location += 4;
}
// disassembler category 2-------------------------------------------------------
void disassembler_cat2(int id, string check, fstream &de){
	string idbits = identification_cat2(binary_to_int(check.substr(2,4)));
	string instr = "";
	//direct
	if(id == 0){
		instrarray[count] = check + "\t" + ntos(memory_location) + "\t" + idbits + " " + ntos(register_no(binary_to_int(check.substr(16,5)))) + ", " + ntos(register_no(binary_to_int(check.substr(6,5)))) + ", " + ntos(register_no(binary_to_int(check.substr(11,5)))) + "\n";
		de << instrarray[count++] ;
	}
	//immediate
	else{
		instrarray[count] = check + "\t" + ntos(memory_location) + "\t" + idbits + " " + ntos(register_no(binary_to_int(check.substr(11,5)))) + ", " + ntos(register_no(binary_to_int(check.substr(6,5)))) + ", #" + ntos(binary_to_int(check.substr(16,16))) + "\n";
		de << instrarray[count++] ;
	}
	//print_for_simulator(instr);
	if(isbreak == 0)
	memory_location += 4;
}


//check which category instruction is this-----------------------------------------------
void dissassembler(string check){
	//just to open file once---------------------
	if(awi == 0){
	de.open("disassembly.txt",ios_base::out);
	awi = 1;
	}
	//--------------------------------------------
	//check if break has happened or not because after break its just numbers
	if(isbreak == 0){
	string catid = "";
	catid = check.substr (0,2);
	if(!catid.compare("01")){
		disassembler_ca1(check, de);
	}
	if(!catid.compare("11")){
		string lastbits = check.substr(21,11);
		int id = lastbits.compare("00000000000") ? 1:0;//id = 0 when direct, id =1 when immediate
		disassembler_cat2(id, check, de);
	}
	
	}
	//-----------------------------------------------------------------
	//if break is there, now only signed numbers are there
	else{
		datastack[dataamount] = signed_binary_to_decimal(check);
		if(awi == 1)
		awi = memory_location;
		de << check <<"\t"<< memory_location<< "\t"<< datastack[dataamount] << "\n";
		dataamount++;
	}
	//------------------------------------------------------------------
}

int simulator(string check){
	int flag;

		flag = 0;
		int i = find_instrarray(check);
//-------------------------------------CATEGORY 1 WORKING CODE---------------------------------------------------------------------
		if(!(check.substr(37,3)).compare("BEQ")&& flag == 0){
		//cout<< instrarray[i]<<"\n";
		BEQ(i , binary_to_int(check.substr(6,5)), binary_to_int(check.substr(11,5)), (binary_to_int(check.substr(16,16)) * 4));
		flag = 1;
		}
		if(!(check.substr(37,4)).compare("BGTZ")&& flag == 0){
		BGTZ(i , binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(16,16)) * 4));
		flag = 1;
		}
		if(!(check.substr(37,4)).compare("BLTZ")&& flag == 0){
		BLTZ(i , binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(16,16)) * 4));
		flag = 1;
		}
		if(!(check.substr(37,5)).compare("BREAK")&& flag == 0){
		BREAK();
		flag = 1;
		}
		if(!(check.substr(37,2)).compare("JR")&& flag == 0){
		JR(i , binary_to_int(check.substr(6,5)));
		flag = 1;
		}
		else if(!(check.substr(37,1)).compare("J")&& flag == 0){
		
		J(i , (binary_to_int(check.substr(6,26)) * 4));
		flag = 1;	
		
		}

		if(!(check.substr(37,2)).compare("LW")&& flag == 0){
		LW(binary_to_int(check.substr(11,5)), binary_to_int(check.substr(16,16)), binary_to_int(check.substr(6,5)));
		flag = 1;
		}
		
		if(!(check.substr(37,2)).compare("SW")&& flag == 0){
		SW(binary_to_int(check.substr(11,5)), binary_to_int(check.substr(16,16)), binary_to_int(check.substr(6,5)));
		flag = 1;
		}
		
		if(!(check.substr(37,3)).compare("SLL")&& flag == 0){
		SLL(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(11,5)), binary_to_int(check.substr(21,5)));
		flag = 1;
		}
		
		if(!(check.substr(37,3)).compare("SRL")&& flag == 0){
		SRL(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(11,5)), binary_to_int(check.substr(21,5)));
		flag = 1;
		}
		
		if(!(check.substr(37,3)).compare("SRA")&& flag == 0){
		SRA(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(11,5)), binary_to_int(check.substr(21,5)));
		flag = 1;
		}
		if(!(check.substr(37,3)).compare("NOP")&& flag == 0){
		NOP();
		flag = 1;
		}
//----------
//---------------------------------------------------------------------------------------------------------------------------------		
//-------------------------------------CATEGORY 2 WORKING CODE---------------------------------------------------------------------
		if(!(check.substr(37,4)).compare("ADDI")&& flag == 0){
			
			ADDI(binary_to_int(check.substr(11,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(16,16))));
			flag = 1;
		}
		else if(!(check.substr(37,3)).compare("ADD")&& flag == 0){
			flag = 1;
			ADD(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(11,5))));
		}
		if(!(check.substr(37,4)).compare("ANDI")&& flag == 0){
			flag = 1;
			ANDI(binary_to_int(check.substr(11,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(16,16))));
		}
		else if(!(check.substr(37,3)).compare("AND")&& flag == 0){
			flag = 1;
			AND(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(11,5))));
		}
		if(!(check.substr(37,4)).compare("XORI")&& flag == 0){
			flag = 1;
			XORI(binary_to_int(check.substr(11,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(16,16))));
		}
		else if(!(check.substr(37,3)).compare("XOR")&& flag == 0){
			flag = 1;
			XOR(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(11,5))));
		}
		if(!(check.substr(37,3)).compare("ORI")&& flag == 0){
			flag = 1;
			 ORI(binary_to_int(check.substr(11,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(16,16))));
		}
		else if(!(check.substr(37,2)).compare("OR")&& flag == 0){
			flag = 1;
			OR(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(11,5))));
		}
		if(!(check.substr(37,3)).compare("SUB")&& flag == 0){
			flag = 1;
			SUB(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(11,5))));
		}
		if(!(check.substr(37,3)).compare("MUL")&& flag == 0){
			flag = 1;
			MUL(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(11,5))));
		}
		if(!(check.substr(37,3)).compare("NOR")&& flag == 0){
			flag = 1;
			NOR(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(11,5))));
		}
		if(!(check.substr(37,3)).compare("SLT") && flag == 0){
			flag = 1;
			SLT(binary_to_int(check.substr(16,5)), binary_to_int(check.substr(6,5)), (binary_to_int(check.substr(11,5))));
		}
		
//----------------------------------------------------------------------------------------------------------
	return i;
		//print_for_simulator(check);
	
}

int stall = 0;
string waitinginstr="", executedinstr ="";
string preissuebuffer[4]={""};
string preALU1[2]={"",""};
string premem="";
string postmem="";
string preALU2[2]={"",""};
string postALU2="";
string executable="";
int doneornot[1000000] = {0};
int nonsense = 0;
// 0 for add etc, SLL,SRA,SRL;   1 for lw,sw ;    2 for J,JR,BEQ,BGTZ, BLTZ, BREAK, NOP


int typeofinstr = 0;// 0 for add etc, SLL,SRA,SRL;   1 for lw,sw ;    2 for J,JR,BEQ,BGTZ, BLTZ, BREAK, NOP
int checktype_instr(string s){
	if(!s.empty()){
	if(!(s.substr(37,3)).compare("BEQ") || !(s.substr(37,4)).compare("BGTZ") || !(s.substr(37,4)).compare("BLTZ") || !(s.substr(37,5)).compare("BREAK") || !(s.substr(37,2)).compare("JR") || !(s.substr(37,1)).compare("J") || !(s.substr(37,3)).compare("NOP")){
		typeofinstr = 2;
	}
	if(!(s.substr(37,2)).compare("LW") || !(s.substr(37,2)).compare("SW")){
		typeofinstr = 1;
	}
	else
		typeofinstr = 0;
	if(!(s.substr(37,3)).compare("BEQ") || !(s.substr(37,4)).compare("BGTZ") || !(s.substr(37,4)).compare("BLTZ") || !(s.substr(37,5)).compare("BREAK") || !(s.substr(37,2)).compare("JR") || !(s.substr(37,1)).compare("J") || !(s.substr(37,3)).compare("NOP")){
		typeofinstr = 2;
	}	
}
	return typeofinstr;	
}
int preissuecount = 0;
void IFunit(int i){
	//cout << "no - "<< instrarray[i]<<"  "<< checktype_instr(instrarray[i]) << "  "<< instrarray[i].substr(37,3)<<" \n";

	if(stall == 1 && executedinstr.compare("") ){
		executedinstr = "";
		stall = 0;
	}
	
	if(waitinginstr.compare("")){
		stall = 1;
	}
	if(preissuecount == 4)
		stall =1;
	if(preissuecount < 4 && !waitinginstr.compare("") && !executedinstr.compare(""))
		stall =0;	
		if(i>=-1){
			
	if(checktype_instr(instrarray[i]) == 2 && stall == 0){	
	
	waitinginstr = instrarray[i];
	doneornot[find_instrarray(waitinginstr)] = 1;
		stall = 1;

if(instrarray[i].compare("")){
	if((!(instrarray[i].substr(37,1)).compare("J")||!(instrarray[i].substr(37,5)).compare("BREAK")) && (instrarray[i].substr(37,2)).compare("JR")){
		waitinginstr ="";
		executedinstr = instrarray[i];		
		nonsense = simulator(executedinstr)+1;
		stall = 1;
		doneornot[find_instrarray(executedinstr)] = 1;
	}
	}
}
}

}

int allreg[4][3] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int ready[10000] = {1};
int regstatus[32] = {0};
int reguse[3] = {-1,-1,-1};
void whichreg(string check){
		reguse[0] = -1;
		reguse[1] = -1;
		reguse[2] = -1;
		if(!check.empty()){
		if(!(check.substr(37,3)).compare("BEQ") || !(check.substr(37,4)).compare("BGTZ") || !(check.substr(37,4)).compare("BLTZ") ){
				reguse[0] = binary_to_int(check.substr(6,5));
				if(!(check.substr(37,3)).compare("BEQ"))
				reguse[1] = binary_to_int(check.substr(11,5));
				else
				reguse[1] = 33;
				reguse[2] = 33;
				return;
		}	
	    if(!(check.substr(37,3)).compare("ORI") || !(check.substr(37,4)).compare("XORI") ||!(check.substr(37,4)).compare("ANDI") || !(check.substr(37,4)).compare("ADDI")) 
			{
				reguse[0] = binary_to_int(check.substr(11,5));
				reguse[1] = binary_to_int(check.substr(6,5));
				reguse[2] = binary_to_int(check.substr(6,5));
				return;
		}

		if(!(check.substr(37,3)).compare("SRA") || !(check.substr(37,3)).compare("SRL") || !(check.substr(37,3)).compare("SLL"))	{
				reguse[0] = binary_to_int(check.substr(16,5));
				reguse[1] = binary_to_int(check.substr(11,5));
				reguse[2] = binary_to_int(check.substr(11,5));
				return;
		}

		if(!(check.substr(37,2)).compare("LW") )	{
				reguse[0] = binary_to_int(check.substr(11,5));
				reguse[1] = binary_to_int(check.substr(6,5));
				reguse[2] = binary_to_int(check.substr(6,5));
				return;
		}

		if( !(check.substr(37,2)).compare("SW"))	{
				reguse[0] = 33;
				reguse[1] = binary_to_int(check.substr(6,5));
				reguse[2] = binary_to_int(check.substr(11,5));
				return;
		}
		if(!(check.substr(37,5)).compare("BREAK") || !(check.substr(37,1)).compare("J") ){
				reguse[0] = 33;
				reguse[1] = 33;
				reguse[2] = 33;
				return;
		}
		if(!(check.substr(37,2)).compare("JR") ){
				reguse[0] = binary_to_int(check.substr(6,5));;
				reguse[1] = binary_to_int(check.substr(6,5));
				reguse[2] =  binary_to_int(check.substr(6,5));
				return;
		}
		else{
				reguse[0] = binary_to_int(check.substr(16,5));
				reguse[1] = binary_to_int(check.substr(6,5));
				reguse[2] = binary_to_int(check.substr(11,5));	
				return;
		}	
		}
}

void checkforhazards(){
	string check = "";
	int y;
	for(y = 0; y<preissuecount; y++){
		check = preissuebuffer[y];
		if(!check.empty()){
	    if(!(check.substr(37,3)).compare("ORI") || !(check.substr(37,4)).compare("XORI") ||!(check.substr(37,4)).compare("ANDI") || !(check.substr(37,4)).compare("ADDI")) 
			{
				allreg[y][0] = binary_to_int(check.substr(11,5));
				allreg[y][1] = binary_to_int(check.substr(6,5));
				allreg[y][2] = binary_to_int(check.substr(6,5));
				regstatus[binary_to_int(check.substr(11,5))] += 1;
 
 				regstatus[binary_to_int(check.substr(6,5))] += 1;
				continue;
		}

		if(!(check.substr(37,3)).compare("SRA") || !(check.substr(37,3)).compare("SRL") || !(check.substr(37,3)).compare("SLL"))	{
				allreg[y][0] = binary_to_int(check.substr(16,5));
				allreg[y][1] = binary_to_int(check.substr(11,5));
				allreg[y][2] = binary_to_int(check.substr(11,5));
				regstatus[binary_to_int(check.substr(16,5))] += 1;
				regstatus[binary_to_int(check.substr(11,5))] += 1;
				continue;
		}

		if(!(check.substr(37,2)).compare("LW") )	{
				allreg[y][0] = binary_to_int(check.substr(11,5));
				allreg[y][1] = binary_to_int(check.substr(6,5));
				allreg[y][2] = binary_to_int(check.substr(6,5));
				regstatus[binary_to_int(check.substr(11,5))] += 1;
				//regstatus[binary_to_int(check.substr(6,5))] += 1;
				continue;
		}

		if( !(check.substr(37,2)).compare("SW"))	{
				allreg[y][0] = 33;
				allreg[y][1] = binary_to_int(check.substr(6,5));
				allreg[y][2] = binary_to_int(check.substr(11,5));
				//regstatus[binary_to_int(check.substr(11,5))] += 1;
				regstatus[binary_to_int(check.substr(6,5))] += 1;
				continue;
		}

		else{
				allreg[y][0] = binary_to_int(check.substr(16,5));
				allreg[y][1] = binary_to_int(check.substr(6,5));
				allreg[y][2] = binary_to_int(check.substr(11,5));	
				regstatus[binary_to_int(check.substr(16,5))] += 1;
				regstatus[binary_to_int(check.substr(6,5))] += 1;
				regstatus[binary_to_int(check.substr(11,5))] += 1;
				continue;
		}	
		}
	}
	for(y=0;y<preissuecount;y++){
		ready[find_instrarray(preissuebuffer[y])] = 1;	
	}

		for(y = 0; y < preissuecount; y++){

		for(int yy = y+1; yy < preissuecount ; yy++){
			if(allreg[y][0] == allreg[yy][0] )//WAW
			{
				ready[find_instrarray(preissuebuffer[yy])] = 0;	
				//cout<<"in waw and raw 1 =="<<allreg[y][0] <<" "<< ready[find_instrarray(preissuebuffer[y])]<<"  "<<allreg[yy][1]<<" "<< ready[find_instrarray(preissuebuffer[yy])]<<"\n"	;
			}
			if(allreg[y][0] == allreg[yy][1] || allreg[y][0] == allreg[yy][2]) {//RAW
				ready[find_instrarray(preissuebuffer[yy])] = 0;	
				//cout << "RAW here="<< preissuebuffer[y]<<" " << preissuebuffer[yy] <<"\n";
			}
			if(allreg[y][1] == allreg[yy][0] || allreg[y][2] == allreg[yy][0])//WAR
			{
			
				ready[find_instrarray(preissuebuffer[yy])] = 0;	
					//cout << "WAR in "<<preissuebuffer[yy]<<"  because of==="<<preissuebuffer[y]<<"\n";
			}
		}
		
				//cout << "ssssstatus = "<<ready[find_instrarray(preissuebuffer[y])]<<"-----\n";
	}
		for(y=0; y<preissuecount;y++){
		whichreg(preALU1[0]);
		if(allreg[y][0] == reguse[0] || allreg[y][1] == reguse[0] || allreg[y][2] == reguse[0] ){
			ready[find_instrarray(preissuebuffer[y])] = 0;
			//cout<<allreg[y][0] <<" "<< reguse[0] <<" "<< allreg[y][1]<<" " <<reguse[0] <<" "<< allreg[y][2] <<" "<< reguse[0] ;
			//cout <<"waw war in preissue="<<preALU1[0]<<";;  y="<< preissuebuffer[y]<<"\n";
		}
		whichreg(preALU1[1]);
		if(allreg[y][0] == reguse[0] || allreg[y][1] == reguse[0] || allreg[y][2] == reguse[0] ){
			ready[find_instrarray(preissuebuffer[y])] = 0;
			//cout <<"waw war in prealu1\n";
		}
		whichreg(preALU2[0]);
		if(allreg[y][0] == reguse[0] || allreg[y][1] == reguse[0] || allreg[y][2] == reguse[0] ){
			ready[find_instrarray(preissuebuffer[y])] = 0;
			//cout <<"waw war in prealu20\n";
		}
		whichreg(preALU2[1]);
		if(allreg[y][0] == reguse[0] || allreg[y][1] == reguse[0] || allreg[y][2] == reguse[0] ){
			ready[find_instrarray(preissuebuffer[y])] = 0;
			//cout <<"waw war in prealu21\n";
		}
		whichreg(premem);
		if(allreg[y][0] == reguse[0] || allreg[y][1] == reguse[0] || allreg[y][2] == reguse[0] ){
			ready[find_instrarray(preissuebuffer[y])] = 0;
			//cout <<"waw war in premem\n";
			
		}
		whichreg(postmem);
		if(allreg[y][0] == reguse[0] || allreg[y][1] == reguse[0] || allreg[y][2] == reguse[0] ){
			ready[find_instrarray(preissuebuffer[y])] = 0;
			//cout <<"waw war in postmem\n";
		}
		whichreg(postALU2);
		if(allreg[y][0] == reguse[0] || allreg[y][1] == reguse[0] || allreg[y][2] == reguse[0] ){
			ready[find_instrarray(preissuebuffer[y])] = 0;
			//cout <<"waw war in postalu2\n";
		}
		//cout << "status = "<<ready[find_instrarray(preissuebuffer[y])]<<"\n";
	}


}
void preissuepush(int& i){

	if(stall == 1)
		return;
	if(stall == 0 && checktype_instr(instrarray[i]) != 2) {
		if(doneornot[find_instrarray(instrarray[i])] == 0){
			doneornot[find_instrarray(instrarray[i])] = 1; 
			preissuebuffer[preissuecount++] = instrarray[i];
			//cout<<instrarray[i] <<"preisssuepush"<< checktype_instr(instrarray[i])<<"\n";
			//cout << premem << doneornot[find_instrarray(premem)]<<"\n";
		}
	}
	int k=find_instrarray(instrarray[++i]);
	IFunit(k);
	
	if(stall == 0 && checktype_instr(instrarray[i]) != 2){
		if(doneornot[find_instrarray(instrarray[i])] == 0){
			doneornot[find_instrarray(instrarray[i])] = 1; 
			preissuebuffer[preissuecount++] = instrarray[i];
			//cout<<instrarray[i] <<"preisssuepush"<< checktype_instr(instrarray[i])<<"\n";
			//cout << premem << doneornot[find_instrarray(premem)]<<"\n";
		}
	}
	
	checkforhazards();

}
void removei(int i){
	int q, u =0;
	string temp[4]={"","","",""};
	//cout << "i="<<i<<"u="<<u<<"\n";
	for(q = 0; u<preissuecount; q++){
		if(u == i)
			u++;	
		temp[q] = preissuebuffer[u];
		//cout <<"temp ="<<temp[q]<<"pre="<<preissuebuffer[u];
		u=u+1;
	}
	if(preissuecount > 0)
	preissuecount--;
	for(q=0; q<4;q++){
		preissuebuffer[q] = temp[q];
		//cout <<"removing" << preissuebuffer[q]<<"with"<<preissuecount;
	}

}

void writeback(string s){

	if(s.compare("")){
	if(doneornot[find_instrarray(s)] == 0){
	doneornot[find_instrarray(s)] = 1;
			
	simulator(s);
	return;
	}
}
	
}
void postmempop(){	
	//cout << "postmempop"<<postmem<<"--\n";
	if(postmem.compare("")){
		//cout << "postmempop"<<postmem <<doneornot[find_instrarray(postmem)] ;
		if(!(postmem.substr(37,2)).compare("LW")){
		if(doneornot[find_instrarray(postmem)] == 0){

			writeback(postmem);
			doneornot[find_instrarray(postmem)] = 1;
			postmem = "";
		}
	}

	}
	

}

void postALU2pop(){
	end = postALU2;
	if(postALU2.compare("")){
		if(doneornot[find_instrarray(postALU2)] == 0){
			//cout << "here";
			end = postALU2;
			doneornot[find_instrarray(end)] = 1;
			writeback(postALU2);
			doneornot[find_instrarray(postALU2)] = 1;
			postALU2 = "";
		}
	}
	
}
void preALU1pop(){
	if(preALU1[0].compare("")&& !premem.compare("")){
		
		if(doneornot[find_instrarray(preALU1[0])] == 0){
			//cout << "prealu0="<<preALU1[0]<<"premem="<<premem<<"\n"; 
			premem = preALU1[0];
			
			doneornot[find_instrarray(premem)] = 1;

			preALU1[0] = preALU1[1];
			doneornot[find_instrarray(preALU1[0])] = 0;
			preALU1[1] = "";
			//cout << "prealu0="<<preALU1[0]<<"premem="<<premem<<"\n";
		}
	else 
	{

		premem ="";
	}
}
}
void premempop(){
	//cout << "in premempop "<< premem<<" ++ "<<doneornot[find_instrarray(premem)]<<"||\n"; 
	if(premem.compare(""))
		if(!premem.substr(37,2).compare("LW")){
		if(doneornot[find_instrarray(premem)] == 0){
			postmem = premem;
			doneornot[find_instrarray(premem)] =1;
			premem = "";
		}
	}
	if(premem.compare("")){
			
		if(!premem.substr(37,2).compare("SW")){
		if(doneornot[find_instrarray(premem)] == 0){
			writeback(premem);
			doneornot[find_instrarray(premem)] =1;
			premem = "";
		}
	}
	}
}

void preALU2pop(){
	if(preALU2[0].compare("")){
		if(doneornot[find_instrarray(preALU2[0])] == 0){
			postALU2 = preALU2[0];
			doneornot[find_instrarray(preALU2[0])] = 1;
			preALU2[0] = preALU2[1];
			preALU2[1] = "";
			
		}
	}
}

void preALU1code(string s){
	if(!preALU1[0].compare("")){
		preALU1[0] = s;	
	}
	else
		preALU1[1] = s;
	doneornot[find_instrarray(s)] = 1;		
}	


void preALU2code(string s){
	if(preALU2[0].empty()){
		preALU2[0] = s;	
		doneornot[find_instrarray(s)] = 1;	
	}
	
	else if(preALU2[1].empty()){
		preALU2[1] = s;
	doneornot[find_instrarray(s)] = 1;	
	}

}

void intialisetozero(){
	for(int i =0; i< count; i++){
		doneornot[i] = 0;
	}
	
}
void preissuepop(){
	int ok1=0, ok2 =0;
	for(int i=0; i<4; i++){
		
		if(ready[find_instrarray(preissuebuffer[i])] == 1){
		if(preissuebuffer[i].compare("")){
		checktype_instr(preissuebuffer[i]); 
		if(typeofinstr == 1 && ok1 ==0){
			if(( !preALU1[0].compare("")|| !preALU1[1].compare("") )&& doneornot[find_instrarray(preissuebuffer[i])] == 0){
				preALU1code(preissuebuffer[i]);
				ok1=1;
				doneornot[find_instrarray(preissuebuffer[i])] = 1;
				//cout << "in rem  "<<preissuebuffer[i];
				removei(i);
	
			}
		}	
		else if(typeofinstr == 0 && ok2 ==0){
			if((!preALU2[0].compare("")|| !preALU2[1].compare(""))&& doneornot[find_instrarray(preissuebuffer[i])] == 0){
				preALU2code(preissuebuffer[i]);

				ok2=1;
				doneornot[find_instrarray(preissuebuffer[i])] = 1;
				removei(i);
	
			}
		}
		}
	}
	}
}
void postmempush(){

	if(!premem.compare("")){
	//cout << "in postmempush"<<postmem<<"premem ="<<premem;
	if(doneornot[find_instrarray(preALU1[0])] == 0){
		premem = preALU1[0];
		
		if(preALU1[1].compare("")){
			preALU1[0] = preALU1[1];
			preALU1[1] = "";
		}
		else{
			preALU1[0] = "";
			preissuepop();
		}
		doneornot[find_instrarray(premem)] = 1;
	} 		
	}
}
void laststep(){
	en =0;
	if(waitinginstr.compare("")){
	if(!(waitinginstr.substr(37,5)).compare("BREAK") ) {
		executedinstr = waitinginstr;

		waitinginstr = "";
			
		if(executedinstr.compare("")){
					
			nonsense = simulator(executedinstr);
			doneornot[find_instrarray(executedinstr)] = 1;
		}
	stall = 0; 
	
	return;
	}
}
	whichreg(waitinginstr);
	int r1 = reguse[0], ir1 =33,ir2 =33,ir3 =33;
	int r2 = reguse[1];
	for(int i = 0; i<preissuecount;i++){
		//cout << preissuebuffer[i];
		whichreg(preissuebuffer[i]);
		ir1 = reguse[0];
		ir2 = reguse[1];
		ir3 = reguse[2];
		//cout <<"reg ="<<r1<<" all reg = "<< ir1<<" "<<ir2<<" "<<ir3<<"\n";
		if(r1 == ir1|| r1 == ir2 || r1 == ir3 || r2 == ir1|| r2 == ir2 || r2 == ir3){
			en =1;
		}

	}
	whichreg(preALU1[0]);
	ir1 = reguse[0];
	ir2 = reguse[1];
	ir3 = reguse[2];
		//cout <<"reg ="<<r1<<" all reg = "<< ir1<<" "<<ir2<<" "<<ir3<<"\n";
	if(r1 == ir1|| r1 == ir2 || r1 == ir3 || r2 == ir1|| r2 == ir2 || r2 == ir3){
		en =1;
	}
	whichreg(preALU1[1]);
	ir1 = reguse[0];
	ir2 = reguse[1];
	ir3 = reguse[2];
		//cout <<"reg ="<<r1<<" all reg = "<< ir1<<" "<<ir2<<" "<<ir3<<"\n";
	if(r1 == ir1|| r1 == ir2 || r1 == ir3 || r2 == ir1|| r2 == ir2 || r2 == ir3){
		en =1;
	}
	whichreg(preALU2[0]);
	ir1 = reguse[0];
	ir2 = reguse[1];
	ir3 = reguse[2];
	//cout<<preALU2[0] <<"reg ="<<r1<<" all reg = "<< ir1<<" "<<ir2<<" "<<ir3<<"\n";
	if(r1 == ir1|| r1 == ir2 || r1 == ir3 || r2 == ir1|| r2 == ir2 || r2 == ir3){
		en =1;
	}
	whichreg(preALU2[1]);
	ir1 = reguse[0];
	ir2 = reguse[1];
	ir3 = reguse[2];
		//cout <<"reg ="<<r1<<" all reg = "<< ir1<<" "<<ir2<<" "<<ir3<<"\n";
	if(r1 == ir1|| r1 == ir2 || r1 == ir3 || r2 == ir1|| r2 == ir2 || r2 == ir3){
		en =1;
	}

	whichreg(premem);
	ir1 = reguse[0];
	ir2 = reguse[1];
	ir3 = reguse[2];
		//cout <<"reg ="<<r1<<" all reg = "<< ir1<<" "<<ir2<<" "<<ir3<<"\n";
	if(r1 == ir1|| r1 == ir2 || r1 == ir3 || r2 == ir1|| r2 == ir2 || r2 == ir3){
		en =1;
	}
	whichreg(postmem);
	ir1 = reguse[0];
	ir2 = reguse[1];
	ir3 = reguse[2];
		//cout <<"reg ="<<r1<<" all reg = "<< ir1<<" "<<ir2<<" "<<ir3<<"\n";
	if(r1 == ir1|| r1 == ir2 || r1 == ir3 || r2 == ir1|| r2 == ir2 || r2 == ir3){
		en =1;
	}
	whichreg(postALU2);
	ir1 = reguse[0];
	ir2 = reguse[1];
	ir3 = reguse[2];
	//cout<<postALU2 <<"reg ="<<r1<<" all reg = "<< ir1<<" "<<ir2<<" "<<ir3<<"\n";
	if(r1 == ir1|| r1 == ir2 || r1 == ir3 || r2 == ir1|| r2 == ir2 || r2 == ir3){
		en =1;
	}
	if(r1 == -1){
		en = 0;
	}
	if(waitinginstr.compare("")){
	if((en == 0  && doneornot[find_instrarray(waitinginstr)] == 0) || !(waitinginstr.substr(37,5)).compare("BREAK") ) {
		executedinstr = waitinginstr;

		waitinginstr = "";
			
		if(executedinstr.compare("")){
				
			nonsense = simulator(executedinstr);
			doneornot[find_instrarray(executedinstr)] = 1;
		}
	stall = 0; 
	
	return;
	}
}
}
string substringtell(string s){
	
	if(!s.compare(""))
		return "";
	else{

		 string po = "[" + s.substr(37,s.length()-1-37) + "]";
		 //cout << po<<"\n";
		 return po;
}
		
}
int checkifempty(){
	if(preissuecount < 4)
	{
		return 1;
	}
	return 0;
}
bool checkbreak(){
	if(!executedinstr.compare(""))
	return true;
	else if( !(executedinstr.substr(37,5)).compare("BREAK") ){
		return false;
	}
	return true;
}
void checkforsw(){
	if(premem.compare("")){
		if(!preissuebuffer[0].compare(premem)){
			doneornot[find_instrarray(premem)] = 0;
		}
		if(!preissuebuffer[1].compare(premem)){
			doneornot[find_instrarray(premem)] = 0;
		}
		if(!preissuebuffer[2].compare(premem)){
			doneornot[find_instrarray(premem)] = 0;
		}
		if(!preissuebuffer[3].compare(premem)){
			doneornot[find_instrarray(premem)] = 0;
		}
		if(!preALU1[0].compare(premem)){
			doneornot[find_instrarray(premem)] = 0;
		}
		if(!preALU1[1].compare(premem)){
			doneornot[find_instrarray(premem)] = 0;
		}
		if(premem.compare("")){
				//cout <<doneornot[find_instrarray(premem)] << "++ "<< (premem.substr(37,2));
		if(!(premem.substr(37,2)).compare("SW")){
		if(doneornot[find_instrarray(premem)] == 0){
			writeback(premem);
			doneornot[find_instrarray(premem)] =1;
			premem = "";
		}
	}
	}
	else return ;
}
}
void attheend(){
	if(postALU2.compare("") && doneornot[find_instrarray(postALU2)]==0){
		writeback(postALU2);
		doneornot[find_instrarray(postALU2)]= 1;
		postALU2 = "";
		return;
	}
	if(end.compare("")){
		writeback(end);
		end ="";
	}
}
string latest = instrarray[0];
void simulate(){ 
	string q = "";	
	int als= 0;
	fstream sim;
	sim.open("simulation.txt",ios_base::out);
    if(count > 0){
	while(checkbreak()){
	
		intialisetozero();

		if(postmem.compare("")){
			//cout <<"2.1-"<<nonsense<<"\n";
			writeback(postmem);
			postmem = "";
		}
		if(stall == 0){
			executedinstr = "";
		}	
		IFunit(nonsense);
		if(waitinginstr.compare("")){
			stall =1;
		}
		//cout << "status =" << doneornot[find_instrarray(premem)]<<"\n";
		
		if(stall == 0){
			preissuepush(nonsense);
		}
		//cout << "status =" << doneornot[find_instrarray(premem)];
		preissuepop();
		preALU1pop();
		checkforsw();
		premempop();
		postmempop();
		postmempush();
		preALU2pop();
		laststep();
		attheend();
		postALU2pop();
		checkforhazards();
		//cout <<"4-"<<nonsense<<"\n";
		
		sim <<"--------------------\n";
		sim<< "Cycle:" << ++als<<"\n";
		sim<< "\nIF Unit:\n";
		sim<< "\tWaiting Instruction:" << substringtell(waitinginstr) <<"\n\tExecuted Instruction: " << substringtell(executedinstr)<<"\n";
		sim<< "Pre-Issue Queue:\n";
		sim<< "\tEntry 0: " <<substringtell(preissuebuffer[0])<<"\n\tEntry 1: "<<substringtell(preissuebuffer[1])<<"\n\tEntry 2: "<<substringtell(preissuebuffer[2])<<"\n\tEntry 3: "<<substringtell(preissuebuffer[3])<<"\n";
		sim<< "Pre-ALU1 Queue:\n";
		sim<< "\tEntry 0: "<< substringtell(preALU1[0])<<"\n\tEntry 1: "<< substringtell(preALU1[1])<<"\n";
		sim<< "Pre-MEM Queue: ";
		sim<< substringtell(premem)<<"\n";
		sim<< "Post-MEM Queue: ";
		sim<< substringtell(postmem)<<"\n";
		sim<< "Pre-ALU2 Queue: \n";
		sim<< "\tEntry 0: "<< substringtell(preALU2[0])<<"\n\tEntry 1: "<< substringtell(preALU2[1])<<"\n";
		sim<< "Post-ALU2 Queue: ";
		sim<< substringtell(postALU2)<<"\n\n";
		print_for_simulator(sim);

	if(checkifempty() == 1 && stall ==0)
	nonsense++;
	

	}
}
}


int main(int argc, char* argv[])
{
	fstream	input;
	string data = "";
	//input.open("sampl.txt",ios_base::in | ios_base::out | ios_base::app);
	input.open(argv[1],ios_base::in | ios_base::out | ios_base::app);
	while(!input.eof()){
	input >> data;	
	if(!input.eof())
	dissassembler(data);
	}

	simulate();
    return 0;
}
