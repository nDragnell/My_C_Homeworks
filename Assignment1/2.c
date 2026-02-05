#include <stdio.h>

float radius,area;
float pi=3.14;

int main()
{
    printf("Please enter radius of the sphere to calculate area of sphere : ");
    scanf("%f",&radius);
    if (radius<0)
    {
        printf("Radius cannot be negative. \n");
    }
    else
    {
        area=4*pi*radius*radius;
        printf("The area of the sphere : %.2f",area);
    }
    
    return 0;
}