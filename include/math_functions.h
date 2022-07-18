/* ---------------------------------------------------------------------------------------------------------------------------------------- \
	Descrição: Contém funções matemáticas gerais.
	Criação: ??/??/????									Última Modificação: 26/08/2014

	Port: Leandro
\ ---------------------------------------------------------------------------------------------------------------------------------------- */

#pragma once

#include <cmath>
#include <limits>

#include "Point3D.h"
#include "PointList.h"
#include "SolidContour.h"
#include "Triangle.h"

#define PI 3.1415926
#define TAU 2*PI

#define COMPF_RESULT int
#define COMPF_BIGGER 1
#define COMPF_SMALLER -1
#define COMPF_EQUAL 0

float  roundIt (const float &num, const int &prec = 100);

bool compf (const float &a, const float &b, float epsilon = 0.0001);
COMPF_RESULT fullcompf (const float &a, const float &b, float epsilon = 0.0001);

float degreeToRadian (float degree);
float radianToDegreee (float radian);

float triangleArea(Point3D* point_a, Point3D* point_b, Point3D* point_c);
int pointPosition(Point3D* point_a, Point3D* point_b, Point3D* point_c);

float parametricIntersectionS(Point3D* point_a, Point3D* point_b, Point3D* point_c, Point3D* point_d);
float parametricIntersectionT(Point3D* point_a, Point3D* point_b, Point3D* point_c, Point3D* point_d);
int directionalPIP (Point3D* point_a, Point3D* point_b, PointList* _list);



int pointInPolygon (Point3D* point, PointList* _list);
int pointHelp (Point3D point, Point3D p1, Point3D p2);

float pointsDistanceAxisX (Point3D* p1, Point3D* p2);
float pointsDistanceAxisY (Point3D* p1, Point3D* p2);
void swap(float* a, float* b);

float pointLineDistance (Point3D p1, Point3D p2);

float pointAngleBetween (Point3D p1, Point3D p2, Point3D bet);

void deletePointLine (PointList* _list, float distance);
void deletePointAngle (PointList* _list, float angle);

float getNormal (bool flag, Point3D p1, Point3D p2);
bool getSentido (Point3D p1, Point3D p2);
float dotProduct (Point3D p1, Point3D p2);
Point3D externalProduct (Point3D p1, Point3D p2);
float angleBetweenVectors (Point3D p1, Point3D p2);

bool getIntersecPoint (Point3D* p1, Point3D* p2, Point3D* p3, Point3D* p4, Point3D* pi, int roundFactor = 100);
bool getRectIntersecPoint (Point3D* p1, Point3D* p2, Point3D* p3, Point3D* p4, Point3D* pi);
float correctCoord(float coord1, float coord2);

float calcArea (SolidContour* contour);

void normPointList(PointList* _list, int prec = 1000);

float linearCoef(const Point3D *p, const float angle);

float arredNum(float Num, float Prec);

bool isConvex(Point3D* p1, Point3D* p2, Point3D* p3);
