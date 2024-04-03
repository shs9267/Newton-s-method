#include <stdio.h>
#include <math.h>

// 다항식 함수
double p1(double coefficient[], int degree, double x) {
    double result = 0;
    for (int i = 0; i <= degree; i++) {
        result += coefficient[i] * pow(x, degree - i);
    }
    return result;
}

// 다항식의 미분 함수
double p2(double coefficient[], int degree, double x) {
    double result = 0;
    for (int i = 0; i < degree; i++) {
        result += coefficient[i] * (degree - i) * pow(x, degree - i - 1);
    }
    return result;
}

// Newton's Method 구현
double newton(double coefficient[], int degree, double x0, double tolerance, int max_repeat) {
    double x = x0;
    int repeat = 1;
    
    // 반복문을 이용한 Newton's Method
    while (p1(coefficient, degree, x) > tolerance && repeat < max_repeat) {
        x = x - p1(coefficient, degree, x) / p2(coefficient, degree, x); // Newton's Method 공식
        repeat++;
    }

    // 결과 출력
    if (repeat == max_repeat) {
        printf("최대 반복 횟수에 도달했습니다. 근을 찾을 수 없습니다.\n");
    } else {
        printf("반복한 횟수입니다: %d\n", repeat);
        printf("근을 찾았습니다: %lf\n", x);
    }
    
    return x;
}

int main() {
    int degree;
    double coefficient[11]; // 다항식의 계수
    
    // 사용자로부터 차수 입력 받기
    printf("다항식의 차수를 입력하세요 (최대 10) : ");
    scanf("%d", &degree);
    
    // 사용자로부터 계수 입력 받기
    printf("각 항의 계수를 차수가 높은 순서대로 입력하세요:\n");
    for (int i = 0; i <= degree; i++) {
        printf("x^%d의 계수: ", degree - i);
        scanf("%lf", &coefficient[i]);
    }
    
    double first;
    double tolerance;
    int max_repeat;
    
    // 초기 추정값, 허용 오차, 최대 반복 횟수 입력 받기
    printf("초기 추정값을 입력하세요: ");
    scanf("%lf", &first);
    printf("허용 오차를 입력하세요: ");
    scanf("%lf", &tolerance);
    printf("최대 반복 횟수를 입력하세요: ");
    scanf("%d", &max_repeat);
    
    // Newton's Method 호출
    newton(coefficient, degree, first, tolerance, max_repeat);
    
    return 0;
}
