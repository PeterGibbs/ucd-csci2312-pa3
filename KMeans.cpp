//
// Created by Peter on 3/26/2016.
//

//namespace
#include <assert.h>
#include <limits>
#include "KMeans.h"
#include "Exceptions.h"

namespace Clustering {
    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) {

        __dimensionality = dim;
        __maxIter = maxIter;
        __numNonempty = 0;


        if (k == 0) { throw (ZeroClustersEx()); }
        __k = k;
        __clusters = new Cluster *[k];
        __initCentroids = new Point *[k];
        for (int i = 0; i < __k; i++) {
            __clusters[i] = new Cluster(dim);
            __initCentroids[i] = new Point(dim);

        }


        Cluster temp(dim);
        std::ifstream file(filename);
        if (file.is_open()) {

            file >> (*__clusters[0]);
            file.close();
        } else {
            delete[] __clusters;
            delete[] __initCentroids;
            throw (DataFileOpenEx(filename));
        }
        (*__clusters[0]).pickCentroids(k, __initCentroids);
        for (int i = 0; i < __k; i++) {
            __clusters[i]->centroid.set((*__initCentroids[i]));
            __clusters[i]->centroid.setValid(true);


            if (__clusters[i]->centroid.get().getValue(0) != std::numeric_limits<double>::max()) {
                __numNonempty++;
            }
        }


    }


    KMeans::~KMeans() {
        for (int i = 0; i < __k; i++) {
            delete __clusters[i];
        }
        for (int i = 0; i < __k; i++) {
            delete __initCentroids[i];
        }
        delete[] __clusters;
        delete[] __initCentroids;
    }

    Cluster &KMeans::operator[](unsigned int u) {
        return *__clusters[u];
    }

    const Cluster &KMeans::operator[](unsigned int u) const {
        return *__clusters[u];
    }

    unsigned int KMeans::getMaxIter() {
        return __maxIter;
    }

    unsigned int KMeans::getNumIters() {
        return __numIter;
    }

    unsigned int KMeans::getNumNonemptyClusters() {
        return __numNonempty;
    }

    unsigned int KMeans::getNumMovesLastIter() {
        return __numMovesLastIter;
    }

    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans) {
        for (int i = 0; i < kmeans.__k; i++) {
            try {
                os << (*kmeans.__clusters[i]);
            } catch (EmptyClusterEx ex) {

            }
        }
        return os;
    }


    void KMeans::run() {

        typedef class Cluster *clusterPtr;
        unsigned int moves = 100;
        unsigned int iter = 0;
        clusterPtr currentRunning;
        clusterPtr currentComparing;
        clusterPtr closest = nullptr;


        for (int i = 0; i < __numNonempty; i++) {
            assert(__clusters[i]->centroid.get().getValue(0) != std::numeric_limits<double>::max());
        }

        while (moves > 0 && iter < __maxIter) {
            iter++;
            moves = 0;
            for (int i = 0; i < __k; i++) {
                currentRunning = __clusters[i];
                for (int p = 0; p < currentRunning->getSize(); p++) {
                    Point curPoint((*currentRunning)[p]);
                    closest = __clusters[0];
                    Point bestCentroid(__clusters[0]->centroid.get());
                    for (int c = 0; c < __k; c++) {
                        currentComparing = __clusters[c];
                        Point currentCentroid(currentComparing->centroid.get());
                        if (curPoint.distanceTo(currentCentroid) < curPoint.distanceTo(bestCentroid)) {
                            bestCentroid = currentCentroid;
                            closest = currentComparing;
                        }
                    }

                    if (closest != currentRunning) {
                        Cluster::Move move(curPoint, (*currentRunning), (*closest));
                        move.perform();
                        moves++;
                    }
                }

            }
            for (int i = 0; i < __k; i++) {
                if (!__clusters[i]->centroid.isValid()) {
                    __clusters[i]->centroid.compute();
                }
            }

        }


        __numIter = iter;
        __numMovesLastIter = moves;
        unsigned int notEmpty = 0;
        for (int i = 0; i < __k; i++) {
            if ((*__clusters[i]).getSize() > 0) {
                notEmpty++;
            }
        }
        __numNonempty = notEmpty;

    }
}