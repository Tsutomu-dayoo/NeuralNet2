//
//  SimultaneousEqu.cpp
//  NeuralNet2
//
//  Created by 山口勉 on 2018/10/02.
//  Copyright © 2018年 山口勉. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define num 4 + 1
#define num_tri 100000

double sigmoid(int i,double sum[num]);
double energy(void);
double SimultaneousEqu(double x[num]);
double ConstantValue(void);
double ObtaineTheta(int n,double c);
double ObtaineWeight(int n,int m,double theta_n,double theta_m,double c);
void initialization(void);
void CountState(double x[num]);
void Probability(void);

double a[num][num];
double theta[num] = {0.0,-0.5,-0.5,-0.5,-0.5}; //閾値
double w[num][num] = 
{ {0.0,theta[1],theta[2],theta[3],theta[4]},
  {theta[1],0.0,10.0,0.0,-20.0},
  {theta[2],10.0,0.0,0.0,12.0},
  {theta[3],0.0,-15.0,0.0,31.0},
  {theta[4],19.0,0.0,-40.0,0.0} };//重み
double x[num]; //後で変える//最初はダミーニューロン
double y[num];
double sum[num]; //重み付け総和
double Pr_x[16]; //出現確率

int main(void){
    static double sum[num];
    static double E; //エネルギー関数
    static double const_value;
    int i,j,k;

    for(i=0;i<16;i++){
        Pr_x[i] = 0.0; //Pr_xの初期化
    }
    initialization(); //xの初期値決定

    for(i=0;i<num_tri;i++){

        for(j=0;j<num;j++){

            if(j != 0){
                for(k=0;k<num;k++){
                    sum[j] += w[k][j] * x[k];
                }   
                if(rand() < RAND_MAX * sigmoid(j,sum)){
                    x[j] = 1.0;
                }
                else{
                    x[j] = 0.0;
                }
                CountState(x);
                //E = energy();
                //printf("%lf\n",-0.5*E);
                sum[j] = 0.0;
                //printf("aaa\n");
            }
        }
    }
    Probability();
    
}

double sigmoid(int i,double sum[]){
    double p;
    double a = 100;
    p = 1 / (1 + exp(-a * sum[i]));
    
    return p;
}

double energy(void){
    double e = 0.0;
    int i,j;
    
    for(i=0;i<num;i++){
        for(j=0;j<num;j++){
            e += w[i][j] * x[i] * x[j];
        }
    }
    return e;
}
//n,m番目の重みを求める、必要な値は予め求めておく
double ObtaineWeight(int n,int m,double theta_n,double theta_m,double c){
    int i;
    double E_nm = 0.0;
    
    for(i=0;i<num;i++){
        if(i == 0 || i == n || i == m){
            x[i] = 1.0;
        }
        else{
            x[i] = 0.0;
        }
    }
    E_nm = -SimultaneousEqu(x) + theta_n + theta_m + c;
    return E_nm;
}

//n番目のthetaを求める、cは定数の値を予め求める
double ObtaineTheta(int n,double c){
    int i;
    double E_n = 0.0;
    
    for(i=0;i<num;i++){
        if(i == 0 || i == n){
            x[i] = 1.0;
        }
        else{
            x[i] = 0.0;
        }
    }
    E_n = SimultaneousEqu(x) - c;
    return E_n;
}
//定数を求める
double ConstantValue(void){
    int i;
    double C = 0.0;
    
    for(i=0;i<num;i++){
        if(i == 0){
            x[i] = 1.0;
        }
        else{
            x[i] = 0.0;
        }
    }
    C = SimultaneousEqu(x);
    return C;
}

double SimultaneousEqu(double x[num]){
    double E = 0.0;
    double sum = 0.0;
    int i,j;
    
    for(i=0;i<num;i++){
        
        for(j=0;j<num;j++){
            sum += a[i][j] * x[j];
        }
        E += sum * sum;
        sum = 0.0;
    }
    return E;
}

void initialization(void){
    double ini_x[num] = {1.0,0.0,0.0,0.0,0.0};//最初の１はダミーニューロン
    int i;
    
    //srand((unsigned)time(NULL));//0と1を適当に決めるための乱数
    for(i=0;i<num;i++){
        x[i] = ini_x[i];
    }
    
}

void CountState(double x[num]){
    char str[100];
    int i;

    for(i=0;i<num;i++){
        if(i !=0 ){
            if(x[i] == 1.0){
                str[i-1] = '1';
            }
            else if(x[i] == 0.0){
                str[i-1] = '0';
            }
            else{
                str[i-1] = 'n';
            }
        }
    }
    //printf("%s\n",str);

    if(!strcmp(str,"0000")){
        Pr_x[0] += 1.0; 
    }
    else if(!strcmp(str,"0001")){
        Pr_x[1] += 1.0; 
    }
    else if(!strcmp(str,"0010")){
        Pr_x[2] += 1.0; 
    }
    else if(!strcmp(str,"0011")){
        Pr_x[3] += 1.0; 
    }
    else if(!strcmp(str,"0100")){
        Pr_x[4] += 1.0; 
    }
    else if(!strcmp(str,"0101")){
        Pr_x[5] += 1.0; 
    }
    else if(!strcmp(str,"0110")){
        Pr_x[6] += 1.0; 
    }
    else if(!strcmp(str,"0111")){
        Pr_x[7] += 1.0; 
    }
    else if(!strcmp(str,"1000")){
        Pr_x[8] += 1.0; 
    }
    else if(!strcmp(str,"1001")){
        Pr_x[9] += 1.0; 
    }
    else if(!strcmp(str,"1010")){
        Pr_x[10] += 1.0; 
    }
    else if(!strcmp(str,"1011")){
        Pr_x[11] += 1.0; 
    }
    else if(!strcmp(str,"1100")){
        Pr_x[12] += 1.0; 
    }
    else if(!strcmp(str,"1101")){
        Pr_x[13] += 1.0; 
    }
    else if(!strcmp(str,"1110")){
        Pr_x[14] += 1.0; 
    }
    else if(!strcmp(str,"1111")){
        Pr_x[15] += 1.0; 
    }
}

void Probability(void){
    int i;

    for(i=0;i<16;i++){
        printf("Pr_x[%d] : %lf \n",i,Pr_x[i] / ((num-1) * num_tri));
    }
}

