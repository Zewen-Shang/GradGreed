#pragma once

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkTetra.h>
#include <vtkPolyDataWriter.h>

#include <array>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <iostream>
#include <Eigen/Dense>

#include "Utils.h"
#include "Heap.h"
#define N 70000

using namespace std;
using namespace Eigen;

typedef Matrix<double, 6, 1> Vector6d;

enum State {
	Valid,
	Invalid
};

class Prime {
public:

	State state = Valid;
	void kill() {
		state = Invalid;
	}
	bool valid() {
		return state == Valid;
	}
};

class Vertex;
class Edge;
class Tetra;

class Vertex:public Prime
{
public:
	int id;
	Vector6d pos;
	//������ص�
	vector<Edge*> edge;
	Vertex();
	Vertex(int id, Vector6d pos);
	void mapTo(Vertex* target);
	Edge* isNeighbour(Vertex* other) const;
	vector<Tetra*> getTetras() const;
};


class Edge:public Prime ,public HeapNode{
public:
	Vertex* v0, * v1;
	vector<Tetra*> tetra;
	Edge() {};
	Edge(Vertex* v0,Vertex* v1);
	Vector6d midPos;
	void collapse();
	//����/���Ƿ��ڱ���
	bool checkVertex(Vertex* v);
	bool checkTetra(Tetra* f);
	//Ѱ��v֮�����һ����/�棬v/f����e�Ͼͱ���
	Vertex* anotherVertex(Vertex* v);
	//����v1v0˳�򣬱�֤v0idС��v1id
	void adjust();
	//����from�ڵ��ָ���Ϊ��to�ڵ��ָ��
	void changeVertex(Vertex* from, Vertex* to);
	void mapTo(Edge* target);
	double len();
	void InitCost(Heap &H);
	void updateCost();
	double calcCost();
	bool checkInversion() const;
};

class Tetra :public Prime {
public:
	Edge* es[6];
	Tetra() {};
	Tetra(Edge* es[]);
	bool checkEdge(Edge* e);
	void changeEdge(Edge* from,Edge*to);
	vector<int> getVertexIds() const;
	vector<Vertex*>getVertexs() const;
	bool checkInversion(Vertex* v, Vector6d midPos);
	Vector6d getCenter()const;
};

