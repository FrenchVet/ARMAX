#include "armaxfunctor.h"



armaxfunctor::armaxfunctor()
{

}

 double armaxfunctor::operator() (const column_vector& Model) const
{
    vector<double> PreviousValues;

    int length = this->y.size();
    double output  = 0;
    double prediction;
    int i;
    int m;
    //cout << "Proba: " << Model(0) << " " << Model(1) << " "<< Model(2) << " "<< Model(3) << " "<< '\n';
    for(i = 0; i < this->MaxModel; i++)
    {
        PreviousValues.push_back(this->E[i]);
    }

    for(i; i < length; i++)
    {
       prediction = this->E[i];
       for(m = 0; m < this->AR; m++)
       {
           prediction += Model(m)*PreviousValues[AR-m-1];
       }
       for(m; m < (this->AR+this->MA); m++)
       {
           prediction += Model(m)*this->E[i-m+AR-1];
       }
       PreviousValues.erase(PreviousValues.begin());
       PreviousValues.push_back(prediction);
       output += fabs((prediction-this->y[i])/this->y[i]);
    }

    output = output/length;
    return output;

}


