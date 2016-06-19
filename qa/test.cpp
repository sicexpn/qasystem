#include<iostream>
#include<fstream>
#include<sstream>
#include<cstring>
#include<cstdio>
#include<map>
#include<vector>
#include<string>
#include<ctime>
#include<algorithm>
#include<cmath>
#include<cstdlib>
using namespace std;

map<string,int> words2id,entity2id;
map<int,string> id2entity;
int words_num,entity_num;
int k= 50;
vector<vector<int> > q_matrix;

double sigmod(double x)
{
    return 1.0/(1+exp(-x));
}

double vec_len(vector<double> a)
{
	double res=0;
	for (int i=0; i<a.size(); i++)
		res+=a[i]*a[i];
	return sqrt(res);
}

void vec_output(vector<double> a)
{
	for (int i=0; i<a.size(); i++)
	{
		if (i%10==9)
			cout<<endl;
	}
}

double sqr(double x)
{
    return x*x;
}

char buf[100000];


double cmp(pair<int,double> a, pair<int,double> b)
{
     return a.second<b.second;
}

class Test{
    vector<vector<double> > questions_vec,entity_vec;
    vector<int> h,t,r;
    vector<int> fb_result;
    double res ;
public:
    void add(int x)
    {
	    fb_result.push_back(x);
    }

    int rand_max(int x)
    {
        int res = (rand()*rand())%x;
        if (res<0)
            res+=x;
        return res;
    }
    double len;
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
	for(int jj=0;jj<k;jj++){
            sum+=tmp[jj]*entity_vec[a][jj];
        }
        return sum;
    }
    void run()
    {
        FILE* f1 = fopen("words2vec","r");
        FILE* f3 = fopen("entity2vec","r");
        int words_num_fb=words_num;
        questions_vec.resize(words_num_fb);
        for (int i=0; i<words_num_fb;i++)
        {
            questions_vec[i].resize(k);
            for (int ii=0; ii<k; ii++)
                fscanf(f1,"%lf",&questions_vec[i][ii]);
        }
        entity_vec.resize(entity_num);
        for (int i=0; i<entity_num;i++)
        {
            entity_vec[i].resize(k);
            for (int ii=0; ii<k; ii++)
                fscanf(f3,"%lf",&entity_vec[i][ii]);
        }
        fclose(f1);
        fclose(f3);
	double lsum=0 ,lsum_filter= 0;
	double rsum = 0,rsum_filter=0;
	double lp_n=0,lp_n_filter;
	double rp_n=0,rp_n_filter;
	map<int,double> lsum_r,lsum_filter_r;
	map<int,double> rsum_r,rsum_filter_r;
	map<int,double> lp_n_r,lp_n_filter_r;
	map<int,double> rp_n_r,rp_n_filter_r;
	for (int testid = 0; testid<fb_result.size(); testid+=1)
	{
		int h = fb_result[testid];
                vector<int> q = q_matrix[testid];
		vector<pair<int,double> > a;
		for (int i=0; i<entity_num; i++)
		{
			double sum = calc_sum(q,i);
			a.push_back(make_pair(i,sum));
		}
		sort(a.begin(),a.end(),cmp);
		for (int i=0; i<a.size(); i++)
		{
			if (a[i].first ==h)
			{
				cout<<id2entity[i]<<endl;
                                lsum+=i;
				if (i<=10)
				{
					lp_n+=1;
				}
				break;
				}
			}
			a.clear();
                }
        
                //cout<<lsum<<'\t'<<fb_result.size()<<'\t'<<lsum/fb_result.size()<<endl;
                //cout<<lp_n<<'\t'<<fb_result.size()<<'\t'<<lp_n/fb_result.size()<<endl;
    }

};
Test test;

void prepare()
{
    FILE* f1 = fopen("wordsdict.txt","r");
    FILE* f2 = fopen("entitydict.txt","r");
    int x;
    while (fscanf(f1,"%s%d",buf,&x)==2)
    {
	    string st=buf;
	    words2id[st]=x;
	    words_num++;
    }
    while (fscanf(f2,"%s%d",buf,&x)==2)
    {
	    string st=buf;
	    entity2id[st]=x;
            id2entity[x]=st;
	    entity_num++;
    }

    ifstream f_qkb;
    f_qkb.open("questions_for_test.txt",ios::in);
    string tmp;
    while(getline(f_qkb,tmp,'\n'))
    {
	    string buf1;
	    stringstream ss(tmp);
	    vector<int> tokens;
	    while(ss>>buf1)
		    tokens.push_back(words2id[buf1]);
	    q_matrix.push_back(tokens);
    }
    f_qkb.close();

    FILE* f_akb = fopen("answer_for_test.txt","r");
    while (fscanf(f_akb,"%s",buf)==1)
    {
	    string s1=buf;
	    test.add(entity2id[s1]);
    }
    fclose(f_akb);
}


int main(int argc,char**argv)
{
	prepare();
        test.run();
}

