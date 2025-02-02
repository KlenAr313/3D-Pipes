#include "Builder.h"
#include <stdlib.h>
#include <ctime>
#include <glm/glm.hpp>
#include <iostream>

Builder::Builder(std::vector<bool>& availability, int gridSize, std::vector<Element*>& freshElements, glm::vec3 col, bool& canContinue, bool& noMoreSpace)
	:availability(availability), gridSize(gridSize), freshElements(freshElements), ID(allID++)
{
	this->color = glm::vec3(col);
	std::vector<int> possAvail;
	for (int i = 0; i < availability.size(); ++i)
	{
		if (availability[i])
		{
			possAvail.push_back(i);
		}
	}

	if(possAvail.size() != 0)
	{
		int ind = possAvail[std::rand() % possAvail.size()];

		availability[ind] = false;

		px = (ind - (ind % (gridSize * gridSize))) / (gridSize * gridSize);
		ind -= px * gridSize * gridSize;
		py = (ind - (ind % gridSize)) / gridSize;
		pz = ind - py * gridSize;

		std::vector<glm::vec3> ways;
		if (isIn(px + 1) && availability[toIndex(px + 1, py, pz)])
			ways.push_back(glm::vec3(1, 0, 0));

		if (isIn(px - 1) && availability[toIndex(px - 1, py, pz)])
			ways.push_back(glm::vec3(-1, 0, 0));

		if (isIn(py + 1) && availability[toIndex(px, py + 1, pz)])
			ways.push_back(glm::vec3(0, 1, 0));

		if (isIn(py - 1) && availability[toIndex(px, py - 1, pz)])
			ways.push_back(glm::vec3(0, -1, 0));

		if (isIn(pz + 1) && availability[toIndex(px, py, pz + 1)])
			ways.push_back(glm::vec3(0, 0, 1));

		if (isIn(pz - 1) && availability[toIndex(px, py, pz - 1)])
			ways.push_back(glm::vec3(0, 0, -1));

		if (ways.size() != 0)
		{
			glm::vec3 way = ways[std::rand() % ways.size()];

			fx = static_cast<int>(way.x);
			fy = static_cast<int>(way.y);
			fz = static_cast<int>(way.z);

			availability[toIndex(px + fx, py + fy, pz + fz)] = false;

			Element* e = new Element(px, py, pz, fx, fy, fz, color, ID, true, true, false); //begin can continue
			freshElements.push_back(e);
		}
		else
		{
			Element* e = new Element(px, py, pz, 1, 1, 1, color, ID, true, true, true); //begin can't continue
			freshElements.push_back(e);
			canContinue = false;
		}
	}
	else
	{
		noMoreSpace = true;
	}
}

bool Builder::next()
{
	int pfx = -fx;
	int pfy = -fy;
	int pfz = -fz;

	px += fx;
	py += fy;
	pz += fz;

	if (std::rand() % 100 < 30 || !isIn(px + fx) || !isIn(py + fy) || !isIn(pz + fz) || !availability[toIndex(px + fx, py + fy, pz + fz)])
	{
		std::vector<glm::vec3> ways;
		if (isIn(px + 1) && availability[toIndex(px + 1, py, pz)])
			ways.push_back(glm::vec3(1, 0, 0));

		if (isIn(px - 1) && availability[toIndex(px - 1, py, pz)])
			ways.push_back(glm::vec3(-1, 0, 0));

		if (isIn(py + 1) && availability[toIndex(px, py + 1, pz)])
			ways.push_back(glm::vec3(0, 1, 0));

		if (isIn(py - 1) && availability[toIndex(px, py - 1, pz)])
			ways.push_back(glm::vec3(0, -1, 0));

		if (isIn(pz + 1) && availability[toIndex(px, py, pz + 1)])
			ways.push_back(glm::vec3(0, 0, 1));

		if (isIn(pz - 1) && availability[toIndex(px, py, pz - 1)])
			ways.push_back(glm::vec3(0, 0, -1));

		if (ways.size() != 0)
		{
			glm::vec3 way = ways[std::rand() % ways.size()];

			if (!(fx == static_cast<int>(way.x) && fy == static_cast<int>(way.y) && fz == static_cast<int>(way.z)))
			{
				fx = static_cast<int>(way.x);
				fy = static_cast<int>(way.y);
				fz = static_cast<int>(way.z);


				Element* e = new Element(px, py, pz, fx, fy, fz, color, ID, true, false, false, pfx, pfy, pfz); //middle
				freshElements.push_back(e);
			}
			else
			{
				Element* e = new Element(px, py, pz, fx, fy, fz, color, ID, false); //cylinder
				freshElements.push_back(e);
			}

			availability[toIndex(px + fx, py + fy, pz + fz)] = false;
			return true;
		}
		else
		{
			Element* e = new Element(px, py, pz, 1, 1, 1, color, ID, true, false, true, pfx, pfy, pfz); //end, can't continue
			//TODO cannot pushback e; Seems solved don't know why
			freshElements.push_back(e);
			return false;
		}
	}
	else
	{
		Element* e = new Element(px, py, pz, fx, fy, fz, color, ID, false); //cylinder
		freshElements.push_back(e);
		availability[toIndex(px + fx, py + fy, pz + fz)] = false;
		return true;
	}


}

void Builder::end()
{
	availability[toIndex(px + fx, py + fy, pz + fz)] = true;
}

Builder::~Builder()
{
	//std::cout << "Builder killed" << std::endl;
}

int Builder::toIndex(int x, int y, int z)
{
	return gridSize * gridSize * x + gridSize * y + z;
}

bool Builder::isIn(int a)
{
	return a >= 0 && a < gridSize;
}

int Builder::allID = 0;

void Builder::resetIds()
{
	allID = 0;
}