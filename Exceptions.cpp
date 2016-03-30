//
// Created by Peter on 3/26/2016.
//

#include "Exceptions.h"

using std::endl;
namespace Clustering {


    OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r) {
        __current = c;
        __rhs = r;
        __name = "OutOfBoundsEx";
    }

    unsigned int OutOfBoundsEx::getCurrent() const {
        return __current;
    }

    int OutOfBoundsEx::getRhs() const {
        return __rhs;
    }

    std::string OutOfBoundsEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const OutOfBoundsEx &ex) {
        os << ex.__name << "c=" << ex.__current << "rhs=" << ex.__rhs;
    }


    DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r) {
        __current = c;
        __rhs = r;
        __name = "DimensionalityMismatchEx";
    }

    unsigned int DimensionalityMismatchEx::getCurrent() const {
        return __current;
    }

    std::string DimensionalityMismatchEx::getName() const {
        return __name;
    }

    unsigned int DimensionalityMismatchEx::getRhs() const {
        return __rhs;
    }

    std::ostream &operator<<(std::ostream &os, const DimensionalityMismatchEx &ex) {
        os << ex.__name << "c=" << ex.__current << "rhs=" << ex.__rhs;
    }

    ZeroClustersEx::ZeroClustersEx() {
        __name = "ZeroClustersEx";
    }

    std::string ZeroClustersEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const ZeroClustersEx &ex) {
        os << ex.__name;
    }

    DataFileOpenEx::DataFileOpenEx(std::string filename) {
        __name = "DataFileOpenEx";
        __filename = filename;
    }

    std::string DataFileOpenEx::getName() const {
        return __name;
    }

    std::string DataFileOpenEx::getFilename() const {
        return __filename;
    }

    std::ostream &operator<<(std::ostream &os, const DataFileOpenEx &ex) {
        os << ex.__name << " while atempting to open " << ex.__filename;
    }

    ZeroDimensionsEx::ZeroDimensionsEx() {
        __name = "ZeroDimensionsEx";
    }

    std::string ZeroDimensionsEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const ZeroDimensionsEx &ex) {
        os << ex.__name;
    }

    EmptyClusterEx::EmptyClusterEx() {
        __name = "EmptyClusterEx";
    }

    std::string EmptyClusterEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const EmptyClusterEx &ex) {
        os << ex.__name;
    }
}