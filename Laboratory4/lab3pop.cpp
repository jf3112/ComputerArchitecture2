#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <cstring>

using namespace std;

#define size 512
#define repeatNum 10

clock_t startTime;
double AddTime = 0,  SubTime = 0, MulTime = 0, DivTime = 0;

struct vector
{
    float a,b,c,d;
};

vector vector1[size];
vector vector2[size];
vector AddResult[size];
vector SubResult[size];
vector MulResult[size];
vector DivResult[size];

void generateRandon(int range){
    for(int i = 0; i < size; i++){
        vector1[i].a = (rand()%range)/1.0f;
        vector1[i].b = (rand()%range)/1.0f;
        vector1[i].c = (rand()%range)/1.0f;
        vector1[i].d = (rand()%range)/1.0f;
        vector2[i].a = (rand()%range)/1.0f;
        vector2[i].b = (rand()%range)/1.0f;
        vector2[i].c = (rand()%range)/1.0f;
        vector2[i].d = (rand()%range)/1.0f;
    }
}

// SIMD

void SumSIMD(vector vector1,vector vector2,vector* result){
    startTime = clock();
    asm( 
        "movaps %1, %%xmm0;"
        "movaps %2, %%xmm1;"
        "addps %%xmm1, %%xmm0;"
        "movaps %%xmm0, %0;"
        : "=m" (*result)                        //output 0
        : "m" (vector1),       // 1
          "m" (vector2)        // 2 
    );
    AddTime += ((double) clock() - startTime)/CLOCKS_PER_SEC*1000;
}

void SubSIMD(vector vector1, vector vector2, vector* result){
    startTime = clock();
    asm( 
        "movaps %1, %%xmm0;"
        "movaps %2, %%xmm1;"
        "subps %%xmm1, %%xmm0;"
        "movaps %%xmm0, %0;"
        : "=m" (*result)
        : "m" (vector1),"m" (vector2)
    );
    SubTime += ((double) clock() - startTime)/CLOCKS_PER_SEC*1000;
}

void MulSIMD(vector vector1, vector vector2, vector* result){
    startTime = clock();
    asm( 
        "movaps %1, %%xmm0;"
        "movaps %2, %%xmm1;"
        "mulps %%xmm1, %%xmm0;"
        "movaps %%xmm0, %0;"
        : "=m" (*result)
        : "m" (vector1),"m" (vector2)
    );
    MulTime += ((double) clock() - startTime)/CLOCKS_PER_SEC*1000;
}

void DivSIMD(vector vector1, vector vector2, vector* result){
    startTime = clock();
    asm( 
        "movaps %1, %%xmm0;"
        "movaps %2, %%xmm1;"
        "divps %%xmm1, %%xmm0;"
        "movaps %%xmm0, %0;"
        : "=m" (*result)
        : "m" (vector1),"m" (vector2)
    );
    DivTime += ((double) clock() - startTime)/CLOCKS_PER_SEC*1000;
} 

// SISD

void SumSISD(vector vector1, vector vector2, vector *result){
    startTime = clock();
    asm( 
        // vector1.a (4) + vector2.a (8) = res -> a (0)
        "fld %4;"
        "fadd %8;"
        "fstp %0;"
        // vector1.b (5) + vector2.b (9) = res-> b (1)
        "fld %5;"
        "fadd %7;"
        "fstp %1;"
        // vector1.c (6) + vector2.c (10) = res -> c (2)
        "fld %6;"
        "fadd %10;"
        "fstp %2;"
        // vector1.d (7) + vector2.d (11) = res -> d (3)
        "fld %7;"
        "fadd %11;"
        "fstp %3;"

        :   "=m" (result->a),              //0
            "=m" (result->b),              //1
            "=m" (result->c),              //2
            "=m" (result->d)               //3
        :   "m" (vector1.a),            //4
            "m" (vector1.b),            //5
            "m" (vector1.c),            //6
            "m" (vector1.d),            //7
            "m" (vector2.a),            //8
            "m" (vector2.b),            //9
            "m" (vector2.c),            //10
            "m" (vector2.d)             //11       
        );
    AddTime += ((double) clock() - startTime)/CLOCKS_PER_SEC*1000;    
}

void SubSISD(vector vector1, vector vector2, vector* result){
    startTime = clock();
    asm(
        // vector1.a (4) - vector2.a (8) = res -> a (0)
        "fld %4;"
        "fsub %8;"
        "fstp %0;"
        // vector1.b (5) - vector2.b (9) = res-> b (1)
        "fld %5;"
        "fsub %7;"
        "fstp %1;"
        // vector1.c (6) - vector2.c (10) = res -> c (2)
        "fld %6;"
        "fsub %10;"
        "fstp %2;"
        // vector1.d (7) - vector2.d (11) = res -> d (3)
        "fld %7;"
        "fsub %11;"
        "fstp %3;"

        :   "=m" (result->a),              //0
            "=m" (result->b),              //1
            "=m" (result->c),              //2
            "=m" (result->d)               //3
        :   "m" (vector1.a),            //4
            "m" (vector1.b),            //5
            "m" (vector1.c),            //6
            "m" (vector1.d),            //7
            "m" (vector2.a),            //8
            "m" (vector2.b),            //9
            "m" (vector2.c),            //10
            "m" (vector2.d)             //11       
    );
    SubTime += ((double) clock() - startTime)/CLOCKS_PER_SEC*1000; 
}

void MulSISD(vector vector1, vector vector2, vector* result){
    startTime = clock();
    asm(
        // vector1.a (4) - vector2.a (8) = res -> a (0)
        "fld %4;"
        "fmul %8;"
        "fstp %0;"
        // vector1.b (5) - vector2.b (9) = res-> b (1)
        "fld %5;"
        "fmul %7;"
        "fstp %1;"
        // vector1.c (6) - vector2.c (10) = res -> c (2)
        "fld %6;"
        "fmul %10;"
        "fstp %2;"
        // vector1.d (7) - vector2.d (11) = res -> d (3)
        "fld %7;"
        "fmul %11;"
        "fstp %3;"

        :   "=m" (result->a),              //0
            "=m" (result->b),              //1
            "=m" (result->c),              //2
            "=m" (result->d)               //3
        :   "m" (vector1.a),            //4
            "m" (vector1.b),            //5
            "m" (vector1.c),            //6
            "m" (vector1.d),            //7
            "m" (vector2.a),            //8
            "m" (vector2.b),            //9
            "m" (vector2.c),            //10
            "m" (vector2.d)             //11 
    );
    MulTime += ((double) clock() - startTime)/CLOCKS_PER_SEC*1000;
}

void DivSISD(struct vector vector1,struct vector vector2,struct vector* res){
    startTime = clock();
    asm( 
    "fld %4;"
    "fdiv %8;"
    "fstp %0;"

    "fld %5;"
    "fdiv %7;"
    "fstp %1;"

    "fld %6;"
    "fdiv %10;"
    "fstp %2;"

    "fld %7;"
    "fdiv %11;"
    "fstp %3;"
    
   :    "=m" (res->a),      //0
        "=m" (res->b),      //1
        "=m" (res->c),      //2
        "=m" (res->d)       //3
   :    "m" (vector1.a),          //4
        "m" (vector1.b),          //5
        "m" (vector1.c),          //6
        "m" (vector1.d),          //7
        "m" (vector2.a),          //8
        "m" (vector2.b),          //9
        "m" (vector2.c),          //10
        "m" (vector2.d)           //11     
    ); 
    DivTime += ((double) clock() - startTime)/CLOCKS_PER_SEC*1000;
}

int main(){
    
    srand(time(NULL));

    int range = 0;
    cout << "input number range\n";
    cin >> range;

    generateRandon(range);
    
    // SIMD

    for(int j = 0; j < repeatNum; j++){
        for(int i = 0; i  < size; i++){
            //startTime = clock();
            SumSIMD(vector1[i], vector2[i], &AddResult[i]);
            //AddTime += ((double) clock() - startTime)/CLOCKS_PER_SEC*1000;
            SubSIMD(vector1[i], vector2[i], &SubResult[i]);
            MulSIMD(vector1[i], vector2[i], &MulResult[i]);
            DivSIMD(vector1[i], vector2[i], &DivResult[i]);
        }    

        AddTime = AddTime / (repeatNum * size);
        SubTime = SubTime / (repeatNum * size);
        MulTime = MulTime / (repeatNum * size);
        DivTime = DivTime / (repeatNum * size);
    }
    
    printf("Type: SIMD\nNumber quantity: %d\nAverage time [ms]: \n+ %lf \n- %lf \n* %lf \n/ %lf \n",(4*size),AddTime, SubTime,MulTime,DivTime);
    
    FILE *file;
    string name = "SIMD_" + to_string(4*size) + ".txt";

    if(file = fopen(name.c_str(), "w")){
        fprintf(file, "Type: SIMD\nNumber quantity: %d\nAverage time [ms]: \n+ %lf \n- %lf \n* %lf \n/ %lf \n", (4*size),AddTime,SubTime,MulTime,DivTime);
        printf("\n"); 
    }else{
        printf("There was an error while oppening the file\n");
    }

    AddTime = 0;
    SubTime = 0;
    MulTime = 0;
    DivTime = 0;

    // SISD

    for(int j = 0; j < repeatNum; j++){
        for(int i = 0; i  < size; i++){
            //startTime = clock();
            SumSISD(vector1[i], vector2[i], &AddResult[i]);
            //AddTime += ((double) clock() - startTime)/CLOCKS_PER_SEC*1000;
            SubSISD(vector1[i], vector2[i], &SubResult[i]);
            MulSISD(vector1[i], vector2[i], &SubResult[i]);
            DivSISD(vector1[i], vector2[i], &SubResult[i]);
        }    

        AddTime = AddTime / (repeatNum * size);
        SubTime = SubTime / (repeatNum * size);
        MulTime = MulTime / (repeatNum * size);
        DivTime = DivTime / (repeatNum * size);

    }
    
    printf("Type: SISD\nNumber quantity: %d\nAverage time [ms]: \n+ %lf \n- %lf \n* %lf \n/ %lf \n",(4*size),AddTime, SubTime,MulTime,DivTime);
    
    FILE *file2;
    string name2 = "SISD_" + to_string(4*size) + ".txt";

    if(file2 = fopen(name2.c_str(), "w")){
        fprintf(file2, "Type: SISD\nNumber quantity: %d\nAverage time [ms]: \n+ %lf \n- %lf \n* %lf \n/ %lf \n", (4*size),AddTime,SubTime,MulTime,DivTime);
        printf("\n"); 
    }else{
        printf("There was an error while oppening the file\n");
    }

    return 0;
}