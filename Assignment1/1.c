#include <stdio.h>

float first, second, expected, experimental, error, errorpercentage, totalerrorpercentage;
char op;

int main()
{
    //The first operation

    printf("Please enter two values and one operation sign to calculate: ");
    scanf("%f %c %f", &first,&op,&second);

    if (op=='+')
    {
        expected=first+second;
        experimental=first-second;
    }

    else if (op=='-')
    {
        expected=first-second;
        experimental=first*second;
    }

    else if (op=='*')
    {
        expected=first*second;
        experimental=first+second;
    }

    else if (op=='/')
    {
        if(second!=0)
        {
            expected=first/second;
            experimental=first-second;
        }
        else
        {
            printf("The second value can not be zero.\n");
            return 0;
        }
    }

    else
    {
        printf("Invalid operation!\n");
        return 0;
    }

    error=expected-experimental;
    if(error<0)
    {
        error=-error;
    }
    
    errorpercentage=(error/expected)*100;
    
    if(expected==0)
    {
        errorpercentage=0;
    }
    
    if(errorpercentage<0)
    {
        errorpercentage=-errorpercentage;
    }
    totalerrorpercentage+=errorpercentage;

    printf("Expected Result: %.2f\n", expected);
    printf("Experimental Result: %.2f\n", experimental);
    printf("Error: %.2f\n", error);

    if (errorpercentage==0)
    {
        printf("Error Percentage: It could not calculate!\n");
    }

    else
    {
        printf("Error Percentage: %.2f%%\n", errorpercentage);
    }

    //The second operation
    printf("Please enter two values and one operation sign to calculate: ");
    scanf("%f %c %f", &first,&op,&second);

    if (op=='+')
    {
        expected=first+second;
        experimental=first-second;
    }

    else if (op=='-')
    {
        expected=first-second;
        experimental=first*second;
    }

    else if (op=='*')
    {
        expected=first*second;
        experimental=first+second;
    }

    else if (op=='/')
    {
        if(second!=0)
        {
            expected=first/second;
            experimental=first-second;
        }
        else
        {
            printf("The second value can not be zero.\n");
            return 0;
        }
    }

    else
    {
        printf("Invalid operation!\n");
        return 0;
    }

    error=expected-experimental;
    if(error<0)
    {
        error=-error;
    }
    
    errorpercentage=(error/expected)*100;
    
    if(expected==0)
    {
        errorpercentage=0;
    }
    
    if(errorpercentage<0)
    {
        errorpercentage=-errorpercentage;
    }
    totalerrorpercentage+=errorpercentage;

    printf("Expected Result: %.2f\n", expected);
    printf("Experimental Result: %.2f\n", experimental);
    printf("Error: %.2f\n", error);

    if (errorpercentage==0)
    {
        printf("Error Percentage: It could not calculate!\n");
    }

    else
    {
        printf("Error Percentage: %.2f%%\n", errorpercentage);
    }

    //The third operation

    printf("Please enter two values and one operation sign to calculate: ");
    scanf("%f %c %f", &first,&op,&second);

    if (op=='+')
    {
        expected=first+second;
        experimental=first-second;
    }

    else if (op=='-')
    {
        expected=first-second;
        experimental=first*second;
    }

    else if (op=='*')
    {
        expected=first*second;
        experimental=first+second;
    }

    else if (op=='/')
    {
        if(second!=0)
        {
            expected=first/second;
            experimental=first-second;
        }
        else
        {
            printf("The second value can not be zero.\n");
            return 0;
        }
    }

    else
    {
        printf("Invalid operation!\n");
        return 0;
    }

    error=expected-experimental;
    if(error<0)
    {
        error=-error;
    }
    
    errorpercentage=(error/expected)*100;
    
    if(expected==0)
    {
        errorpercentage=0;
    }
    
    if(errorpercentage<0)
    {
        errorpercentage=-errorpercentage;
    }
    totalerrorpercentage+=errorpercentage;

    printf("Expected Result: %.2f\n", expected);
    printf("Experimental Result: %.2f\n", experimental);
    printf("Error: %.2f\n", error);

    if (errorpercentage==0)
    {
        printf("Error Percentage: It could not calculate!\n");
    }

    else
    {
        printf("Error Percentage: %.2f%%\n", errorpercentage);
    }

    //The fourth operation

    printf("Please enter two values and one operation sign to calculate: ");
    scanf("%f %c %f", &first,&op,&second);

    if (op=='+')
    {
        expected=first+second;
        experimental=first-second;
    }

    else if (op=='-')
    {
        expected=first-second;
        experimental=first*second;
    }

    else if (op=='*')
    {
        expected=first*second;
        experimental=first+second;
    }

    else if (op=='/')
    {
        if(second!=0)
        {
            expected=first/second;
            experimental=first-second;
        }
        else
        {
            printf("The second value can not be zero.\n");
            return 0;
        }
    }

    else
    {
        printf("Invalid operation!\n");
        return 0;
    }

    error=expected-experimental;
    if(error<0)
    {
        error=-error;
    }
    
    errorpercentage=(error/expected)*100;
    
    if(expected==0)
    {
        errorpercentage=0;
    }
    
    if(errorpercentage<0)
    {
        errorpercentage=-errorpercentage;
    }
    totalerrorpercentage+=errorpercentage;

    printf("Expected Result: %.2f\n", expected);
    printf("Experimental Result: %.2f\n", experimental);
    printf("Error: %.2f\n", error);

    if (errorpercentage==0)
    {
        printf("Error Percentage: It could not calculate!\n");
    }

    else
    {
        printf("Error Percentage: %.2f%%\n", errorpercentage);
    }
    
    //Average Error 
    printf("Average Error Percent Value: %.2f%%\n",totalerrorpercentage/4);

    return 0;
}
