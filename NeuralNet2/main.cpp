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
#define num_tri 1000000

double sigmoid(int i,double sum[num]);
double energy(void);
double SimultaneousEqu(double x[num]);
double ConstantValue(void);
double ObtaineTheta(int n,double c);
double ObtaineWeight(int n,int m,double theta_n,double theta_m,double c);
void initialization(void);
void CountState(double x[num]);
void Probability(void);

double a[num][num] = 
{ 
    {-2.0,1.0,1.0,1.0,-1.0},
    {-3.0,2.0,0.0,1.0,2.0,},
    {1.0,-1.0,2.0,0.0,-1.0},
    {1.0,0.0,3.0,-1.0,-1.0}
};
double theta[num]; //閾値
double w[num][num];//重み
double x[num]; //後で変える//最初はダミーニューロン
double y[num];
double sum[num]; //重み付け総和
double Pr_x[16]; //出現確率

int main(void){
    static double sum[num];
    static double E; //エネルギー関数
    static double const_value;
    int i,j,k;

    const_value = ConstantValue();
    
    for(i=0;i<num;i++){
        
        for(j=0;j<num;j++){
            if(i == j){
                w[i][j] = 0.0;
            }
            else if(i == 0){
                w[i][j] = -ObtaineTheta(j,const_value);
            }
            else if(j == 0){
                w[i][j] = -ObtaineTheta(i,const_value);
            }
            else{
                w[i][j] = ObtaineWeight(i, j, ObtaineTheta(i,const_value), ObtaineTheta(j,const_value), const_value);
            }
            //printf("w[%d][%d]:%lf\n",i,j,w[i][j]);
        }
    }
    initialization();//xの初期値を与える
    for(k=0;k<num;k++){
        if(k != 0){
            //printf("x%d:%lf",k,x[k]);
            //printf(",");
            if(k == (num - 1)){
                printf("\n");
            }
        }
    }
    
    for(i=0;i<num_tri;i++){

        for(j=0;j<num;j++){
            if(j != 0){
                for(k=0;k<num;k++){
                    x[0] = 1.0;
                    sum[j] += w[k][j] * x[k];
                }
                //printf("sum[%d]:%lf\n",j,sum[j]);
                //xの値を更新
                if(rand() < RAND_MAX * sigmoid(j,sum)){
                    x[j] = 1.0;
                }
                else{
                    x[j] = 0.0;
                }
                /*//デバッグ用↓
                for(k=0;k<num;k++){
                    if(k != 0){ 
                        printf("x%d:%lf",k,x[k]);
                        printf(",");
                        if(k == (num - 1)){
                            printf("\n");
                        }
                    }
                }*/
                CountState(x);
                //エネルギーの評価
                E = energy();
                //printf("E:%lf\n",-0.5*E);
                
            }
            sum[j] = 0.0;
        }
    }
    Probability();
}

double sigmoid(int i,double sum[]){
    double p;
    double a = 1.5;
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
    printf("%lf\n",C);
    return C;
}

double SimultaneousEqu(double x[num]){
    double E = 0.0;
    double sum = 0.0;
    int i,j;
    
    for(i=0;i<num-1;i++){
        
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

