#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

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

// 다항식의 이계도함수
double p3(double coefficient[], int degree, double x) {
    double result = 0;
    for (int i = 0; i < degree; i++) {
        result += coefficient[i] * (degree - i) * (degree - i - 1) * pow(x, degree - i - 2);
    }
    return result;  
}

// Newton's Method 구현
double newton(double coefficient[], int degree, double x0, double tolerance, int max_repeat) {
    double x = x0;
    int repeat = 1;
    
    // 반복문을 이용한 Newton's Method
    while (fabs(p2(coefficient, degree, x)) > tolerance && repeat < max_repeat) {
        x = x - p2(coefficient, degree, x) / p3(coefficient, degree, x); // Newton's Method 공식
        repeat++;
    }

    double result = p1(coefficient, degree, x);

    // 결과 출력
    if (repeat == max_repeat) {
        printf("최대 반복 횟수에 도달했습니다. 근을 찾을 수 없습니다.\n");
    } else {
        printf("반복한 횟수입니다: %d\n", repeat);
        printf("f(x)의 최솟값입니다: %lf\n", result);
    }

    return result;
}

int main() {
    int degree;
    double coefficient[11]; // 다항식의 계수
    double tolerance;
    int max_repeat;
    int num_trials;

    // 사용자로부터 차수 입력 받기
    printf("다항식의 차수를 입력하세요 (최대 10) : ");
    scanf("%d", &degree);
    
    // 사용자로부터 계수 입력 받기
    printf("각 항의 계수를 차수가 높은 순서대로 입력하세요:\n");
    for (int i = 0; i <= degree; i++) {
        printf("x^%d의 계수: ", degree - i);
        scanf("%lf", &coefficient[i]);
    }
    
    // 허용 오차와 최대 반복 횟수 입력 받기
    printf("허용 오차를 입력하세요: ");
    scanf("%lf", &tolerance);
    printf("최대 반복 횟수를 입력하세요: ");
    scanf("%d", &max_repeat);

    // 시도 횟수 입력 받기
    printf("몇 번의 시도를 하시겠습니까? ");
    scanf("%d", &num_trials);
    
    srand(time(NULL));

    double min_result = INFINITY; // 최솟값을 저장할 변수, 초기값은 무한대로 설정

    // Newton's Method를 여러 번 호출하여 랜덤한 초기 추정값 사용
    for (int i = 0; i < num_trials; i++) {
        double random = (double)rand() / RAND_MAX * 200.0 - 100.0; // -100에서 100 사이의 double 값
        printf("시도 %d:\n", i + 1);
        double result = newton(coefficient, degree, random, tolerance, max_repeat);
        printf("난수 값입니다: %lf\n", random);
        if (result < min_result) {
            min_result = result; // 현재의 f(x) 값이 최솟값보다 작으면 최솟값을 업데이트
        }
        printf("\n");
    }

    printf("최소 f(x) 값: %lf\n", min_result);

    return 0;
}
