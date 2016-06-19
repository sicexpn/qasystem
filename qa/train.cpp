#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstring>
#include<cstdio>
#include<map>
#include<vector>
#include<string>
#include<ctime>
#include<cmath>
#include<cstdlib>
using namespace std;

#define pi 3.1415926535897932384626433832795

//normal distribution[min,max)
double rand(double min, double max)
{
    return min+(max-min)*rand()/(RAND_MAX+1.0);
}
double normal(double x, double miu,double sigma)
{
    return 1.0/sqrt(2*pi)/sigma*exp(-1*(x-miu)*(x-miu)/(2*sigma*sigma));
}
double randn(double miu,double sigma, double min ,double max)
{
    double x,y,dScope;
    do{   
        x=rand(min,max);
        y=normal(x,miu,sigma);
        dScope=rand(0.0,normal(miu,miu,sigma));
    }while(dScope>y);
    return x; 
}            

double sqr(double x)
{   
    return x*x;
}

double vec_len(vector<double> &a)
{
     double res=0;
     for(int i=0;i<a.size();i++)
	  res+=a[i]*a[i];
     res = sqrt(res);
     return res;
}

char buf[100000];
int words_num,entity_num;
map<string,int> words2id,entity2id;
vector<vector<int> > q_matrix;

class Train{

public:
    void  add(int x)
    {
        fb_result.push_back(x);
    }
    void run(int k_in,double rate_in,double margin_in)
    {
        k = k_in;
        rate = rate_in;
        margin = margin_in;
        questions_vec.resize(words_num);
        for (int i=0; i<questions_vec.size(); i++)
             questions_vec[i].resize(k);
        entity_vec.resize(entity_num);
        for (int i=0; i<entity_vec.size(); i++)
             entity_vec[i].resize(k);
        questions_tmp.resize(words_num);
        for (int i=0; i<questions_tmp.size(); i++)
             questions_tmp[i].resize(k);
        entity_tmp.resize(entity_num);
        for (int i=0; i<entity_tmp.size(); i++)
             entity_tmp[i].resize(k);
        for (int i=0; i<words_num; i++)
        {
            for (int ii=0; ii<k; ii++)
                questions_vec[i][ii] = randn(0,1.0/k,-6/sqrt(k),6/sqrt(k));
        }
        for (int i=0; i<entity_num; i++)
        {
            for (int ii=0; ii<k; ii++)
                entity_vec[i][ii] = randn(0,1.0/k,-6/sqrt(k),6/sqrt(k));
            norm(entity_vec[i]);
        }
        bfgs();
    }

private:
    int k;
    double res;//loss function value
    double count,count1;//loss function gradient
    double rate,margin;
    vector<int> fb_result;
    vector<vector<int> > q_words;
    vector<vector<double> > questions_vec,entity_vec;
    vector<vector<double> > questions_tmp,entity_tmp;
    double norm(vector<double> &a)
    {
        double x = vec_len(a);
        if (x>1)
        for (int ii=0; ii<a.size(); ii++)
                a[ii]/=x;
        return 0;
    }
    int rand_max(int x)
    {
        int res = (rand()*rand())%x;
        while (res<0)
            res+=x;
        return res;
    }

    void bfgs()
    {
	res=0;
        int nbatches=2;
        int nepoch = 100;
        int batchsize = fb_result.size()/nbatches;
        for (int epoch=0; epoch<nepoch; epoch++)
        {
             res=0;
             for (int batch = 0; batch<nbatches; batch++)
             {
                    questions_tmp = questions_vec;
                    entity_tmp = entity_vec;
                    for (int count=0; count<batchsize; count++)
                    {
			  int i=rand_max(fb_result.size());
                          int j=rand_max(fb_result.size());
                          while (j==i){
			       j=rand_max(fb_result.size());
			  }
			  train_kb(q_matrix[i],fb_result[i],fb_result[j]);
                          vector<int>::iterator iter;
			  for(iter=q_matrix[i].begin();iter!=q_matrix[i].end();iter++)
		          {
                          	norm(questions_tmp[*iter]);
                          }
                          //cout<<"ok"<<endl;
                          norm(entity_tmp[fb_result[i]]);
                          norm(entity_tmp[fb_result[j]]);
			//cout<<"ok end"<<endl;
                   } 
	            questions_vec = questions_tmp;
                    entity_vec = entity_tmp;
             }
             cout<<"epoch:"<<epoch<<' '<<res<<endl;
             FILE* f2 = fopen(("words2vec"),"w");
             FILE* f3 = fopen(("entity2vec"),"w");
             for (int i=0; i<words_num; i++)
             {
                  for (int ii=0; ii<k; ii++)
                      fprintf(f2,"%.6lf\t",questions_vec[i][ii]);
                  fprintf(f2,"\n");
             }
             for (int i=0; i<entity_num; i++)
             {
                   for (int ii=0; ii<k; ii++)
                       fprintf(f3,"%.6lf\t",entity_vec[i][ii]);
                   fprintf(f3,"\n");
             }
             fclose(f2);
             fclose(f3);
	}
    }


    double calc_sum(vector<int> qm,int a)
    {
        double sum=0;
        vector<double> tmp(k,0);
        vector<int>::iterator iter;
        for(int ii=0;ii<k;ii++)
        {
            for(iter=qm.begin();iter!=qm.end();iter++){
		tmp[ii]+=questions_vec[*iter][ii];
	    }
	}
	for(int jj=0;jj<k;jj++)
        {
		sum+=tmp[jj]*entity_vec[a][jj];
	}
        return sum;
    }

    void gradient(vector<int> qm,int e1,int e2)
    {
        vector<double> tmp(k,0);
        for (int ii=0; ii<k; ii++)
        {
	     vector<int>::iterator iter;
             for(iter=qm.begin();iter!=qm.end();iter++){
	          questions_tmp[*iter][ii]-=rate*(entity_tmp[e2][ii]-entity_tmp[e1][ii]);
	     }
             for(iter=qm.begin();iter!=qm.end();iter++){
                  tmp[ii]+=questions_tmp[*iter][ii];
	     }

	     entity_tmp[e1][ii]-=-1*rate*tmp[ii];
	     entity_tmp[e2][ii]-=rate*tmp[ii];
	}

    }

    void train_kb(vector<int> qm,int e1,int e2)
    {
        //cout<<qm<<" "<<e1_a<<" "<<rel_a<<" "<<e2_a<<" "<<e1_b<<" "<<rel_b<<" "<<e2_b<<endl;
	double sum1 = calc_sum(qm,e1);
        double sum2 = calc_sum(qm,e2);
        if (sum1<margin+sum2)
        {
                res+=margin-sum1+sum2;
                gradient(qm,e1,e2);
        }

    }

};


Train train;

void prepare()
{
    FILE* f1 = fopen("wordsdict.txt","r");
    FILE* f2 = fopen("entitydict.txt","r");
    int x;
    while(fscanf(f1,"%s%d",buf,&x)==2)
    {
	string st = buf;
	words2id[st] = x;
	words_num++;
    }
    while(fscanf(f2,"%s%d",buf,&x)==2)
    {
	string st = buf;
	entity2id[st] = x;
        entity_num++;
    
    }
    ifstream f_qkb;
    f_qkb.open("questions.txt",ios::in);
    string tmp;
    while(getline(f_qkb,tmp,'\n'))
    {
	string buf1;
        stringstream ss(tmp);
        vector<int> tokens;
        while(ss >> buf1)
	    tokens.push_back(words2id[buf1]);
        q_matrix.push_back(tokens);
    }
    f_qkb.close();

    FILE* f_akb = fopen("answer.txt","r");
    while (fscanf(f_akb,"%s",buf)==1)
    {
        string s1=buf;
        train.add(entity2id[s1]);
        cout<<entity2id[s1]<<endl;
    }
    fclose(f_akb);
}


int main(int argc,char**argv)
{	
    int k = 50;
    double rate = 0.001;
    int margin = 0.1;
    prepare();
    train.run(k,rate,margin);
}
