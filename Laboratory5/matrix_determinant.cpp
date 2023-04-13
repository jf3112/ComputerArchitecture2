#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct Vector
{
    float x0,x1,x2,x3;
};


float findDeterminant(Vector vector0, Vector vector1, Vector vector2, Vector vector3){
    Vector determinant;

    asm(
        "movups %1, %%xmm0\n\t"
        "movups %2, %%xmm1\n\t"
        "shufps $0x48, %%xmm0, %%xmm0\n\t"       
        "shufps $0xB1, %%xmm1, %%xmm1\n\t"       
        "mulps %%xmm1, %%xmm0\n\t" 
        "movups %1, %%xmm1\n\t"
        "movups %2, %%xmm2\n\t"
        "shufps $0xB1, %%xmm1, %%xmm1\n\t"        
        "shufps $0x48, %%xmm2, %%xmm2\n\t"       
        "mulps %%xmm2, %%xmm1\n\t" 
        "subps %%xmm0, %%xmm1\n\t" 
        "movups %3, %%xmm0\n\t"
        "movups %4, %%xmm2\n\t"
        "shufps $0x16, %%xmm0, %%xmm0\n\t"        
        "shufps $0xEF, %%xmm2, %%xmm2\n\t"        
        "mulps %%xmm2, %%xmm0\n\t" 
        "movups %3, %%xmm2\n\t"
        "movups %4, %%xmm3\n\t"
        "shufps $0x16, %%xmm2, %%xmm2\n\t"        
        "shufps $0xEF, %%xmm3, %%xmm3\n\t"        
        "mulps %%xmm3, %%xmm2\n\t" 
        "subps %%xmm0, %%xmm2\n\t" 
        "mulps %%xmm2, %%xmm1\n\t" 
        "movups %1, %%xmm0\n\t"
        "movups %2, %%xmm2\n\t"
        "shufps $0x0E, %%xmm0, %%xmm0\n\t"               
        "shufps $0x07, %%xmm2, %%xmm2\n\t"               
        "mulps %%xmm2, %%xmm0\n\t" 
        "movups %1, %%xmm2\n\t"
        "movups %2, %%xmm3\n\t"
        "shufps $0x07, %%xmm2, %%xmm2\n\t"               
        "shufps $0x0E, %%xmm3, %%xmm3\n\t"               
        "mulps %%xmm3, %%xmm2\n\t" 
        "subps %%xmm0, %%xmm2\n\t" 
        "movups %3, %%xmm0\n\t"
        "movups %4, %%xmm3\n\t"
        "shufps $0b00000000, %%xmm0, %%xmm0\n\t" 
        "shufps $0b00011000, %%xmm3, %%xmm3\n\t" 
        "mulps %%xmm3, %%xmm0\n\t" 
        "movups %3, %%xmm3\n\t"
        "movups %4, %%xmm4\n\t"
        "shufps $0b00011000, %%xmm3, %%xmm3\n\t" 
        "shufps $0b00000000, %%xmm4, %%xmm4\n\t" 
        "mulps %%xmm4, %%xmm3\n\t" 
        "subps %%xmm0, %%xmm3\n\t" 
        "mulps %%xmm3, %%xmm2\n\t" 
        "addps %%xmm2, %%xmm1\n\t"
        "movups %%xmm1, %0\n\t"  
        :"=m" (determinant)                     
	    :"m"(vector0),"m"(vector1),"m"(vector2),"m"(vector3)
        ); 

    return determinant.x0 + determinant.x1 + determinant.x2 + determinant.x3;
}

int main(){
    
    Vector matrix1[4];

    for(int i = 0; i < 4; i++){
        cout << "input numbers "<< i+1 << " matrix row: "<< endl;
        cin >> matrix1[i].x0;
        cin >> matrix1[i].x1;
        cin >> matrix1[i].x2;
        cin >> matrix1[i].x3;
        cout << endl;
    }

    for (int i = 0; i < 4; i++){
        cout << matrix1[i].x0 << " " << matrix1[i].x1 << " " << matrix1[i].x2 << " " << matrix1[i].x3 << endl;
    }
    
    cout << "Matrix determinant = " << findDeterminant(matrix1[0],matrix1[1],matrix1[2],matrix1[3]) << endl; 
    
    return 0;
}


