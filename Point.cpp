//
// Created by Peter on 3/25/2016.
//

//
// Created by Peter on 2/19/2016.
//
#include <iostream>

#include <cmath>
#include <sstream>
#include <stdlib.h>

#include "Point.h"
#include "Exceptions.h"

namespace Clustering {

    unsigned int Point::__idGen = 0;
    //static const char POINT_VALUE_DELIM=0;
    static const char POINT_VALUE_DELIM = ',';

    Point::Point(unsigned int dim) {
        if (dim == 0) { throw (ZeroDimensionsEx()); }
        __id = __idGen;
        __idGen++;
        __values = nullptr;
        __values = new double[dim];
        __dim = dim;
        for (int i = 0; i < dim; i++) {
            __values[i] = 0;
        }
    }

    void Point::rewindIdGen() {
        //std::cout<<__idGen<<" changed to ";
        __idGen--;
        //__id==__idGen;
        //std::cout<<__idGen<<std::endl;
    }

    Point::Point(unsigned int dim, double *values) {
        if (dim == 0) { throw (ZeroDimensionsEx()); }
        __id = __idGen;
        __idGen++;
        __values = new double[dim];
        __dim = dim;
        for (int i = 0; i < dim; i++) {
            __values[i] = values[i];
        }
    }

    Point::Point(const Point &inPoint) {
        if (inPoint.getDims() == 0) { throw (ZeroDimensionsEx()); }
        __id = inPoint.getId();
        __dim = inPoint.getDims();
        __values = new double[inPoint.getDims()];
        for (int i = 0; i < __dim; i++) {
            __values[i] = inPoint.getValue(i);
        }

    }

    Point &Point::operator=(const Point &inPoint) {
        if (__dim != inPoint.getDims()) { throw (DimensionalityMismatchEx(__dim, inPoint.getDims())); }
        __id = inPoint.getId();
        __dim = inPoint.getDims();

        __values = new double[inPoint.getDims()];
        for (int i = 0; i < __dim; i++) {
            __values[i] = inPoint.__values[i];
        }

        return *this;
    }

    Point::~Point() {
        delete[] __values;
    }

    int Point::getId() const {
        return __id;
    }

    unsigned int Point::getDims() const {
        return __dim;
    }

    void Point::setValue(unsigned int index, double inValue) {
        if (index >= getDims() | index < 0) { throw (OutOfBoundsEx(index, getDims())); }
        __values[index] = inValue;

    }

    double Point::getValue(unsigned int index) const {
        if (index >= getDims() | index < 0) { throw (OutOfBoundsEx(index, getDims())); }
        return __values[index];

    }

    double Point::distanceTo(const Point &point2) const {
        if (getDims() != point2.getDims()) { throw (DimensionalityMismatchEx(getDims(), point2.getDims())); }
        double *distanceArray;
        double sum = 0;
        distanceArray = new double[__dim];

        for (unsigned int i = 0; i < __dim; i++) {
            double manhatanDistance = (point2.getValue(i) - __values[i]);
            distanceArray[i] = manhatanDistance * manhatanDistance;
        }
        for (int i = 0; i < __dim; i++) {
            sum += distanceArray[i];
        }
        delete[] distanceArray;

        return sqrt(sum);


    }

/*
    double Point::distanceTo(const Point &point2) const {
        int  dims=point2.getDims()
        Point difference(dims);
        rewindIdGen();
        Point differenceSquared(dims);
        rewindIdGen();
        difference=*this-point2;
        for(int i=0; i<dims; i++){

        }

    }*/

    Point &Point::operator*=(double multiplier) {
        for (int i = 0; i < __dim; i++) {
            __values[i] *= multiplier;
        }
        return *this;
    }

    Point &Point::operator/=(double divider) {
        for (int i = 0; i < __dim; i++) {
            __values[i] /= divider;
        }
        return *this;
    }

    const Point  Point::operator*(double multiplier) const {
        return Point(*this) *= multiplier;
    }

    const Point  Point::operator/(double divider) const {


        return Point(*this) /= divider;
    }

    double &Point::operator[](unsigned int index) {
        if (index >= __dim) { throw (OutOfBoundsEx(index, getDims())); }
        return __values[index];
    }
    // const double &Point::operator[](unsigned int index) const{
    //if(index>=getDims()|index<0){throw(OutOfBoundsEx(index,getDims()));}
    //return __values[index];
    // }

    Point &operator+=(Point &lhs, const Point &rhs) {
        if (lhs.getDims() != rhs.getDims()) { throw (DimensionalityMismatchEx(lhs.getDims(), rhs.getDims())); }
        if (&lhs == &rhs) {
            return lhs *= 2;
        } else if (lhs.__dim == rhs.__dim) {
            for (int i = 0; i < lhs.__dim; i++)
                lhs.__values[i] += rhs.__values[i];
        }
        return lhs;
    }

    Point &operator-=(Point &lhs, const Point &rhs) {
        if (lhs.getDims() != rhs.getDims()) { throw (DimensionalityMismatchEx(lhs.getDims(), rhs.getDims())); }
        if (&lhs == &rhs) {
            return lhs *= 2;
        } else if (lhs.__dim == rhs.__dim) {
            for (int i = 0; i < lhs.__dim; i++)
                lhs.__values[i] -= rhs.__values[i];
        }
        return lhs;

    }

    const Point operator+(const Point &point1, const Point &point2) {
        if (point1.getDims() != point2.getDims()) {
            throw (DimensionalityMismatchEx(point1.getDims(), point2.getDims()));
        }
        Point sum(point1);

        return sum += point2;
    }

    const Point  operator-(const Point &point1, const Point &point2) {
        if (point1.getDims() != point2.getDims()) {
            throw (DimensionalityMismatchEx(point1.getDims(), point2.getDims()));
        }

        Point difference(point1);

        return difference -= point2;
    }

    bool operator==(const Point &lhs, const Point &rhs) {

        if (lhs.getDims() != rhs.getDims()) { throw (DimensionalityMismatchEx(lhs.getDims(), rhs.getDims())); }
        for (int i = 0; i < lhs.__dim; i++) {
            if (lhs.__values[i] != rhs.__values[i]) {
                return false;
            }
        }


        return lhs.getId() == rhs.getId();

    }

    bool operator!=(const Point &lhs, const Point &rhs) {
        if (lhs.getDims() != rhs.getDims()) { throw (DimensionalityMismatchEx(lhs.getDims(), rhs.getDims())); }
        return !(lhs == rhs);

    }

    bool operator<(const Point &lhs, const Point &rhs) {
        if (lhs.getDims() != rhs.getDims()) { throw (DimensionalityMismatchEx(lhs.getDims(), rhs.getDims())); }
        bool areEqual = true;
        for (int i = 0; i < lhs.__dim; i++) {
            if (lhs.__values[i] != rhs.__values[i]) {

                areEqual = false;
            }

        }
        return (lhs <= rhs) & !areEqual;

    }

    bool operator>(const Point &lhs, const Point &rhs) {
        if (lhs.getDims() != rhs.getDims()) { throw (DimensionalityMismatchEx(lhs.getDims(), rhs.getDims())); }

        bool areEqual = true;
        for (int i = 0; i < lhs.__dim; i++) {
            if (lhs.__values[i] != rhs.__values[i]) {

                areEqual = false;
            }

        }

        return !(lhs <= rhs) & !areEqual;


    }


    bool operator<=(const Point &lhs, const Point &rhs) {
        if (lhs.getDims() != rhs.getDims()) { throw (DimensionalityMismatchEx(lhs.getDims(), rhs.getDims())); }
        for (int i = 0; i < rhs.__dim; i++) {
            if (lhs.__values[i] < rhs.__values[i]) {

                return true;
            }
            if (lhs.__values[i] > rhs.__values[i]) {

                return false;
            }

        }
    }

    bool operator>=(const Point &lhs, const Point &rhs) {
        if (lhs.getDims() != rhs.getDims()) { throw (DimensionalityMismatchEx(lhs.getDims(), rhs.getDims())); }

        bool areEqual = true;
        for (int i = 0; i < lhs.__dim; i++) {
            if (lhs.__values[i] != rhs.__values[i]) {
                areEqual = false;
            }

        }
        return lhs > rhs | areEqual;
    }

    std::ostream &operator<<(std::ostream &outStream, const Point &inPoint) {
        double value = 0;
        for (int i = 0; i < inPoint.__dim; i++) {
            outStream << inPoint.getValue(i);
            if (i < inPoint.__dim - 1) {
                outStream << ',';
            }


        }
        return outStream;
    }

    std::istream &operator>>(std::istream &inStream, Point &outPoint) {
        std::string line;
        std::getline(inStream, line);//we read the current stream and get a string out of it.
        double value = 0;

        int count = 0;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == POINT_VALUE_DELIM) {
                line[i] = ' ';//We replace all comas with whitespaces so that the
                // stringstream >> operator down the line can easily read it. I know its a strange method but it works.
                count++;//The total number of dimensions is the total number of comas +1 so we count all the comas.
            }
        }
        count++;//then we add one.
        if (count != outPoint.getDims()) { throw (DimensionalityMismatchEx(count, outPoint.getDims())); }

        std::istringstream iss(line);
        for (int i = 0; i < count; i++) {
            iss >> value;
            outPoint.__values[i] = value;
        }
        return inStream;
    }


}