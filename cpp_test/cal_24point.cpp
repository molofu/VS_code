#include <iostream>
#include <math.h>

using namespace std;

double my_add(double a, double b)
{
    return a + b;
}

double my_sub(double a, double b)
{
    return a - b;
}

double my_mul(double a, double b)
{
    return a * b;
}

double my_dev(double a, double b)
{
    return a / b;
}

class res
{
public:
    double r = 0;
    int flag1 = 0;
    int flag2 = 0;
    int flag3 = 0;
};

void my_cal(double a, double e, double c, double d)
{
    int point = 0;

    cout << "请输入要求的结果：" << endl;
    cin >> point;
    double sum;
    res result[48];
    double num[4] = {a, e, c, d};
    int b = 0;
    double (*pfun[4])(double, double) = {my_add, my_sub, my_mul, my_dev};
    char al[4] = {'+', '-', '*', '/'};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                if (j != k)
                {
                    result[b].r = pfun[i](num[j], num[k]);
                    result[b].flag1 = j; // 数字位
                    result[b].flag2 = k; // 数字位
                    result[b].flag3 = i; // 符号位
                    b++;
                }
            }
        }
    }
    // cout << b << endl;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 48; j++)
        {
            for (int k = 0; k < 48; k++)
            {
                if (result[j].flag1 != result[k].flag1 && result[j].flag1 != result[k].flag2 && result[j].flag2 != result[k].flag1 && result[j].flag2 != result[k].flag2)
                {
                    if (pfun[i](result[j].r, result[k].r) == point)
                    {
                        /*cout << "(" << result[j].flag1 << " " << result[j].flag3 << " " << result[j].flag2 << ")"
                            << i << "(" << result[k].flag1 << " " << result[k].flag3 << " " << result[k].flag2 << ")" << endl;*/

                        cout << "(" << num[result[j].flag1] << " " << al[result[j].flag3] << " " << num[result[j].flag2] << ")"
                             << al[i] << "(" << num[result[k].flag1] << " " << al[result[k].flag3] << " " << num[result[k].flag2] << ")" << endl;
                        return;
                    }
                }
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                for (int l = 0; l < 4; l++)
                {
                    for (int m = 0; m < 48; m++)
                    {
                        if ((result[m].flag1 != k) && (result[m].flag1 != l) && (result[m].flag2 != k) && (result[m].flag2 != l) && (k != l))
                        {
                            if (fabs((pfun[i](pfun[j](result[m].r, num[k]), num[l])) - point) < 0.000001)
                            {
                                cout << "((" << num[result[m].flag1] << al[result[m].flag3] << num[result[m].flag2]
                                     << ")" << al[j] << num[k] << ")" << al[i] << num[l] << endl;
                                return;
                            }
                            // if (i == 2)
                            // {
                            //     cout << (pfun[i](pfun[j](result[m].r, num[k]), num[l])) << endl;
                            // }
                        }
                    }
                }
            }
        }
    }
    cout << "未成功" << endl;
    return;
}

int main()
{
    while (1)
    {
        int a, b, c, d;
        cin >> a >> b >> c >> d;

        my_cal(a, b, c, d);
    }

    return 0;
}