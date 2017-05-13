#include <QCoreApplication>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

int r(int x)
{
    return rand() % x;
}

int r(int x, int y)
{
    return (rand()% (y-x)) + x;
}

int main()
{
    srand(time(NULL));

    string maker[] = {"DMX", "Nickelodeon", "eSports", "ape Inc.", "Thermaltake", "MobileExpress"};
    string part[] = {"spoiler", "engine", "roof", "disk", "suspension", "carburetor", "brakes", "headlights"};
    string mark[] = {"BMW", "Lexus", "Nissan", "Skoda", "Mercedes", "LadaKalina", "КАМАЗ", "Renault"};
    string owner[] = {"Абрамович", "Ткаченко", "ОАО Бетонзавод", "Путин", "А.Станков"};

    /*for (int i = 0; i < 35; ++i)
    {
        cout << "INSERT [Details] ([detail_id], [price], [part], [maker], [name], [guarantee]) VALUES (" << i <<", "<< r(2000)
             << ", '" << part[r(8)] << "', '" << maker[r(6)] << "', 'dr-"<< r(1000, 9999)<<"', "<<r(10)<<")" << endl;
    }



        for (int i = 0; i < 35; ++i)
        {
            cout << "INSERT [Changes] ([delta_maxspeed], [delta_appearance], [delta_power]) VALUES ("
                 <<r(-70, 70) << ", "<<r(-10, 10) << ", " << r(-10, 10) << ")"<< endl;
        }


    for (int i = 0; i < 17; ++i)
    {
        cout << "INSERT [Cars] ([car_id], [maxspeed], [model], [year], [power], [appearance]) VALUES ("
             << i <<", "<< r(210) << ", '"<<mark[r(8)] << "', "<<r(1960, 2016) << ", " << r(1, 500) << ", " <<r(25, 75)<< ")" << endl;
    }

    for (int i = 0; i < 15; ++i)
    {
        cout << "INSERT [Owners] ([name], [car_id]) VALUES ('"
             << owner[r(5)] <<"', "<< r(17) << ")" << endl;
    }*/



    for (int i = 0; i < 35; ++i)
    {
        cout << "INSERT [ChangeList] ([car_id], [change_id], [worker_id], [date]) VALUES ("
             <<r(17) <<", "<< i << ", "<< r(9) <<", '2015-12-"<<r(1,29) <<")" << endl;
    }

    for (int i = 0; i < 35; ++i)
    {
        cout << "INSERT [Changes] ([change_id], [delta_maxspeed], [delta_appearance], [delta_power], [detail_id]) VALUES ("
             <<i <<", " <<r(-70, 70) << ", "<<r(-10, 10) << ", " << r(-10, 10) << ", " << r(30)<< ")"<< endl;
    }

    return 0;
}
