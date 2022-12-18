#include <iostream>

typedef struct commodities
{
    int number = 0;
    float price[5]; // current price and previous 4 prices
    float average[2];
} commodity;

using namespace std;

int main()
{
    cout << sizeof(commodity) << endl;
    // int arr[5] = {1, 2, 3, 4, 5};
    // int count = 5;

    // for (int i = 0; i < count; i++)
    // {
    //     cout << arr[i] << " ";
    // }
    // cout << endl;

    // cout << arr[0] << endl;

    // for (int i = 0; i < count - 1; i++)
    // {
    //     arr[i] = arr[i + 1];
    // }

    // count--;

    // for (int i = 0; i < count; i++)
    // {
    //     cout << arr[i] << " ";
    // }
    // cout << endl;
    return 0;
}