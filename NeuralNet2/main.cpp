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

#define num 5 + 1

double sigmoid(int i,double sum[num]);
double energy(void);
double SimultaneousEqu(double x[num]);
double ConstantValue(void);
double ObtaineTheta(int n,double c);
double ObtaineWeight(int n,int m,double theta_n,double theta_m,double c);
void initialization(void);

double a[num][num] =
{ {0.0,1.0,-1.0,-1.0,1.0,-2.0},
    {-3.0,2.0,0.0,1.0,-1.0,2.0},
    {-1.0,-1.0,2.0,0.0,-1.0,1.0},
    {1.0,0.0,-2.0,1.0,-1.0,0.0,},
    {-3.0,1.0,-1.0,2.0,-1.0,1.0,},
    {0.0,0.0,0.0,0.0,0.0,0.0}
};
double w[num][num];//重み
double x[num]; //後で変える//最初はダミーニューロン
double y[num];
double theta[num];

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
            printf("w[%d][%d]:%lf\n",i,j,w[i][j]);
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
    
    for(i=0;i<100;i++){
        for(j=0;j<num;j++){
            if(j != 0){
                for(k=0;k<num;k++){
                    x[0] = 1.0;
                    sum[j] += w[k][j] * x[k];
                }
                //printf("sum[%d]:%lf\n",j,sum[j]);
                //xの値を更新
                if(sum[j]>=0){
                    x[j] = 1.0;
                }
                else{
                    x[j] = 0.0;
                }
                for(k=0;k<num;k++){
                    if(k != 0){
                        /*
                         printf("x%d:%lf",k,x[k]);
                         printf(",");
                         if(k == (num - 1)){
                         printf("\n");
                         }*/
                    }
                }
                //エネルギーの評価
                //E = SimultaneousEqu(x);
                E = energy();
                //printf("E:%lf\n",-0.5*E);
            }
            sum[j] = 0.0;
        }
    }
    
    
}

double sigmoid(int i,double sum[]){
    double p;
    double a = 0.5;
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
    double ini_x[num] = {1.0,1.0,0.0,1.0,0.0,0.0};//最初の１はダミーニューロン
    int i;
    
    for(i=0;i<num;i++){
        x[i] = ini_x[i];
    }
    
}
