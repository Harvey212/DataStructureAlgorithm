#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <fstream>
#include <cmath>
#include <unordered_map>
using namespace std;
clock_t start_t, end_t;
double dif;

class DoubleHash{
public:
    int findPrime(int sizee)
    {
        int judge=0;
        for(int i=sizee;i<=2*sizee;i++)
        {

            int square=sqrt(i);
            for(int j=2;j<=square;j++)
            {
                if(i%j==0)
                {   judge=1;
                    break;
                }
            }

            if(judge==0)
            {
                return i;
            }
            judge=0;
        }
    }

    int findHash1(long int quotient,long int siz)
    {
        long int hashh=0;

        long int remainder,r;
        int countt=0;
        int rr;
        while(quotient!=0)
        {
            remainder=quotient%16;
            rr=countt%5;
            switch(rr)
            {
                case 0:
                    r=3;
                    break;
                case 1:
                    r=5;
                    break;
                case 2:
                    r=7;
                    break;
                case 3:
                    r=11;
                    break;
                case 4:
                    r=13;
                    break;
                //case 5:
                //    r=17;
                //    break;
            }   //rand()%siz;
            countt++;
            hashh=remainder*r+hashh;
            quotient=quotient/16;
        }
        hashh=hashh%siz;
        return hashh;
    }

        int findHash2(long int quotient2,long int siz2)
    {
        long int hashh2=0;

        long int remainder2,r2;
        int countt2=0;
        int rr2;
        while(quotient2!=0)
        {
            remainder2=quotient2%1000;
            rr2=countt2%7;
            switch(rr2)
            {
                case 0:
                    r2=3;
                    break;
                case 1:
                    r2=5;
                    break;
                case 2:
                    r2=7;
                    break;
                case 3:
                    r2=11;
                    break;
                case 4:
                    r2=13;
                    break;
                case 5:
                    r2=17;
                    break;
                case 6:
                    r2=19;
                    break;
            }   //rand()%siz;
            countt2++;
            hashh2=remainder2*r2+hashh2;
            quotient2=quotient2/1000;
        }
        hashh2=hashh2%siz2;
        return hashh2;
    }
};

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

int main()
{
    srand(time(NULL));
    int w1,w2;
    int p1,p2;
    int collide=0;
    int load=50;
    long int number;
    int searchSuccess=0;
    int searchFail=0;
    unordered_map<pair<int, int>, int, hash_pair> um;
    DoubleHash h;
    load=load*load;
    p1=h.findPrime(10000000);
    p2=h.findPrime(load);

    std::ofstream myfile;
    myfile.open ("hash.csv");

    for(int j=0;j<10000;j++)
    {
        start_t = clock();
            for(int i=0;i<1000;i++)
            {
                number=rand()%10000000+1;
                w1=h.findHash1(number,p1);
                w2=h.findHash2(number,p2);

                if(um[pair<int,int>(w1,w2)]==0)
                {
                    um[pair<int,int>(w1,w2)]=number;
                }else
                {
                    if(number!=um[pair<int,int>(w1,w2)])
                    {
                        collide=collide+1;
                        //cout<<number<<"="<<um[pair<int,int>(w1,w2)]<<endl;
                    }
                }
            }
        end_t = clock();
        dif = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        myfile <<dif<<"\n";
    }
    cout<<"collide:"<<collide<<endl;


    start_t = clock();
    for(int j=0;j<100000;j++)
    {
        number=rand()%10000000+1;
        w1=h.findHash1(number,p1);
        w2=h.findHash2(number,p2);
        if(um[pair<int,int>(w1,w2)]==number)
        {
           searchSuccess=searchSuccess+1;
        }else
        {
            searchFail=searchFail+1;
        }

    }
	end_t = clock();
    dif = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    cout<<"searchSucess:"<<searchSuccess<<" searchFail:"<<searchFail<<endl;
    cout <<"Search time:"<<dif<<endl;

    return 0;
}
