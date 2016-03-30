//
// Created by Peter on 3/25/2016.
//

//
// Created by Peter on 2/19/2016.
//
//Cluster implementation
#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>
#include <assert.h>
#include "Cluster.h"
#include "Exceptions.h"

namespace Clustering {
    unsigned int Cluster::__idGenerator = 0;
    typedef struct LNode *LNodePtr;

    LNode::LNode(const Point &p, LNodePtr n) : point(p.getDims()) {
        point = p;
        p.rewindIdGen();
        next = n;
    }

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) : __p(d), __c(c) {
        __valid = false;
        __dimensions = c.getDimensionality();
    }

    bool Cluster::Centroid::equal(const Clustering::Point &p) const {
        if (p.getDims() != __p.getDims()) { throw (DimensionalityMismatchEx(__p.getDims(), p.getDims())); }
        for (unsigned int i = 0; i < p.getDims(); i++) {
            if (p.getValue(i) != __p.getValue(i)) {
                return false;
            }
        }
        return true;
    }

    void Cluster::Centroid::setValid(bool valid) {
        __valid = valid;
    }

    bool Cluster::Centroid::isValid() const {
        return __valid;
    }


    void Cluster::Centroid::compute() {
        if (!__valid) {
            __valid = true;
            if (__c.__points == nullptr) {
                toInfinity();
                return;
            }

            LNodePtr curr;
            curr = __c.__points;
            int sizeCheck = 0;
            __p = Point(__c.getDimensionality());
            while (curr != nullptr) {
                __p += curr->point / __c.getSize();
                curr = curr->next;
                sizeCheck++;

            }

            assert(sizeCheck == __c.getSize());
            //std::cout<<"new Centroid for cluster "<<__c.getId()<<" is "<< __p<<std::endl;

        }
        return;
    }


    const Point Cluster::Centroid::get() const {
        return __p;
    }

    void Cluster::Centroid::set(const Point &p) {
        __p = p;
        __valid = true;
    }


    void Cluster::Centroid::toInfinity() {
        for (int i = 0; i < __p.getDims(); i++) {
            __p.setValue(i, std::numeric_limits<double>::max());
        }
    }


    Cluster::Cluster(unsigned int d) : centroid(d, *this) {
        __id = __idGenerator;
        __idGenerator++;
        __dimensionality = d;
        __size = 0;
        __points = nullptr;
    }


    void Cluster::__del() {

        while (__size > 0) {
            LNodePtr cursor = __points;
            for (int i = 0; i < __size; ++i) {
                cursor = cursor->next;
            }
            __size--;
            delete cursor;
        }
        __points = nullptr;
        centroid.setValid(false);
    }

    void Cluster::__cpy(LNodePtr pts) {
        __del();
        LNodePtr cursor = pts;
        while (cursor != nullptr) {
            add(cursor->point);
            cursor = cursor->next;
        }


    }

    bool Cluster::__in(const Point &p) const {
        LNodePtr cursor = __points;
        for (int i = 0; i < __size; i++) {
            if (cursor->point == p) {
                return true;
            }
            cursor = cursor->next;
        }
        return false;
    }

    Cluster::Cluster(const Cluster &c) : centroid(c.getDimensionality(), c) {
        __id = c.__id;

        __size = 0;
        __dimensionality = c.getDimensionality();
        __points = nullptr;
        __cpy(c.__points);
    }

    Cluster &Cluster::operator=(const Cluster &c) {
        __id = c.__id;
        __cpy(c.__points);
        __size = c.__size;
        return *this;
    }

    Cluster::~Cluster() {
        __del();
    }


    unsigned int Cluster::getSize() const {
        return __size;
    }

    unsigned int Cluster::getDimensionality() const {
        return __dimensionality;
    }

    unsigned int Cluster::getId() const {
        return __id;
    }


    void Cluster::add(const Point &p) {
        if (getDimensionality() != p.getDims()) { throw (DimensionalityMismatchEx(getDimensionality(), p.getDims())); }
        if (!__in(p)) {
            LNodePtr pointToInsertPtr;
            pointToInsertPtr = new LNode(p, __points);
            centroid.setValid(false);
            __size++;
            if (__points == nullptr) {
                __points = pointToInsertPtr;
            } else if (p < __points->point) {
                pointToInsertPtr->next = __points;
                __points = pointToInsertPtr;
            } else {
                LNodePtr leftNodePtr = __points;
                LNodePtr rightNodePtr = __points->next;
                while (true) {
                    if (rightNodePtr == nullptr || rightNodePtr->point > p) {
                        leftNodePtr->next = pointToInsertPtr;
                        pointToInsertPtr->next = rightNodePtr;
                        break;
                    } else {
                        leftNodePtr = rightNodePtr;
                        rightNodePtr = rightNodePtr->next;
                    }
                }

            }

        }
        return;


    }


    const Point &Cluster::remove(const Point &p) {
        if (getDimensionality() != p.getDims()) { throw (DimensionalityMismatchEx(getDimensionality(), p.getDims())); }
        if (__size == 0) { throw (EmptyClusterEx()); }
        if (__in(p)) {
            centroid.setValid(false);
            __size--;
            LNodePtr previous = __points;
            if (previous->point == p) {
                LNodePtr newHead = __points->next;
                LNodePtr oldHead = __points;
                __points = newHead;
                delete oldHead;

            }
            else {
                while (previous->next != nullptr && previous->next->point != p) {
                    previous = previous->next;
                }

                LNodePtr pointToBeDeleted = nullptr;
                LNodePtr pointToBeAssigned = nullptr;

                pointToBeDeleted = previous->next;
                pointToBeAssigned = pointToBeDeleted->next;
                previous->next = pointToBeAssigned;

                delete pointToBeDeleted;
            }

        }

        return p;

    }

    bool Cluster::contains(const Point &p) const {
        return __in(p);
    }

    void Cluster::pickCentroids(unsigned int k, Point **pointArray) {
        if (k >= __size) {
            for (int i = 0; i < k; i++) {
                if (i < __size) {
                    *pointArray[i] = (*this)[i];
                } else {

                    for (int j = 0; j < pointArray[i]->getDims(); j++) {
                        pointArray[i]->setValue(j, std::numeric_limits<double>::max());
                    }

                }
            }
            return;
        } else {
            for (int i = 0; i < k; i++) {
                if (i < __size) {
                    *pointArray[i] = (*this)[i];
                }
            }
        }

    }

    const Point &Cluster::operator[](unsigned int index) const {
        if (__size == 0) { throw (EmptyClusterEx()); }
        if (index >= __size || index < 0) { throw (OutOfBoundsEx(__size, index)); }
        LNodePtr cursor = __points;
        for (int i = 0; i < index; ++i) {
            cursor = cursor->next;
        }

        return cursor->point;
    }


    Cluster &Cluster::operator+=(const Point &p) {
        if (getDimensionality() != p.getDims()) { throw (DimensionalityMismatchEx(getDimensionality(), p.getDims())); }
        if (!__in(p)) {
            add(p);
        }
        return *this;
    }

    Cluster &Cluster::operator-=(const Point &p) {
        if (getDimensionality() != p.getDims()) { throw (DimensionalityMismatchEx(getDimensionality(), p.getDims())); }
        if (__in(p)) {

            remove(p);
        }
        return *this;

    }

    Cluster &Cluster::operator+=(const Cluster &c) {
        for (int i = 0; i < c.__size; i++) {
            Point curPoint(c[i]);
            if (!__in(curPoint)) {
                add(curPoint);
            }
        }
        return *this;
    }

    Cluster &Cluster::operator-=(const Cluster &c) {
        for (int i = 0; i < c.__size; i++) {
            Point curPoint(c[i]);
            if (__in(curPoint)) {
                remove(curPoint);
            }
        }

        return *this;
    }

    std::ostream &operator<<(std::ostream &outStream, const Cluster &c) {
        LNodePtr cursor;
        cursor = c.__points;
        while (cursor != nullptr) {
            Point curPoint = cursor->point;
            outStream << curPoint << ":" << c.getId() << std::endl;

            cursor = cursor->next;


        }
        return outStream;
    }

    std::istream &operator>>(std::istream &inStream, Cluster &c) {
        c.__del();
        std::string line;
        Point curLine(c.getDimensionality());
        while (getline(inStream, line)) {
            std::istringstream iss(line);


            Point curLine(c.getDimensionality());
            try {

                iss >> curLine;
                c.add(curLine);
            } catch (DimensionalityMismatchEx ex) {
                curLine.rewindIdGen();
            }


        }


    }


    bool operator==(const Cluster &rhs, const Cluster &lhs) {
        if (rhs.__dimensionality != lhs.__dimensionality) {
            throw (DimensionalityMismatchEx(rhs.__dimensionality, lhs.__dimensionality));
        }
        if (rhs.getSize() != lhs.getSize()) {
            return 0;
        }

        LNodePtr cursorR = rhs.__points;
        LNodePtr cursorL = lhs.__points;
        for (int i = 0; i < rhs.getSize(); ++i) {
            if ((cursorR->point) != (cursorL->point)) {
                return false;

            }
            cursorR = cursorR->next;
            cursorL = cursorL->next;
        }
        return true;

    }

    bool operator!=(const Cluster &rhs, const Cluster &lhs) {
        return !(rhs == lhs);
    }


    const Cluster operator+(const Cluster &c, const Point &p) {
        Cluster sum(c);
        sum += p;
        return sum;
    }

    const Cluster operator-(const Cluster &c, const Point &p) {
        Cluster difference(c);
        difference -= p;
        return difference;
    }

    // Friends: Arithmetic (two Clusters)
    const Cluster operator+(const Cluster &rhs, const Cluster &lhs) {
        Cluster sum(rhs.getDimensionality());
        sum += rhs;
        sum += lhs;
        return sum;
    }

    const Cluster operator-(const Cluster &rhs, const Cluster &lhs) {
        Cluster difference(rhs);

        difference -= lhs;
        return difference;

    }


    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __p(p), __to(to), __from(from) {
    }

    void Cluster::Move::perform() {

        // cout<<"Moving "<<__p.getDims()<<" From Cluster "<<__from.getId()<<" to "<<__to.getId()<<endl;
        __to.add(__p);
        __from.remove(__p);

    }
}

