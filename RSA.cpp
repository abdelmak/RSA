/* ProgramName:	RSA.cpp
 * Author:	Faris Abdel-Maksoud
 * Course:	Cybersecurity and Applications
 * Prof:	Dr. Habib Ammari
 * Description:	Implementation of RSA algorithm using C++.
 * Date Created:	11/25/18
 * Date Modified:	12/5/18
*/

#include <iostream>
#include <cmath>
#include <assert.h>

using std::cin;
using std::cout;
using std::endl;

/**********	BEGIN FUNCTION DEFINITIONS	**********/

//Modulo Function
//pre:	n is greater than 1, a can be negative.
//post:	return non-negative a mod n 
//	a mod n = r iff a = nk + r, 0 <= r < n
//	note: r needs to be non-negative.
//	note: ensures integer returned is non-negative.
int Mod(int a, int n){
	assert(n > 1);
	int r = a % n;

	if(r >= 0)
		return r;
	else
		return r + n;
	
	//or equivalently 
	//return r < 0 ? r + m : r;
}

//Exponential Modulo 
//calculate a^e mod n
//pre:	e >= 0, n >= 1
//post:	return a^e mod n
int ExpMod(int a, int e, int n){
	assert(e >= 0 && n >= 1);

	if(e == 0){
		return 1;
	}
	else{
		int result = ExpMod(a, e/2, n);

		if(e % 2 == 0){	//if e is even
			return Mod(result * result, n);
		}
		else{	//e is odd
			int result1= Mod(result * result , n);
			return Mod(result1 *  a, n);
		}
	}
}

//Euclidean Algorthim
//pre:	a >= b >= 0
//post:	return d = gcd(a,b)
int EuclidGCD(int a, int b){
	assert(a != 0 || b != 0);
	assert(a >= b);

	if(b == 0){
		return a;
	}
	else if(b == 1){	//checks for relative primality
		return b;
	}
	else{
		return EuclidGCD(b, a % b);
		//why not this: return (EuclidGCD(b, Mod(a, b));
		//this might be why there were issues in the final results
	}
}

//Extended Euclidean Algorithm
//pre:	a and b !=0, a >= b
//post:	return gcd(a,b)
//	s & t are set so that gcd(a,b) = a*s + b*t
int ExtendedEuclidGCD(int a, int b, int &s, int &t){
	assert(a !=0 || b != 0);
	assert(a >= b);

	if(b==0){
		s = 1;
		t = 0;
		return a;	//a = s*a + t*b
	}
	else{
		int q = a/b;	//q = a/b, auto floor
		int r = a%b;	//r = a - b*q, remainder	//once again, why not: Mod(a,b)
		int s1, t1;	//temp values

		int d = ExtendedEuclidGCD(b, r, s1, t1);
				//d = ExtendedEuclidGCD(b,r)
				//d = s1*b + t1*r
				//  = s1*b + t1*(a-b*q)
				//  = s1*b + t1*a - t1*q*b
				//  = t1*a + (s1-t1*q)*b
		s = t1;
		t = s1 - t1*q;
		return d;	//d = s*a + t*b
	}
}

//Inverse Modulo
//pre:	n > 1, a is non-neg
//	a <= n
//	a and n are coprime
//post:	return s such that a*s mod n is 1
int InverseMod(int a, int n){
	int s, t;
	int d = ExtendedEuclidGCD(n,a,s,t);

	if(d==1){
		return(Mod(t,n));
	}
	else{
		cout << "t,n are not relative primes...\n";
		return 0;
	}
}

//Primality
//Check the modulus of each integer from 2 up to the root of the number you're checking.
//If modulus equals zero then it's not prime
bool IsPrime(int x){
	assert(x >= 1);
	if(x == 1 || x == 2 || x == 3) return true;

	for (int i = 2; i <= x; i++){
		if ((Mod(x,i) == 0) || i != x)
			return false;
		}
	}
	return true;
}

//Relative primality
//pre:	
//post:	
//note:	return an int that is relative prime with n and greater than 1
int RelativePrime(int n){
	int m = 2;	

	while(EuclidGCD(n,m) != 1){
		m++;
	}
	return m;
}

//RSA Algorithm
//Functions implementing RSA Algorithm
int Encode(int M, int d, int PQ){
	return ExpMod(M, d, PQ);
}
int Decode(int C, int e, int PQ){
	return ExpMod(C, e, PQ);
}

/**********	BEGIN MAIN FUNCTION	**********/


int main(){

	cout << "-----Implementation of RSA Algorithm-----\n";
	/*
	const int P=89;
	const int Q=61;
	int PQ=P*Q;
	*/
	int P(0),Q(0);
	int PQ(0);
	bool primeP(0), primeQ(0);
	
	while(!primeP || !primeQ){
		cout << "Enter prime #, P: " << endl;	cin >> P;
		cout << "Enter prime #, Q: " << endl;	cin >> Q;
		primeP = IsPrime(P); primeQ = IsPrime(Q);
		if(!primeP || !primeQ)
			cout << "Not prime!\n";
	}
	PQ = P*Q;

	int d = RelativePrime((P-1)*(Q-1));	//find relative prime
	cout << "Relative Prime of (" << P <<"-1)(" << Q << "-1) = d = " << d << endl;

	int e = InverseMod(d,(P-1)*(Q-1));	//find inverse modulo
	assert(Mod(d*e,(P-1)*(Q-1)) == 1);	//d * e mod(p-1)*(q-1) should equal 1
	cout << "InverseMod(" << d << ",(" << P << "-1)*(" << Q << "-1)) = " << e << endl;

	int M;	//M is an int smaller than PQ
	cout << "Enter message, M: ";
	cin >> M;
	assert(M < PQ);

	int C = Encode(M, d, PQ);
	cout << "Ciphertext, C = " << C << endl;

	int DM = Decode(C, e, PQ);
	cout << "M1 = " << DM<< endl;

	assert(M==DM);

	return 0;

}


	/*
	//1. and 1.2
	int a = 660, b = 43;
	int s, t;
	int euclidean = EuclidGCD(a,b);
	int extendedEuclidean = ExtendedEuclidGCD(a,b,s,t);
	cout << "Euclidean Algorithm: \n";
	cout << "gcd(" << a << "," << b <<") = " << euclidean << endl;
	cout << "Extended Euclidean Algorithm: \n";
	cout << "gcd(" << a << "," << b <<") = " << extendedEuclidean << endl;

	//2.
	cout << "Inverse Modulo: \n";
	cout << InverseMod(b,a) << endl;

	//3.

	//4. Relative Prime
	cout << "Relative prime: \n";
	cout << RelativePrime(a) << endl;
	*/


