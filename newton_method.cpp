#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

// 기반 클래스 정의
class Polynomial {
protected:
    double* coefficients; // 다항식의 계수를 저장하는 동적 배열
    int degree; // 다항식의 차수

public:
    // 기본 생성자
    Polynomial() : coefficients(), degree(0) {}

    //추상클래스
    virtual double findMin(double x0, double tolerance, int max_repeat) = 0;
    virtual double findGlobalMin(double tolerance, int max_repeat, int num_trials) = 0;

    // 생성자: 계수를 받아 다항식을 초기화
    Polynomial(double* coeffs, int deg) : degree(deg) {
        coefficients = new double[degree + 1]; // 동적 배열 할당
        for (int i = 0; i <= degree; ++i) {
            coefficients[i] = coeffs[i];
        }
    }

    // 다항식의 값을 계산하는 함수
    double p1(double x) const {
        double result = 0;
        for (int i = 0; i <= degree; i++) {
            result += coefficients[i] * pow(x, degree - i); // 각 항의 계수와 x의 거듭제곱을 곱하여 더함
        }
        return result;
    }

    // 다항식의 1차 도함수 값을 계산하는 함수
    double p2(double x) const {
        double result = 0;
        for (int i = 0; i < degree; i++) {
            result += coefficients[i] * (degree - i) * pow(x, degree - i - 1); // 각 항의 계수와 차수를 곱하여 도함수 계산
        }
        return result;
    }

    // 다항식의 2차 도함수 값을 계산하는 함수
    double p3(double x) const {
        double result = 0;
        for (int i = 0; i < degree - 1; i++) {
            result += coefficients[i] * (degree - i) * (degree - i - 1) * pow(x, degree - i - 2); // 각 항의 계수와 차수를 곱하여 2차 도함수 계산
        }
        return result;
    }

    // 난수를 생성하는 함수
    double generateRandom(double min = -100.0, double max = 100.0) {
        return min + (double(rand()) / RAND_MAX) * (max - min);
    }

    // 소멸자: 동적 배열 메모리 해제
    virtual ~Polynomial() {
        delete[] coefficients;
    }
};

// Newton's Method 클래스를 정의 - 유도클래스
class NewtonMethod : public Polynomial {
public:
    // 생성자: 다항식 객체를 받아 NewtonMethod 초기화
    NewtonMethod(double* coeffs, int deg) : Polynomial(coeffs, deg) {}

    // Newton's Method를 사용하여 함수의 최소값을 찾는 함수
    double findMin(double x0, double tolerance, int max_repeat) {
        double x = x0; // 초기 추정값
        int repeat = 0; // 반복 횟수 초기화
        double lower_bound = -1e10; // 임계값 설정

        // 허용 오차와 최대 반복 횟수 내에서 반복
        while (fabs(p2(x)) > tolerance && repeat < max_repeat) {
            double p3x = p3(x); // 2차 도함수 계산
            if (p3x == 0) {
                cout << "2차 도함수가 0이므로 중단합니다." << endl;
                return numeric_limits<double>::infinity(); // 2차 도함수가 0이면 중단
            }
            x = x - p2(x) / p3x; // Newton's Method 공식 적용
            repeat++;

            // p1(x)의 값이 임계값보다 작아질 경우 반복 중단
            if (p1(x) < lower_bound) {
                cout << "p1(x)의 값이 임계값보다 작아 중단합니다." << endl;
                return numeric_limits<double>::infinity(); // 음의 무한대 대신 무한대 반환
            }
        }
        double realx = x;
        double result = p1(x); // 최종 x 값에서 다항식 값 계산

        // 결과 출력
        if (repeat == max_repeat) {
            cout << "최대 반복 횟수에 도달했습니다. 근을 찾을 수 없습니다." << endl;
        }
        else {
            //cout << "반복한 횟수입니다 : " << repeat << endl;
            cout << "f'(x)=0을 만족시키는 x 값 :" << realx << endl;
            cout << "f(x) 값 : " << result << endl;
        }

        return result;
    }

    // 여러 번 Newton's Method를 호출하여 최솟값을 찾는 함수
    double findGlobalMin(double tolerance, int max_repeat, int num_trials) {
        double min_result = numeric_limits<double>::infinity(); // 최솟값을 저장할 변수, 초기값은 무한대로 설정

        // Newton's Method를 여러 번 호출하여 랜덤한 초기 추정값 사용
        for (int i = 0; i < num_trials; i++) {
            double random = generateRandom(); // 난수 생성
            cout << "시도 " << i + 1 << ":" << endl;
            double result = findMin(random, tolerance, max_repeat); // Newton's Method 호출
            //cout << "난수 값입니다: " << random << endl;
            
            if (result < min_result) {
                min_result = result; // 현재의 f(x) 값이 최솟값보다 작으면 최솟값을 업데이트
            }
            cout << endl;
        }

        cout << "최소 f(x) 값: " << min_result << endl; // 최종 최솟값 출력
        return min_result;
    }
};

int main() {
    srand(static_cast<unsigned>(time(NULL))); // 랜덤 시드 초기화
    int degree;
    double* coefficients;
    double tolerance;
    int max_repeat;
    int num_trials;

    cout << "다항식의 차수를 입력하세요: ";
    cin >> degree;

    if (degree < 1) {
        cout << "차수는 1 이상의 정수여야 합니다." << endl;
        return 1;
    }

    coefficients = new double[degree + 1]; // 다항식의 계수를 저장할 동적 배열 할당

    cout << "각 항의 계수를 차수가 높은 순서대로 입력하세요:" << endl;
    for (int i = 0; i <= degree; ++i) {
        cout << "x^" << degree - i << "의 계수: ";
        cin >> coefficients[i];
    }

    NewtonMethod newton(coefficients, degree); // NewtonMethod 객체 생성

    cout << "허용 오차를 입력하세요: ";
    cin >> tolerance;
    cout << "최대 반복 횟수를 입력하세요: ";
    cin >> max_repeat;
    cout << "몇 번의 시도를 하시겠습니까? ";
    cin >> num_trials;

    newton.findGlobalMin(tolerance, max_repeat, num_trials); // 여러 번 Newton's Method를 호출하여 최솟값 찾기

    delete[] coefficients; // 동적 배열 메모리 해제

    return 0;
}